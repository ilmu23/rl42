// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<set_mark.c>>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

rl42_fn(set_mark) {
	i64	len;
	i64	n;

	len = (i64)vector_size(line->line);
	n = (n_arg.set) ? get_numeric_arg(line) : (i64)line->i;
	if (n < 0)
		n = (len - -n >= 0) ? len - -n : len;
	else if (n > len)
		n = len;
	add_mark(user, n);
	return term_display_line(line, 0);
}
