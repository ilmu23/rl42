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

static inline const char	*_fmt_cntrl(const u32 ucp);
static inline u8			_horizontal_display_line(rl42_line *line, const rl42_display_opts opts, va_list *args);
static inline u8			_add_str_to_buf(cvector s, cvector hl, const rl42_display_opts opts, const size_t start, const size_t max_visible);

u8	term_display_line(rl42_line *line, const rl42_display_opts opts, ...) {
	va_list		args;

	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_start(args, opts);
	if (rl42_get(RL42_HORIZONTAL_SCROLL_MODE).u64)
		return _horizontal_display_line(line, opts, &args);
	hl_user_mark = 0;
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, opts, 0, SIZE_MAX))
			goto _term_display_line_error;
		term_putchar(' ');
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, opts, 0, SIZE_MAX))
		goto _term_display_line_error;
	hl_user_mark = user.set;
	if (~opts & DISPLAY_PROMPT_ONLY) {
		if (!_add_str_to_buf(line->line, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(args, cvector) : NULL, opts, 0, SIZE_MAX))
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

static inline const char	*_fmt_cntrl(const u32 ucp) {
	static char	esc_buf[_BUFFER_SIZE];
	ssize_t		rv;

	if (ucp <= 0x7FU)
		rv = snprintf(esc_buf, _BUFFER_SIZE, "^%c", (ucp != 0x7FU) ? (char)ucp + '@' : '?');
	else
		rv = snprintf(esc_buf, _BUFFER_SIZE, "\\u%x", ucp);
	return (rv != -1) ? esc_buf : NULL;
}

static inline u8	_horizontal_display_line(rl42_line *line, const rl42_display_opts opts, va_list *args) {
	size_t	offset;
	size_t	start;
	size_t	space;
	size_t	i;
	u8		rv;

	i = 0;
	hl_user_mark = 0;
	state_flags |= STATE_H_SCROLLING;
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, opts, 0, SIZE_MAX))
			goto __horizontal_display_line_error;
		term_putchar(' ');
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, opts, 0, SIZE_MAX))
		goto __horizontal_display_line_error;
	if (~opts & DISPLAY_PROMPT_ONLY) {
		hl_user_mark = user.set;
		space = calculate_scroll_space(line);
		if (space >= term_width && ~opts & DISPLAY_PROMPT_ONLY) {
			space = term_width - 1;
			i = 0;
		}
		if (!space)
			space = 1;
		start = line->i;
		if (start < space / 2 || vector_size(line->line) <= space) {
			offset = start - 1;
			start = 0;
		} else if (vector_size(line->line) - start < space / 2) {
			offset = space - (vector_size(line->line) - start) - 1;
			start = vector_size(line->line) - space;
		} else {
			offset = space / 2 - 1;
			start = line->i - space / 2;
		}
		if (!_add_str_to_buf(line->line, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(*args, cvector) : NULL, opts, start, space))
			goto __horizontal_display_line_error;
	}
	if (!term_cursor_move_to(line, line->prompt.root->row, line->prompt.root->col + i))
		goto __horizontal_display_line_error;
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
	rv = term_cursor_move_to(line, line->root->row, term_width - space + offset);
	state_flags &= ~STATE_H_SCROLLING;
	return rv;
__horizontal_display_line_error:
	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_end(*args);
	state_flags &= ~STATE_H_SCROLLING;
	return 0;
}

static inline u8	_add_str_to_buf(cvector s, cvector hl, const rl42_display_opts opts, const size_t start, const size_t max_visible) {
	const char	*cntrl_esc;
	const char	*hl_seq;
	utf8_cbuf	encoded;
	size_t		visible;
	size_t		hl_start;
	size_t		hl_end;
	size_t		size;
	size_t		len;
	size_t		i;
	u32			ucp;

	hl_start = ((opts & DISPLAY_HIGHLIGHT_IGNORE_CASE) == 0) ? rl42str_find(s, hl) : rl42str_find_case(s, hl);
	hl_end = (hl_start != RL42STR_SUBSTR_NOT_FOUND) ? hl_start + vector_size(hl) : hl_start;
	for (i = start, visible = 0, size = vector_size(s); i < size && visible < max_visible; i++) {
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
			visible++;
		} else {
			cntrl_esc = _fmt_cntrl(ucp);
			if (!cntrl_esc)
				return 0;
			len = strlen(cntrl_esc);
			if (visible + len > max_visible)
				break ;
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
			visible += len;
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
