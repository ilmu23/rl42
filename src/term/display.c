// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<display.c>>

#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "rl42.h"

#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_terminfo.h"

#define _BUFFER_SIZE	4096

#define _SGR_RESET		escapes[0]
#define _SGR_REV_VIDEO	escapes[1]
#define _SGR_UNDERLINE	escapes[2]
#define _TERM_CLEAR_END	escapes[3]

#define clear()	((ti_tputs(_TERM_CLEAR_END.seq, 1, __putchar) != -1) ? 1 : 0)
#define fetch(esc, name)	(esc.seq = term_get_seq(name), esc.len = (esc.seq) ? strlen(esc.seq) : 0, esc.fetched = 1)

extern rl42_mark	user;

extern u16	term_width;

static struct {
	const char	*seq;
	size_t		len;
	u8			fetched;
}	escapes[4] = {
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
	{ .seq = NULL, .len = 0, .fetched = 0},
};

static char	buf[_BUFFER_SIZE];
static u8	hl_user_mark;

static inline u8	_horizontal_display_line(rl42_line *line, const rl42_display_opts opts, va_list *args);
static inline u8	_add_str_to_buf(cvector s, cvector hl, const rl42_display_opts opts, size_t *i, const size_t start, const size_t max_visible);

u8	term_display_line(rl42_line *line, const rl42_display_opts opts, ...) {
	va_list		args;
	size_t		i;

	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_start(args, opts);
	if (rl42_get(RL42_HORIZONTAL_SCROLL_MODE).u64)
		return _horizontal_display_line(line, opts, &args);
	i = 0;
	hl_user_mark = 0;
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, opts, &i, 0, SIZE_MAX))
			goto _term_display_line_error;
		buf[i++] = ' ';
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, opts, &i, 0, SIZE_MAX))
		goto _term_display_line_error;
	hl_user_mark = user.set;
	if (~opts & DISPLAY_PROMPT_ONLY) {
		if (!_add_str_to_buf(line->line, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(args, cvector) : NULL, opts, &i, 0, SIZE_MAX))
			goto _term_display_line_error;
		if (!term_calculate_required_rows(line, 1))
			goto _term_display_line_error;
	}
	if (!term_cursor_set_pos(line->prompt.root->row, line->prompt.root->col))
		goto _term_display_line_error;
	if (!_TERM_CLEAR_END.fetched)
		fetch(_TERM_CLEAR_END, ti_ed);
	if (!clear() || write(1, buf, i) != (ssize_t)i)
		goto _term_display_line_error;
	return (~opts & DISPLAY_PROMPT_ONLY) ? term_cursor_move_to_i(line) : 1;
_term_display_line_error:
	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_end(args);
	return 0;
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
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, opts, &i, 0, SIZE_MAX))
			goto __horizontal_display_line_error;
		buf[i++] = ' ';
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, opts, &i, 0, SIZE_MAX))
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
		if (!_add_str_to_buf(line->line, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(*args, cvector) : NULL, opts, &i, start, space))
			goto __horizontal_display_line_error;
	}
	if (!term_cursor_move_to(line, line->prompt.root->row, line->prompt.root->col + i))
		goto __horizontal_display_line_error;
	if (!term_cursor_set_pos(line->prompt.root->row, line->prompt.root->col))
		goto __horizontal_display_line_error;
	if (!_TERM_CLEAR_END.fetched)
		fetch(_TERM_CLEAR_END, ti_ed);
	if (!clear() || write(1, buf, i) != (ssize_t)i)
		goto __horizontal_display_line_error;
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

