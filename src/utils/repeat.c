// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<repeat.c>>

#define __RL42_INTERNAL
#include "internal/_data.h"
#include "internal/_rl42.h"

extern rl42_numeric_arg	n_arg;

u8	repeat(rl42_line *line, const rl42_fn positive, const rl42_fn negative) {
	rl42_fn	f;
	i64		n;
	u8		rv;

	n = n_arg.val;
	if (n == 0)
		return 1;
	f = (!n_arg.neg) ? positive : negative;
	if (!f)
		return 1;
	state_flags |= STATE_REPEAT;
	do {
		rv = f(line);
	} while (rv == 1 && --n);
	state_flags &= ~STATE_REPEAT;
	return rv;
}
