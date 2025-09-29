// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<downcase_word.c>>

#include <ctype.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(downcase_word) {
	size_t	i;
	size_t	len;
	u32		*word;

	i = line->i;
	if (isspace(*(u32 *)vector_get(line->line, (i != 0) ? i - 1 : i)))
		return 1;
	while (i > 0 && !isspace(*(u32 *)vector_get(line->line, i - 1)))
		i--;
	len = vector_size(line->line);
	word = (u32 *)vector_get(line->line, i);
	do
		vector_set(line->line, i++, (u32){tolower(*word++)});
	while (i < len && !isspace(*word));
	return term_display_line(line, 0);
}
