// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<data.h>>

#pragma once

#include "defs.h"

// Changes the behaviour of rl42_map functions in the case that
// the key sequence the user is trying to bind is already bound
// WARN = Issue a warning to the user
// QUIET = Silently fail
// REMAP = Notify the user and remap the key sequence
// QREMAP = Remap the key sequence
typedef enum __keymapping_mode {
	WARN,
	QUIET,
	REMAP,
	QREMAP
}	rl42_keymapping_mode;

#ifndef __RL42_INTERNAL
// Contains all information about the current input line
// Modified by rl42 functions
typedef void *	rl42_line;

// rl42 input manipulation function prototype
// Takes the current input line as parameter
// Returns RL42_CONTINUE / RL42_DONE / RL42_ERROR
typedef u8	(*rl42_fn)(rl42_line);
#else
typedef u8	(*rl42_fn)(void *);
#endif
