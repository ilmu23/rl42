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

// SGR option bit masks
#define SGR_STANDOUT	0x001U
#define SGR_UNDERLINE	0x002U
#define SGR_REVERSE		0x004U
#define SGR_BLINK		0x008U
#define SGR_DIM			0x010U
#define SGR_BOLD		0x020U
#define SGR_INVIS		0x040U
#define SGR_PROTECT		0x080U
#define SGR_ALTCHARSET	0x100U

#define term_cursor_move_to_i(line)	(term_cursor_move_to(line, (line)->root.row, (line)->root.col + (line)->i))

typedef struct termios	term_settings;

typedef _BitInt(9)	sgr_opts;

/** @brief Initializes terminal related settings
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_init(void);

/** @brief Applies specified terminal settings
 *
 * @param settings TERM_SETTINGS_DEFAULT / TERM_SETTINGS_RL42
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_apply_settings(const u8 settings);

/** @brief Gets the escape sequence corresponding to name
 *
 * @param name Name of the escape sequnce to get
 * @returns @c <b>const char *</b> Escape sequence for name,
 * NULL if not found
 */
const char	*term_get_seq(const u16 name);

/** @brief Matches a received key escape sequence
 *
 * @param seq Sequence to match
 * @returns @c <b>u16</b> Name of the sequence,
 * 0 if no match was found
 */
u16			term_match_key_seq(const char *seq);

/** @brief Sets the terminals foreground color
 *
 * @param color Index of the color to set
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_set_fg_color(const u8 color);

/** @brief Sets the terminals background color
 *
 * @param color Index of the color to set
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_set_bg_color(const u8 color);

/** @brief Sets graphics mode options
 *
 * @param opts Bit field of options to enable
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_set_sgr(const sgr_opts opts);

/** @brief Hides the cursor
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_hide_cursor(void);

/** @brief Shows the cursor
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_show_cursor(void);

/** @brief Gets the current cursor position
 *
 * @param row Where to store the current row
 * @param col Whete to store the current column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_cursor_get_pos(i16 *row, i16 *col);

/** @brief Sets cursor position
 *
 * Treats row and col as absolute values, will not work as expected
 * if either of them is out of bounds
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_cursor_set_pos(const i16 row, const i16 col);

/** @brief Moves corsor to given location
 *
 * Unlike term_cursor_set_pos, this function calculates the correct position
 * based on the given row and column instead of treating them as absolute values,
 * updating the prompt and input roots as needed
 * @param line Current input line
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			term_cursor_move_to(rl42_line *line, i16 row, i16 col);
