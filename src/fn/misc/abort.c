// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<abort.c>>

#define __RL42_INTERNAL
#include "rl42.h"
#include "function.h"

#include "internal/_rl42.h"
#include "internal/_term.h"

#ifndef __RL42_USE_EXTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

#define _BEL	"\a"

rl42_fn(rl42_abort) {
	state_flags |= STATE_ABORT;
	switch (rl42_get(RL42_BELL_STYLE).u64) {
		default:
		case RL42_BELL_NONE:
			return 1;
		case RL42_BELL_AUDIBLE:
			return (ti_tputs(_BEL, 1, term_putchar_unbuffered) != -1) ? 1 : 0;
		case RL42_BELL_VISIBLE:
			return (ti_tputs(ti_getstr(ti_flash), 1, term_putchar_unbuffered) != -1) ? 1 : 0;
	}
}
