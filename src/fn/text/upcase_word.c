// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<upcase_word.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_display.h"

rl42_fn(upcase_word) {
	size_t	i;
	size_t	len;
	u32		*word;

	len = darray_size(line->line);
	if (len == 0)
		return 1;
	i = line->i;
	if (is_space(*(u32 *)darray_get(line->line, (i != 0) ? i - 1 : i)))
		return 1;
	while (i > 0 && !is_space(*(u32 *)darray_get(line->line, i - 1)))
		i--;
	word = (u32 *)darray_get(line->line, i);
	do
		darray_set(line->line, i++, (u32){to_upper(*word++)});
	while (i < len && !is_space(*word));
	return term_display_line(line, 0);
}
