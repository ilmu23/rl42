// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<utils.c>>

#include <string.h>
#include <unistd.h>

#include "internal/_map.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"

#define _TERM_SCROLL_UP		"\x1b[S"
#define _TERM_SCROLL_DOWN	"\x1b[T"

#define _csi_match(s, c)	((csi_match){.start = s, .complete = c})

extern u16	term_height;
extern u16	term_width;

extern map	anchors;

static size_t	to_scroll;

static void	_scroll_down(rl42_cursor_pos **anchor);
static void	_scroll_up(rl42_cursor_pos **anchor);

csi_match	term_find_csi(const char *buf, const size_t buf_size, const char ident) {
	const char	*start;
	size_t		offset;
	size_t		i;

	offset = 0;
	start = NULL;
_term_find_csi_find_esc:
	start = cstr_find_char(&buf[offset], buf_size - offset, '\x1b');
	if (!start)
		return _csi_match(NULL, 0);
	offset = (size_t)((uintptr_t)start - (uintptr_t)buf) + 1;
	if (offset >= buf_size)
		return _csi_match(start, 0);
	if (*(++start) != '[')
		goto _term_find_csi_find_esc;
	i = 1;
	while (offset + i < buf_size && in_range(start[i], '\x30', '\x3f'))
		i++;
	while (offset + i < buf_size && in_range(start[i], '\x20', '\x2f'))
		i++;
	if (offset + i >= buf_size)
		return _csi_match(--start, 0);
	if (start[i] != ident) {
		offset += i;
		goto _term_find_csi_find_esc;
	}
	return _csi_match(--start, 1);
}

u8	term_calculate_required_rows(rl42_line *line, const u8 scroll) {
	size_t	orig_i;
	i16		col;

	line->rows = 1;
	orig_i = line->i;
	line->i = SIZE_MAX;
	col = line->root->col + calculate_cursor_offset(line) + ((line->prompt.sprompt) ? vector_size(line->prompt.sprompt) : 0);
	line->i = orig_i;
	while (col > term_width) {
		col -= term_width;
		line->rows++;
	}
	return (line->root->row + line->rows - 1 > term_height && scroll) ? term_scroll_display(line->rows - 1, 0) : 1;
}

u8	term_scroll_display(size_t up, size_t down) {
	if (up) {
		to_scroll = up;
		map_foreach(anchors, (void (*)(void *))_scroll_up);
		while (up--)
			if (write(1, _TERM_SCROLL_UP, sizeof(_TERM_SCROLL_UP) - 1) != (ssize_t)sizeof(_TERM_SCROLL_UP) - 1)
				return 0;
	} else if (down) {
		to_scroll = down;
		map_foreach(anchors, (void (*)(void *))_scroll_down);
		while (down--)
			if (write(1, _TERM_SCROLL_DOWN, sizeof(_TERM_SCROLL_DOWN) - 1) != (ssize_t)sizeof(_TERM_SCROLL_DOWN) - 1)
				return 0;
	}
	return 1;
}

static void	_scroll_down(rl42_cursor_pos **anchor) {
	(*anchor)->row += to_scroll;
}

static void	_scroll_up(rl42_cursor_pos **anchor) {
	(*anchor)->row -= to_scroll;
}
