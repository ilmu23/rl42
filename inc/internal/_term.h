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
#include "internal/_utils.h"
#include "internal/_vector.h"

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

#define term_cursor_move_to_i(line)	(term_cursor_move_to(line, (line)->root->row, (line)->root->col + calculate_cursor_offset(line) + ((line->prompt.sprompt) ? vector_size(line->prompt.sprompt) + 1 : 0)))

typedef struct termios	term_settings;

#if __STDC_VERSION__ >= 202311L
typedef unsigned _BitInt(9)	sgr_opts;
#else
typedef u16	sgr_opts;
#endif

/** @brief Initializes terminal related settings
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_init(void);

/** @brief Applies specified terminal settings
 *
 * @param settings TERM_SETTINGS_DEFAULT / TERM_SETTINGS_RL42
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_apply_settings(const u8 settings);

/** @brief Gets the escape sequence corresponding to name
 *
 * @param name Name of the escape sequnce to get
 * @returns @c <b>const char *</b> Escape sequence for name,
 * NULL if not found
 */
const char	*term_get_seq(const u16 name);

/** @brief Gets the current highlight escape sequence
 *
 * @returns @c <b>const char *</b> Current highlight escape sequence
 */
const char	*term_get_hl_seq(void);

/** @brief Matches a received key escape sequence
 *
 * @param seq Sequence to match
 * @returns @c <b>u16</b> Name of the sequence,
 * 0 if no match was found
 */
u16						term_match_key_seq(const char *seq);

/** @brief Sets the terminals foreground color
 *
 * @param color Index of the color to set
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_set_fg_color(const u8 color);

/** @brief Sets the terminals background color
 *
 * @param color Index of the color to set
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_set_bg_color(const u8 color);

/** @brief Sets graphics mode options
 *
 * @param opts Bit field of options to enable
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_set_sgr(const sgr_opts opts);

/** @brief Hides the cursor
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_hide_cursor(void);

/** @brief Shows the cursor
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_show_cursor(void);

/** @brief Calculates amount of rows needed to display a line
 *
 * Calculates the amount of rows needed to display a line,
 * optionally scrolling the display if not enough rows
 * are available. The result is stored in line->rows
 * @param line Line to use for the calculation
 * @param scroll Whether to scroll if there are not enough rows available
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_calculate_required_rows(rl42_line *line, const u8 scroll);

/** @brief Scroll the display
 *
 * @param up Amount of rows to scroll up by
 * @param down Amount of rows to scroll down by
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_scroll_display(size_t up, size_t down);

/** @brief Returns a new anchor position
 *
 * Creates a new anchor position at the current cursor position
 * @returns @c <b>rl42_cursor_pos *</b> The anchor position,
 * NULL on failure
 */
const rl42_cursor_pos	*term_cursor_new_anchor(void);

/** @brief Deletes an anchor position
 *
 * @param anchor Anchor position to delete
 */
void					term_cursor_delete_anchor(const rl42_cursor_pos *anchor);

/** @brief Gets the current cursor position
 *
 * @param row Where to store the current row
 * @param col Whete to store the current column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_cursor_get_pos(i16 *row, i16 *col);

/** @brief Sets cursor position
 *
 * Treats row and col as absolute values, will not work as expected
 * if either of them is out of bounds
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_cursor_set_pos(const i16 row, const i16 col);

/** @brief Moves corsor to given location
 *
 * Unlike term_cursor_set_pos, this function calculates the correct position
 * based on the given row and column instead of treating them as absolute values,
 * updating all anchors as needed
 * @param line Current input line
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_cursor_move_to(rl42_line *line, i16 row, i16 col);

/** @brief Moves cursor to the start of the next line
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8						term_cursor_next_line(void);
