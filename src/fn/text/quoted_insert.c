// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<quoted_insert.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_kb.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_terminfo.h"

#define _BUF_SIZE	17

rl42_fn(quoted_insert) {
	size_t	i;
	char	buf[_BUF_SIZE];
	u32		ucp;
	i16		x;
	i16		y;

	term_cursor_get_pos(&x, &y);
	ti_tputs("^", 1, __putchar);
	term_cursor_set_pos(x, y);
	if (!kb_listen_buf(-1, buf, _BUF_SIZE))
		return 0;
	for (i = 0; buf[i]; i += charsize_utf8(buf[i])) {
		ucp = utf8_decode(&buf[i]);
		if (!vector_push(line->line, ucp))
			return 0;
		line->i++;
	}
	return term_display_line(line, 0);
}
