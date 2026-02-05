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
#include "internal/_darray.h"
#include "internal/_display.h"

#ifndef __RL42_USE_EXTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

#define _BUF_SIZE	17

rl42_fn(quoted_insert) {
	size_t	i;
	char	buf[_BUF_SIZE];
	u32		ucp;
	i16		x;
	i16		y;

	term_cursor_get_pos(&x, &y);
	ti_tputs("^", 1, term_putchar_unbuffered);
	term_cursor_set_pos(x, y);
	if (!kb_listen_buf(-1, buf, _BUF_SIZE))
		return 0;
	for (i = 0; buf[i]; i += charsize_utf8(buf[i])) {
		ucp = utf8_decode(&buf[i]);
		if (!darray_insert(line->line, line->i++, ucp))
			return 0;
	}
	return term_display_line(line, 0);
}
