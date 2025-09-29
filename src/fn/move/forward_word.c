// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<forward_word.c>>

#include <ctype.h>

#include "function.h"

#include "internal/_term.h"
#include "internal/_vector.h"

rl42_fn(forward_word) {
	size_t	len;

	len = vector_size(line->line);
	if (line->i == len)
		return 1;
	if (isspace(*(u32 *)vector_get(line->line, line->i))) do
		line->i++;
	while (line->i < len && isspace(*(u32 *)vector_get(line->line, line->i)));
	while (line->i < len && !isspace(*(u32 *)vector_get(line->line, line->i)))
		line->i++;
	return term_cursor_move_to_i(line);
}
