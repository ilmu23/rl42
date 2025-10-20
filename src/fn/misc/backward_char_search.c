// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<backward_char_search.c>>

#include "internal/_kb.h"
#include "internal/_term.h"
#include "internal//_vector.h"

#include "internal/fn/misc.h"

rl42_fn(backward_char_search) {
	size_t	i;
	u32		c;

	if (get_numeric_arg(line, 1) < 0)
		return forward_char_search(line);
	if (line->i == 0)
		return 1;
	c = kb_event_to_ucp(kb_listen(-1));
	i = line->i;
	while (i > 0)
		if (c == *(u32 *)vector_get(line->line, --i))
			break ;
	if (i == (size_t)-1)
		return 1;
	line->i = i;
	return term_cursor_move_to_i(line);
}
