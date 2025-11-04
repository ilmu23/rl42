// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<utils.c>>

#include <unistd.h>

#include "internal/_map.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"

#define _TERM_SCROLL_UP		"\x1b[S"
#define _TERM_SCROLL_DOWN	"\x1b[T"

extern u16	term_height;
extern u16	term_width;

extern map	anchors;

static size_t	to_scroll;

static void	_scroll_down(rl42_cursor_pos **anchor);
static void	_scroll_up(rl42_cursor_pos **anchor);

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
