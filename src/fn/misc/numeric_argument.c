// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<numeric_argument.c>>

#include <ctype.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_vector.h"
#include "internal/_display.h"

#include "internal/fn/misc.h"

rl42_numeric_arg	n_arg;

rl42_fn(numeric_argument) {
	size_t	size;
	size_t	i;
	i64		n;


	if (!n_arg.set) {
		line->prompt.sprompt = vector(u32, 21, NULL);
		if (!line->prompt.sprompt)
			return 0;
		n_arg.set = 1;
		n_arg.neg = 0;
		n_arg.val = 0;
	}
	for (i = 0, size = vector_size(line->keyseq); i < size; i++) {
		n = (i64)*(u32 *)vector_get(line->keyseq, i);
		if (isdigit((i32)n) || (i32)n == '-')
			break ;
	}
	if (i == size)
		return 0;
	if ((i32)n == '-') {
		n_arg.neg ^= 1;
		if (n_arg.neg)
			vector_insert(line->prompt.sprompt, 0, (u32){'-'});
		else
			vector_erase(line->prompt.sprompt, 0);
	} else if (n_arg.val != NUMERIC_ARG_MAX) {
		n_arg.val = n_arg.val * 10 + n - '0';
		if (n_arg.val > NUMERIC_ARG_MAX) {
			n_arg.val = NUMERIC_ARG_MAX;
			vector_clear(line->prompt.sprompt);
			if (n_arg.neg)
				vector_push(line->prompt.sprompt, (u32){'-'});
			for (n = NUMERIC_ARG_MAX; n; n /= 10)
				vector_insert(line->prompt.sprompt, n_arg.neg, (u32){n % 10 + '0'});
		} else
			vector_push(line->prompt.sprompt, (u32){n_arg.val % 10 + '0'});
	} else
		return 1;
	if (n_arg.val == 0 && !n_arg.neg) {
		vector_delete(line->prompt.sprompt);
		line->prompt.sprompt = NULL;
		n_arg.set = 0;
	}
	term_display_line(line, 0);
	return 1;
}
