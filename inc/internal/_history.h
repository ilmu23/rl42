// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_history.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

/** @brief Get neighboring history node
 *
 * @param node Node to get from
 * @param direction Direction to move to
 * @returns @c <b>rl42_hist_node *</b> Neighboring node,
 * node if trying to go beyond the start or end of history
 */
rl42_hist_node	*hist_get_next_node(const rl42_hist_node *node, const rl42_direction direction);

/** @brief Gets the first (newest) history entry
 *
 * @returns @c <b>rl42_hist_node *</b> First history entry
 */
rl42_hist_node	*hist_get_first_node(void);

/** @brief Gets the last (oldest) history entry
 *
 * @returns @c <b>rl42_hist_node *</b> Last history entry
 */
rl42_hist_node	*hist_get_last_node(void);

/** @brief Add line to history
 *
 * @param line Line to add
 * @returns @c <b>u8</b> 1 if adding the line succeeded,
 * 0 if not
 */
u8				hist_add_line(const char *line);

/** @brief Loads history from a file
 *
 * @param fname Name of the file to load
 * @returns @c <b>u8</b> 1 if loading the history succeeded,
 * 0 if not
 */
u8				hist_load(const char *fname);
