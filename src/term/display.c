// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<display.c>>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "rl42.h"

#include "internal/_defs.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_terminfo.h"

#define _BUFFER_SIZE	16

#define _SGR_RESET			escapes[0]
#define _SGR_REV_VIDEO		escapes[1]
#define _SGR_UNDERLINE		escapes[2]
#define _TERM_CLEAR_END_SCR	escapes[3]
#define _TERM_CLEAR_END_LNE	escapes[4]

#define clear_screen()	((ti_tputs(_TERM_CLEAR_END_SCR.seq, 1, term_putchar_unbuffered) != -1) ? 1 : 0)
#define clear_line()	((ti_tputs(_TERM_CLEAR_END_LNE.seq, 1, term_putchar_unbuffered) != -1) ? 1 : 0)
#define fetch(esc, name)	(esc.seq = term_get_seq(name), esc.len = (esc.seq) ? strlen(esc.seq) : 0, esc.fetched = 1)

typedef struct {
	cvector	text;
	size_t	cursor_offset;
}	hscroll_section;

#define hscroll_section(t, o)	((hscroll_section){ .text = t, .cursor_offset = o })

extern rl42_mark	user;

extern u16	term_width;

static struct {
	const char	*seq;
	size_t		len;
	u8			fetched;
}	escapes[5] = {
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
};

static u8	hl_user_mark;

static inline hscroll_section	_extract_section(const rl42_line *line, const size_t space);
static inline const char		*_fmt_cntrl(const u32 ucp);
static inline size_t			_calculate_required_space(const rl42_line *line);
static inline size_t			_calculate_start_pos(const rl42_line *line);
static inline size_t			_calculate_escaped_length(u32 ucp);
static inline u8				_pad_partial_escape(vector text, const size_t seq_len);
static inline u8				_horizontal_display_line(rl42_line *line, const rl42_display_opts opts, va_list *args);
static inline u8				_add_str_to_buf(cvector s, cvector hl, const rl42_display_opts opts);

u8	term_display_line(rl42_line *line, const rl42_display_opts opts, ...) {
	va_list		args;

	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_start(args, opts);
	if (rl42_get(RL42_HORIZONTAL_SCROLL_MODE).u64)
		return _horizontal_display_line(line, opts, &args);
	hl_user_mark = 0;
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, opts))
			goto _term_display_line_error;
		term_putchar(' ');
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, opts))
		goto _term_display_line_error;
	hl_user_mark = user.set;
	if (~opts & DISPLAY_PROMPT_ONLY) {
		if (!_add_str_to_buf(line->line, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(args, cvector) : NULL, opts))
			goto _term_display_line_error;
		if (!term_calculate_required_rows(line, 1))
			goto _term_display_line_error;
	}
	if (!term_cursor_set_pos(line->prompt.root->row, line->prompt.root->col))
		goto _term_display_line_error;
	if (!_TERM_CLEAR_END_SCR.fetched)
		fetch(_TERM_CLEAR_END_SCR, ti_ed);
	if (!clear_screen() || !term_flush_outbuf())
		goto _term_display_line_error;
	return (~opts & DISPLAY_PROMPT_ONLY) ? term_cursor_move_to_i(line) : 1;
_term_display_line_error:
	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_end(args);
	return 0;
}

