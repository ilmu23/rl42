// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_term.h>>

#pragma once

#include "internal/_defs.h"

#include "internal/_utils.h"

#include <unistd.h>
#include <termios.h>

#define TERM_SETTINGS_DEFAULT	0
#define TERM_SETTINGS_RL42		1

typedef struct termios	term_settings;

/** @brief Initializes terminal related settings
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	term_init(void);

/** @brief Applies specified terminal settings
 *
 * @param settings TERM_SETTINGS_DEFAULT / TERM_SETTINGS_RL42
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	term_apply_settings(const u8 settings);
