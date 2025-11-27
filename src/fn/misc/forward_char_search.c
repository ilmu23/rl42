// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<forward_char_search.c>>

#include "rl42.h"

#include "internal/_kb.h"
#include "internal/_defs.h"
#include "internal/_term.h"
#include "internal/_vector.h"

#include "internal/fn/misc.h"

rl42_fn(forward_char_search) {
	size_t	len;
	size_t	i;
	u32		c;

	if (get_numeric_arg(line, 1) < 0)
		return backward_char_search(line);
	len = vector_size(line->line);
	if (line->i >= len - 1)
		return 1;
	c = kb_event_to_ucp(kb_listen(-1));
	if (rl42_get(RL42_SEARCH_IGNORE_CASE).u64 == 0) {
		for (i = line->i + 1; i < len; i++)
			if (c == *(u32 *)vector_get(line->line, i))
				break ;
	} else {
		c = (u32)to_upper(c);
		for (i = line->i + 1; i < len; i++)
			if (c == (u32)to_upper(*(u32 *)vector_get(line->line, i)))
				break ;
	}
	if (i == len)
		return 1;
	line->i = i;
	return term_cursor_move_to_i(line);
}
