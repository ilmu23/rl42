// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<cursor.c>>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "internal/_term.h"
#include "internal/_utils.h"

#define _TERM_CURS_POS	"\x1b[6n"

#define _TERM_SCROLL_UP		"\x1b[S"
#define _TERM_SCROLL_DOWN	"\x1b[T"

extern u16	term_width;
extern u16	term_height;

u8	term_cursor_get_pos(i16 *row, i16 *col) {
	ssize_t	rv;
	size_t	i;
	char	*end;
	char	buf[64];

	write(1, _TERM_CURS_POS, sizeof(_TERM_CURS_POS) - 1);
	rv = read(0, buf, 64);
	if (rv == -1)
		return 0;
	// TODO: check that we actually got the response instead of
	// some user input that was buffered before it
	for (i = 0; i < (size_t)rv; i++)
		if (isdigit(buf[i]))
			break ;
	if (i == (size_t)rv)
		return 0;
	*row = (u16)strtol(&buf[i], &end, 10);
	i += (uintptr_t)end - ((uintptr_t)buf + i);
	while (i < (size_t)rv) {
		if (isdigit(buf[i]))
			break ;
		i++;
	}
	if (i == (size_t)rv)
		return 0;
	*col = (u16)strtol(&buf[i], NULL, 10);
	return 1;
}

u8	term_cursor_set_pos(const i16 row, const i16 col) {
	ssize_t	rv;
	char	buf[64];

	rv = snprintf(buf, 64, "\x1b[%hd;%hdH", row, col);
	if (rv >= 64)
		return 0;
	return (write(1, buf, rv) == rv) ? 1 : 0;
}

u8	term_cursor_move_to(rl42_line *line, i16 row, i16 col) {
	size_t	down;
	size_t	up;

	up = 0;
	down = 0;
	if (!in_range(col, 1, term_width)) {
		if (col > term_width) do {
			col -= term_width;
			row++;
		} while (col > term_width); else while (col < 1) {
			col += term_width;
			row--;
		}
	}
	if (!in_range(row, 1, term_height)) {
		if (row > term_height) do {
			row--;
			up++;
		} while (row > term_height); else while (row < 1) {
			row++;
			down++;
		}
	}
	line->prompt.root.row += up - down;
	line->root.row += up - down;
	if (up) do {
		if (write(1, _TERM_SCROLL_UP, sizeof(_TERM_SCROLL_UP) - 1) != (ssize_t)sizeof(_TERM_SCROLL_UP) - 1)
			return 0;
	} while (--up); else if (down) do {
		if (write(1, _TERM_SCROLL_DOWN, sizeof(_TERM_SCROLL_DOWN) - 1) != (ssize_t)sizeof(_TERM_SCROLL_DOWN) - 1)
			return 0;
	} while (--down);
	return term_cursor_set_pos(row, col);
}
