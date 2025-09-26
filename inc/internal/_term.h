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

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

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

/** @brief Sets cursor position
 *
 * Treats row and col as absolute values, will not work as expected
 * if either of them is out of bounds
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	term_cursor_set_pos(const i16 row, const i16 col);

/** @brief Moves corsor to given location
 *
 * Unlike term_cursor_set_pos, this function calculates the correct position
 * based on the given row and column instead of treating them as absolute values
 * @param cursor Current cursor context. Moves to the position set in cursor->row/col
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8	term_cursor_move_to(rl42_cursor *cursor);
