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

#define _BUFFER_SIZE	2048

static inline u8	_add_str_to_buf(cvector s, char buf[_BUFFER_SIZE], size_t *i);

u8	term_display_line(const rl42_line *line, [[maybe_unused]] const rl42_display_opts opts) {
	static char	buf[_BUFFER_SIZE];
	size_t		i;

	i = 0;
	if (line->prompt.sprompt && !_add_str_to_buf(line->prompt.sprompt, buf, &i))
		return 0;
	if (!_add_str_to_buf(line->prompt.prompt, buf, &i))
		return 0;
	if (!_add_str_to_buf(line->line, buf, &i))
		return 0;
	if (!term_cursor_move_to(line->cursor.prompt_row, line->cursor.prompt_col))
		return 0;
	return (write(1, buf, i) == (ssize_t)i) ? 1 : 0;
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
		}
	}
	return (*i != _BUFFER_SIZE) ? 1 : 0;
}
