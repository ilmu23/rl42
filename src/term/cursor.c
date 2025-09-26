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

#define _TERM_CURS_POS	"\x1b[6n"

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

u8	term_cursor_move_to(const i16 row, const i16 col) {
	ssize_t	rv;
	char	buf[64];

	rv = snprintf(buf, 64, "\x1b[%hd;%hdH", row, col);
	if (rv >= 64)
		return 0;
	return (write(1, buf, rv) == rv) ? 1 : 0;
}
