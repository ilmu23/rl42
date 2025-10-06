// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<display.c>>

#include <string.h>
#include <unistd.h>

#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

#define _BUFFER_SIZE	4096

#define _SGR_RESET		"\x1b[m"
#define _SGR_REV_VIDEO	"\x1b[7m"
#define _TERM_CLEAR_END	"\x1b[J"

#define clear()	((write(1, _TERM_CLEAR_END, sizeof(_TERM_CLEAR_END) - 1) == (ssize_t)sizeof(_TERM_CLEAR_END) - 1) ? 1 : 0)

static inline u8	_add_str_to_buf(cvector s, char buf[_BUFFER_SIZE], size_t *i);

u8	term_display_line(rl42_line *line, const rl42_display_opts opts) {
	static char	buf[_BUFFER_SIZE];
	size_t		i;

	i = 0;
	if (line->prompt.sprompt) {
		if (!_add_str_to_buf(line->prompt.sprompt, buf, &i))
			return 0;
		buf[i++] = ' ';
	}
	if (!_add_str_to_buf(line->prompt.prompt, buf, &i))
		return 0;
	if (~opts & DISPLAY_PROMPT_ONLY && !_add_str_to_buf(line->line, buf, &i))
		return 0;
	if (!term_cursor_set_pos(line->prompt.root.row, line->prompt.root.col))
		return 0;
	if (!clear() || write(1, buf, i) != (ssize_t)i)
		return 0;
	if (opts & DISPLAY_PROMPT_ONLY)
		return 1;
	return term_cursor_move_to_i(line);
}

static inline u8	_add_str_to_buf(cvector s, char buf[_BUFFER_SIZE], size_t *i) {
	utf8_cbuf	encoded;
	size_t		size;
	size_t		len;
	size_t		_i;
	u32			ucp;

	for (_i = 0, size = vector_size(s); *i < _BUFFER_SIZE && _i < size; _i++) {
		ucp = *(u32 *)vector_get(s, _i);
		// TODO: proper printable checking
		if (in_range(ucp, ' ', '~')) {
			if (!utf8_encode(ucp, encoded))
				return 0;
			len = strlen(encoded);
			memcpy(&buf[*i], encoded, len);
			*i += len;
		} else if (ucp < 0x20U || ucp == 0x7FU) {
			if (*i + (sizeof(_SGR_REV_VIDEO) - 1 + sizeof(_SGR_RESET) - 1 + 2) >= _BUFFER_SIZE)
				return 0;
			memcpy(&buf[*i], _SGR_REV_VIDEO, sizeof(_SGR_REV_VIDEO) - 1);
			*i += sizeof(_SGR_REV_VIDEO) - 1;
			buf[(*i)++] = '^';
			buf[(*i)++] = (ucp < 0x20) ? (char)ucp + '@' : '?';
			memcpy(&buf[*i], _SGR_RESET, sizeof(_SGR_RESET) - 1);
			*i += sizeof(_SGR_RESET) - 1;
		}
	}
	return (*i != _BUFFER_SIZE) ? 1 : 0;
}
