// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<complete.h>>

#pragma once

#include "internal/_data.h"

#define rl42_completion_fn(name)	cdarray	name([[maybe_unused]] const char *pattern, [[maybe_unused]] cdarray context, [[maybe_unused]] const size_t start, [[maybe_unused]] size_t *pattern_len)

extern u8	rl42_completion_raw_context;

/** @brief Sets the function used for performing completions
 *
 * Sets the function used by the 'complete' command to get
 * completions. If f is NULL, restores the default
 * completion function and turns off raw context mode
 * if it was turned on
 * @param f Function to use for completions
 */
void	rl42_set_completion_fn(rl42_completion_fn f);