static inline hscroll_section	_extract_section(const rl42_line *line, const size_t space) {
	hscroll_section	section;
	const char		*cntrl_esc;
	size_t			req_space;
	size_t			visible_r;
	size_t			visible_l;
	size_t			esc_len;
	size_t			start;
	size_t			half;
	size_t			len;
	size_t			i;
	size_t			j;

	req_space = _calculate_required_space(line);
	if (req_space < space)
		return hscroll_section(vector_copy(line->line, NULL), _calculate_start_pos(line));
	half = space / 2;
	len = vector_size(line->line);
	start = _calculate_start_pos(line);
	if (start < half + (half & 1)) {
		for (i = visible_r = 0; i < len; i++) {
			visible_r += _calculate_escaped_length(*(u32 *)vector_get(line->line, i));
			if (visible_r >= space + (space & 1) + 1)
				break ;
		}
		section = hscroll_section(vector_copy_range(line->line, 0, i, NULL), start);
		if (!section.text)
			goto __extract_section_err;
	} else if (req_space - start < half + (half & 1)) {
		for (i = len - 1, visible_l = 0; i != (size_t)-1; i--) {
			if (i == line->i - 1)
				start = visible_l;
			visible_l += _calculate_escaped_length(*(u32 *)vector_get(line->line, i));
			if (visible_l >= space)
				break ;
		}
		section = hscroll_section(vector_copy_range(line->line, i, vector_size(line->line), NULL), visible_l - start);
		if (!section.text)
			goto __extract_section_err;
		if (visible_l > space) {
			cntrl_esc = _fmt_cntrl(*(u32 *)vector_get(line->line, i));
			esc_len = strlen(cntrl_esc) - (visible_l - space);
			if (!_pad_partial_escape((vector)section.text, esc_len))
				goto __extract_section_err;
		}
	} else {
		for (i = line->i - 1, visible_l = 0; i != (size_t)-1; i--) {
			visible_l += _calculate_escaped_length(*(u32 *)vector_get(line->line, i));
			if (visible_l >= half)
				break ;
		}
		for (j = line->i, visible_r = 0; j < len; j++) {
			visible_r += _calculate_escaped_length(*(u32 *)vector_get(line->line, j));
			if (visible_r >= half + (half & 1) + 1)
				break ;
		}
		section = hscroll_section(vector_copy_range(line->line, i, j, NULL), half);
		if (visible_l > half) {
			cntrl_esc = _fmt_cntrl(*(u32 *)vector_get(line->line, i));
			esc_len = strlen(cntrl_esc) - (visible_l - half);
			if (!_pad_partial_escape((vector)section.text, esc_len))
				goto __extract_section_err;
		}
	}
	return section;
__extract_section_err:
	vector_delete((vector)section.text);
	return hscroll_section(NULL, 0);
}

static inline const char	*_fmt_cntrl(const u32 ucp) {
	static char	esc_buf[_BUFFER_SIZE];
	ssize_t		rv;

	if (ucp <= 0x7FU)
		rv = snprintf(esc_buf, _BUFFER_SIZE, "^%c", (ucp != 0x7FU) ? (char)ucp + '@' : '?');
	else
		rv = snprintf(esc_buf, _BUFFER_SIZE, "\\u%x", ucp);
	return (rv != -1) ? esc_buf : NULL;
}

static inline size_t	_calculate_required_space(const rl42_line *line) {
	size_t	req;
	size_t	len;
	size_t	i;

	for (i = req = 0, len = vector_size(line->line); i < len; i++)
		req += _calculate_escaped_length(*(u32 *)vector_get(line->line, i));
	return req;
}

static inline size_t	_calculate_start_pos(const rl42_line *line) {
	size_t	start;
	size_t	i;

	for (i = start = 0; i < line->i; i++)
		start += _calculate_escaped_length(*(u32 *)vector_get(line->line, i));
	return start;
}

static inline size_t	_calculate_escaped_length(u32 ucp) {
	size_t	len;

	if (is_print(ucp))
		return 1;
	if (ucp <= 0x7FU)
		return 2;
	for (len = 1; ucp > 15; len++)
		ucp /= 16;
	return len + 2;
}

static inline u8	_pad_partial_escape(vector text, const size_t seq_len) {
	size_t	i;

	if (!vector_resize(text, vector_size(text) + seq_len - 1))
		return 0;
	vector_erase(text, 0);
	for (i = 0; i < seq_len; i++)
		vector_insert(text, 0, (u32){' '});
	return 1;
}

static inline u8	_horizontal_display_line(rl42_line *line, const rl42_display_opts opts, va_list *args) {
	hscroll_section	section;
	size_t			space;
	u8				rv;

	space = 0;
	hl_user_mark = 0;
	section.text = NULL;
	state_flags |= STATE_H_SCROLLING;
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, opts))
			goto __horizontal_display_line_error;
		term_putchar(' ');
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, opts))
		goto __horizontal_display_line_error;
	if (~opts & DISPLAY_PROMPT_ONLY) {
		hl_user_mark = user.set;
		space = calculate_scroll_space(line);
		if (space >= term_width)
			space = max(term_width - 1, 1);
		section = _extract_section(line, space);
		if (!section.text)
			goto __horizontal_display_line_error;
		if (!_add_str_to_buf(section.text, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(*args, cvector) : NULL, opts))
			goto __horizontal_display_line_error;
	}
	if (!term_cursor_set_pos(line->prompt.root->row, line->prompt.root->col))
		goto __horizontal_display_line_error;
	if (~opts & DISPLAY_FORCE_SCREEN_CLEAR) {
		if (!_TERM_CLEAR_END_LNE.fetched)
			fetch(_TERM_CLEAR_END_LNE, ti_el);
		if (!clear_line() || !term_flush_outbuf())
			goto __horizontal_display_line_error;
	} else {
		if (!_TERM_CLEAR_END_SCR.fetched)
			fetch(_TERM_CLEAR_END_SCR, ti_ed);
		if (!clear_screen() || !term_flush_outbuf())
			goto __horizontal_display_line_error;
	}
	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_end(*args);
	if (opts && DISPLAY_PROMPT_ONLY)
		return 1;
	rv = term_cursor_move_to(line, line->root->row, term_width - space + section.cursor_offset - 1);
	vector_delete((vector)section.text);
	state_flags &= ~STATE_H_SCROLLING;
	return rv;
