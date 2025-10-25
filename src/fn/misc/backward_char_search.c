// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<backward_char_search.c>>

#include <ctype.h>

#include "rl42.h"

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
	if (rl42_get(RL42_SEARCH_IGNORE_CASE).u64 == 0) {
		while (i > 0)
			if (c == *(u32 *)vector_get(line->line, --i))
				break ;
	} else {
		c = (u32)toupper(c);
		while (i > 0)
			if (c == (u32)toupper(*(u32 *)vector_get(line->line, --i)))
				break ;
	}
	if (i == (size_t)-1)
		return 1;
	line->i = i;
	return term_cursor_move_to_i(line);
}
