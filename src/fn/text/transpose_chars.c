// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<transpose_chars.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_vector.h"
#include "internal/_display.h"

u8	_swap(void *x, void*y);

rl42_fn(transpose_chars) {
	size_t	len;
	size_t	i;

	if (line->i == 0)
		return 1;
	len = vector_size(line->line);
	i = (line->i < len) ? line->i : line->i - 1;
	vector_swap(line->line, i, i - 1, _swap);
	line->i = ++i;
	return term_display_line(line, 0);
}

u8	_swap(void *x, void*y) {
	*(u32 *)x ^= *(u32 *)y;
	*(u32 *)y ^= *(u32 *)x;
	*(u32 *)x ^= *(u32 *)y;
	return 1;
}
