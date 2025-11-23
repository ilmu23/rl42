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
#include <string.h>
#include <unistd.h>

#include "rl42.h"

#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

#define _BUF_SIZE	64

#define _CSI_DSR	"\x1b[6n" // Device Status Report / Report Cursor Position

extern vector	input_buf;

extern u16	term_width;
extern u16	term_height;

map	anchors;

const rl42_cursor_pos	*term_cursor_new_anchor(void) {
	rl42_cursor_pos	*anchor;

	if (!anchors) {
		anchors = map(rl42_cursor_pos *, 8, INTEGER, free);
		if (!anchors)
			return NULL;
	}
	anchor = malloc(sizeof(*anchor));
	if (anchor && (!term_cursor_get_pos(&anchor->row, &anchor->col) || !map_set(anchors, anchor, anchor))) {
		free(anchor);
		return NULL;
	}
	return anchor;
}

void	term_cursor_delete_anchor(const rl42_cursor_pos *anchor) {
	map_erase(anchors, anchor);
}

u8	term_cursor_get_pos(i16 *row, i16 *col) {
	csi_match	cpr;
	ssize_t		rv;
	size_t		i;
	char		*end;
	char		buf[_BUF_SIZE];

	write(1, _CSI_DSR, sizeof(_CSI_DSR) - 1);
	i = 0;
_term_cursor_get_pos_read:
	rv = read(0, &buf[i], _BUF_SIZE - i);
	if (rv == -1)
		return 0;
	cpr = term_find_csi(buf, rv + i, CSI_CPR);
	if (!cpr.start) {
		if (!vector_insert_n(input_buf, -1, rv + i, buf))
			return 0;
		i = 0;
		goto _term_cursor_get_pos_read;
	}
	if (cpr.start != buf) {
		i = (size_t)((uintptr_t)cpr.start - (uintptr_t)buf);
		if (!vector_insert_n(input_buf, -1, i, buf))
			return 0;
		memmove(buf, &buf[i], _BUF_SIZE - i);
		memset(&buf[_BUF_SIZE - i], 0, i);
		if (!cpr.complete)
			goto _term_cursor_get_pos_read;
		rv -= i;
	}
	i = 0;
	do i++;
	while (!isdigit(buf[i]));
	*row = (u16)strtoul(&buf[i], &end, 10);
	i += (uintptr_t)end - ((uintptr_t)buf + i);
	do i++;
	while (!isdigit(buf[i]));
	*col = (u16)strtoul(&buf[i], &end, 10);
	if (*(++end) && !vector_insert_n(input_buf, -1, rv - i, end))
		return 0;
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

	if (rl42_get(RL42_HORIZONTAL_SCROLL_MODE).u64 && ~state_flags & STATE_H_SCROLLING)
		return term_display_line(line, 0);
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
	if (!term_scroll_display(up, down))
		return 0;
	return term_cursor_set_pos(row, col);
}

u8	term_cursor_next_line(void) {
	rl42_cursor_pos	pos;

	if (!term_cursor_get_pos(&pos.row, &pos.col))
		return 0;
	return term_cursor_move_to(NULL, pos.row + 1, 1);
}
