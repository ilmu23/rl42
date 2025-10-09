// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_rl42.h>>

#pragma once

#include "defs.h"

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

typedef u8	rl42_state;

#define STATE_INIT_IN_PROGRESS		0x01U
#define STATE_SAVE_HIST_POSITION	0x02U
#define STATE_REPEAT				0x04U
#define STATE_KILL_DONT_UPDATE_RING	0x08U
#define STATE_YANK_SET_KILL_POS		0x10U

#define NEED_REPEAT	(n_arg.set && ~state_flags & STATE_REPEAT)

extern rl42_numeric_arg	n_arg;

extern rl42_state	state_flags;

extern rl42_mark	user;

extern rl42_fn	prev_fn;

/* @brief Initializes all internal data structures for use
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	rl42_init(void);
