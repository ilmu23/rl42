// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_kb.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

#define KB_MOD_SHIFT	0x01U
#define KB_MOD_ALT		0x02U
#define KB_MOD_CTRL		0x04U
#define KB_MOD_SUPER	0x08U
#define KB_MOD_HYPER	0x10U
#define KB_MOD_META		0x20U

/** @brief Listens for a keyboard event
 *
 * @param timeout Listen timeout in milliseconds, -1 for no timeout
 * @returns @c <b>rl42_fn_info</b> Keyboard event information
 */
rl42_kb_event	*kb_listen(const i32 timeout);

/** @brief Frees all keyboard listener data
 */
void			clean_kb_listener(void);

/** @brief Initializes all keyboard listener data
 *
 * @returns @c <b>u8</b> 1 on success,
 * 0 on failure
 */
u8				init_kb_listener(void);