__horizontal_display_line_error:
	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_end(*args);
	vector_delete((vector)section.text);
	state_flags &= ~STATE_H_SCROLLING;
	return 0;
}

static inline u8	_add_str_to_buf(cvector s, cvector hl, const rl42_display_opts opts) {
	const char	*cntrl_esc;
	const char	*hl_seq;
	utf8_cbuf	encoded;
	size_t		hl_start;
	size_t		hl_end;
	size_t		size;
	size_t		i;
	u32			ucp;

	hl_start = ((opts & DISPLAY_HIGHLIGHT_IGNORE_CASE) == 0) ? rl42str_find(s, hl) : rl42str_find_case(s, hl);
	hl_end = (hl_start != RL42STR_SUBSTR_NOT_FOUND) ? hl_start + vector_size(hl) : hl_start;
	for (i = 0, size = vector_size(s); i < size; i++) {
		ucp = *(u32 *)vector_get(s, i);
		if (i == user.pos && hl_user_mark) {
			if (!_SGR_UNDERLINE.fetched)
				fetch(_SGR_UNDERLINE, ti_smul);
			if (ti_tputs(_SGR_UNDERLINE.seq, 1, term_putchar) == -1)
				return 0;
		} else if (i == user.pos + 1 && hl_user_mark) {
			if (!_SGR_RESET.fetched)
				fetch(_SGR_RESET, ti_sgr0);
			if (ti_tputs(_SGR_RESET.seq, 1, term_putchar) == -1)
				return 0;
		}
		if (i == hl_start) {
			hl_seq = term_get_hl_seq();
			if (ti_tputs(hl_seq, 1, term_putchar) == -1)
				return 0;
		} else if (i == hl_end) {
			if (!_SGR_RESET.fetched)
				fetch(_SGR_RESET, ti_sgr0);
			if (ti_tputs(_SGR_RESET.seq, 1, term_putchar) == -1)
				return 0;
		}
		if (is_print(ucp)) {
			if (!utf8_encode(ucp, encoded))
				return 0;
			if (ti_tputs(encoded, 1, term_putchar) == -1)
				return 0;
		} else {
			cntrl_esc = _fmt_cntrl(ucp);
			if (!cntrl_esc)
				return 0;
			if (!_SGR_REV_VIDEO.fetched)
				fetch(_SGR_REV_VIDEO, ti_rev);
			if (!_SGR_RESET.fetched)
				fetch(_SGR_RESET, ti_sgr0);
			if (ti_tputs(_SGR_REV_VIDEO.seq, 1, term_putchar) == -1)
				return 0;
			if (ti_tputs(cntrl_esc, 1, term_putchar) == -1)
				return 0;
			if (ti_tputs(_SGR_RESET.seq, 1, term_putchar) == -1)
				return 0;
		}
	}
	if (i == hl_end) {
		if (!_SGR_RESET.fetched)
			fetch(_SGR_RESET, ti_sgr0);
		if (ti_tputs(_SGR_RESET.seq, 1, term_putchar) == -1)
			return 0;
	}
	if (i <= user.pos && hl_user_mark) {
		if (!_SGR_UNDERLINE.fetched)
			fetch(_SGR_UNDERLINE, ti_smul);
		if (!_SGR_RESET.fetched)
			fetch(_SGR_RESET, ti_sgr0);
		if (ti_tputs(_SGR_UNDERLINE.seq, 1, term_putchar) == -1)
			return 0;
		if (term_putchar(' ') == -1)
			return 0;
		if (ti_tputs(_SGR_RESET.seq, 1, term_putchar) == -1)
			return 0;
	} else if (i == user.pos + 1 && hl_user_mark) {
		if (!_SGR_RESET.fetched)
			fetch(_SGR_RESET, ti_sgr0);
		if (ti_tputs(_SGR_RESET.seq, 1, term_putchar) == -1)
			return 0;
	}
	return 1;
}
