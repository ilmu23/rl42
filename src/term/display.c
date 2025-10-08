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

static u8	hl_user_mark;

static inline u8	_add_str_to_buf(cvector s, cvector hl, char buf[_BUFFER_SIZE], size_t *i);

u8	term_display_line(rl42_line *line, const rl42_display_opts opts, ...) {
	static char	buf[_BUFFER_SIZE];
	va_list		args;
	size_t		i;

	i = 0;
	hl_user_mark = 0;
	if (opts & DISPLAY_HIGHLIGHT_SUBSTR)
		va_start(args, opts);
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, NULL, buf, &i))
			goto _term_display_line_error;
		buf[i++] = ' ';
	}
	if (!_add_str_to_buf(line->prompt.prompt, NULL, buf, &i))
		goto _term_display_line_error;
	hl_user_mark = user.set;
	if (~opts & DISPLAY_PROMPT_ONLY && !_add_str_to_buf(line->line, (opts & DISPLAY_HIGHLIGHT_SUBSTR) ? va_arg(args, cvector) : NULL, buf, &i))
		goto _term_display_line_error;
	if (!term_cursor_set_pos(line->prompt.root.row, line->prompt.root.col))
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

static inline u8	_add_str_to_buf(cvector s, cvector hl, char buf[_BUFFER_SIZE], size_t *i) {
	const char	*hl_seq;
	utf8_cbuf	encoded;
	size_t		hl_start;
	size_t		hl_end;
	size_t		size;
	size_t		len;
	size_t		_i;
	u32			ucp;

	hl_start = rl42str_find(s, hl);
	hl_end = (hl_start != RL42STR_SUBSTR_NOT_FOUND) ? hl_start + vector_size(hl) : hl_start;
	for (_i = 0, size = vector_size(s); *i < _BUFFER_SIZE && _i < size; _i++) {
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
		} else if (ucp < 0x20U || ucp == 0x7FU) {
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
