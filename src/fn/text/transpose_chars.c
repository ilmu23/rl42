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

#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

u8	_swap(void *x, void *y);

rl42_fn(transpose_chars) {
	size_t	len;
	size_t	i;

	if (NEED_REPEAT) {
		if (!repeat(line, transpose_chars, NULL))
			return 0;
	} else {
		if (line->i == 0)
			return 2;
		len = vector_size(line->line);
		i = (line->i < len) ? line->i : line->i - 1;
		vector_swap(line->line, i, i - 1, _swap);
		line->i = ++i;
	}
	return (~state_flags & STATE_REPEAT) ? term_display_line(line, 0) : 1;
}

u8	_swap(void *x, void *y) {
	*(u32 *)x ^= *(u32 *)y;
	*(u32 *)y ^= *(u32 *)x;
	*(u32 *)x ^= *(u32 *)y;
	return 1;
}
