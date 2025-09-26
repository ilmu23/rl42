// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_term.h>>

#pragma once

#include "defs.h"

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

/** @brief Gets the current cursor position
 *
 * @param row Where to store the current row
 * @param col Whete to store the current column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	term_cursor_get_pos(i16 *row, i16 *col);

/** @brief Moves corsor to given location
 *
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	term_cursor_move_to(const i16 row, const i16 col);