static inline u8	_add_str_to_buf(cvector s, cvector hl, const rl42_display_opts opts, size_t *i, const size_t start, const size_t max_visible) {
	const char	*hl_seq;
	utf8_cbuf	encoded;
	size_t		visible;
	size_t		hl_start;
	size_t		hl_end;
	size_t		size;
	size_t		len;
	size_t		_i;
	u32			ucp;

	hl_start = ((opts & DISPLAY_HIGHLIGHT_IGNORE_CASE) == 0) ? rl42str_find(s, hl) : rl42str_find_case(s, hl);
	hl_end = (hl_start != RL42STR_SUBSTR_NOT_FOUND) ? hl_start + vector_size(hl) : hl_start;
	for (_i = start, visible = 0, size = vector_size(s); *i < _BUFFER_SIZE && _i < size && visible < max_visible; _i++) {
		ucp = *(u32 *)vector_get(s, _i);
		if (_i == user.pos && hl_user_mark) {
			if (!_SGR_UNDERLINE.fetched)
				fetch(_SGR_UNDERLINE, ti_smul);
			if (*i + _SGR_UNDERLINE.len >= _BUFFER_SIZE)
				return 0;
			memcpy(&buf[*i], _SGR_UNDERLINE.seq, _SGR_UNDERLINE.len);
			*i += _SGR_UNDERLINE.len;
		} else if (_i == user.pos + 1 && hl_user_mark) {
			if (!_SGR_RESET.fetched)
				fetch(_SGR_RESET, ti_sgr0);
			if (*i + _SGR_RESET.len >= _BUFFER_SIZE)
				return 0;
			memcpy(&buf[*i], _SGR_RESET.seq, _SGR_RESET.len);
			*i += _SGR_RESET.len;
		}
		if (_i == hl_start) {
			hl_seq = term_get_hl_seq();
			len = strlen(hl_seq);
			if (*i + len >= _BUFFER_SIZE)
				return 0;
			memcpy(&buf[*i], hl_seq, len);
			*i += len;
		} else if (_i == hl_end) {
			if (!_SGR_RESET.fetched)
				fetch(_SGR_RESET, ti_sgr0);
			if (*i + _SGR_RESET.len >= _BUFFER_SIZE)
				return 0;
			memcpy(&buf[*i], _SGR_RESET.seq, _SGR_RESET.len);
			*i += _SGR_RESET.len;
		}
		// TODO: proper printable checking
		if (in_range(ucp, ' ', '~')) {
			if (!utf8_encode(ucp, encoded))
				return 0;
			len = strlen(encoded);
			memcpy(&buf[*i], encoded, len);
			*i += len;
			visible++;
		} else if (ucp < 0x20U || ucp == 0x7FU) {
			if (visible + 1 == max_visible)
				break ;
			if (!_SGR_REV_VIDEO.fetched)
				fetch(_SGR_REV_VIDEO, ti_rev);
			if (!_SGR_RESET.fetched)
				fetch(_SGR_RESET, ti_sgr0);
			if (*i + _SGR_REV_VIDEO.len + _SGR_RESET.len + 2 >= _BUFFER_SIZE)
				return 0;
			memcpy(&buf[*i], _SGR_REV_VIDEO.seq, _SGR_REV_VIDEO.len);
			*i += _SGR_REV_VIDEO.len;
			buf[(*i)++] = '^';
			buf[(*i)++] = (ucp < 0x20) ? (char)ucp + '@' : '?';
			memcpy(&buf[*i], _SGR_RESET.seq, _SGR_RESET.len);
			*i += _SGR_RESET.len;
			visible += 2;
		}
	}
	if (_i == hl_end) {
		if (!_SGR_RESET.fetched)
			fetch(_SGR_RESET, ti_sgr0);
		if (*i + _SGR_RESET.len >= _BUFFER_SIZE)
			return 0;
		memcpy(&buf[*i], _SGR_RESET.seq, _SGR_RESET.len);
		*i += _SGR_RESET.len;
	}
	if (_i <= user.pos && hl_user_mark) {
		if (!_SGR_UNDERLINE.fetched)
			fetch(_SGR_UNDERLINE, ti_smul);
		if (!_SGR_RESET.fetched)
			fetch(_SGR_RESET, ti_sgr0);
		if (*i + _SGR_UNDERLINE.len + _SGR_RESET.len + 1 >= _BUFFER_SIZE)
			return 0;
		memcpy(&buf[*i], _SGR_UNDERLINE.seq, _SGR_UNDERLINE.len);
		*i += _SGR_UNDERLINE.len;
		buf[(*i)++] = ' ';
		memcpy(&buf[*i], _SGR_RESET.seq, _SGR_RESET.len);
		*i += _SGR_RESET.len;
	} else if (_i == user.pos + 1 && hl_user_mark) {
		if (!_SGR_RESET.fetched)
			fetch(_SGR_RESET, ti_sgr0);
		if (*i + _SGR_RESET.len >= _BUFFER_SIZE)
			return 0;
		memcpy(&buf[*i], _SGR_RESET.seq, _SGR_RESET.len);
		*i += _SGR_RESET.len;
	}
	return (*i != _BUFFER_SIZE) ? 1 : 0;
}
