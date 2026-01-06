// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<vi_replace.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_kb.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(vi_replace) {
	rl42_kb_event	*event;
	size_t			count;
	size_t			len;
	u32				ucp;

	event = kb_listen(-1);
	if (event->code == KB_UNRECOGNIZED_ESCAPE)
		return 1;
	ucp = kb_event_to_ucp(event);
	len = vector_size(line->line);
	count = min((NEED_REPEAT) ? (size_t)n_arg.val : 1, len - line->i);
	do vector_replace(line->line, line->i++, ucp);
	while (--count);
	return term_display_line(line, 0);
}
