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

// Changes the behaviour of the rl42_bind function in the case that
// the key sequence the user is trying to bind is already bound
// WARN = Issue a warning to the user
// QUIET = Silently fail
// REMAP = Notify the user and remap the key sequence
// QREMAP = Remap the key sequence
typedef enum __bind_mode {
	WARN,
	QUIET,
	REMAP,
	QREMAP
}	rl42_bind_mode;

// Editing mode identifiers
// EMACS = emacs editing mode
// VI_CMD = vi command mode
// VI_INS = vi insert mode
// CURRENT = Currently active editing mode
typedef enum __editing_mode {
	EMACS = 0,
	VI_CMD = 1,
	VI_INS = 2,
	CURRENT
}	rl42_editing_mode;
