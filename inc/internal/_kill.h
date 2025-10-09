// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_kill.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

extern rl42_mark	kill_start;
extern rl42_mark	kill_end;

/** @brief Kills text in line
 *
 * Kills the text in line between kill_start and kill_end,
 * saving it to the kill ring if the state flag
 * STATE_KILL_DONT_UPDATE_RING is not set
 * @param line Line to kill text on
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		kill_region_internal(rl42_line *line);

/** @brief Copies text in line
 *
 * Copies the text in line between kill_start and kill_end,
 * saving it to the kill ring
 * @param line Line to copy text from
 * @return @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		kill_copy_region(rl42_line *line);

/** @brief Adds text to the kill ring
 *
 * @param text Text to add
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		kill_add_to_ring(cvector text);

/** @brief Gets the text on top of the kill ring
 *
 * @returns @c <b>cvector</b> The text on top of the kill ring,
 * NULL if the kill ring is empty
 */
cvector	kill_get_top_of_ring(void);

/** @brief Rotates the kill ring
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		kill_rotate_ring(void);

/** @brief Clears the kill ring
 */
void	kill_clear_ring(void);
