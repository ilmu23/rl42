// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42.h>>

#pragma once

#include "defs.h"

#include "data.h"

#define RL42_VERSION 3.0.4

/** @brief Gets a line from the user with editing
 *
 * @param prompt Prompt to be displayed
 * @returns @c <b>char *</b> Line entered by the user
 * NULL if EOF is reached with an empty line
 */
char	*ft_readline(const char *prompt);

/** @brief Binds a key sequence to a function
 *
 * @param seq Sequence to bind
 * @param f Function to bind
 * @param bmode Binding mode
 * @param emode Editing mode to apply the bind to
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	rl42_bind(const char *seq, const char *f, const rl42_bind_mode bmode, const rl42_editing_mode emode);
