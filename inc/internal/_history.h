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

/** @brief Get nth history node
 *
 * @param n Node to get
 * @returns @c <b>rl42_hist_node *</b> Nth node,
 * First node if n < 1,
 * Last node if n > hist_size
 */
rl42_hist_node	*hist_get_nth_node(i64 n);

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

/** @brief Searches the history for an entry matching user input
 *
 * @param line Current input line
 * @param directiom Direction to search in
 * @param incremental Whether to perform the search incrementally or not
 * @returns @c <b>u8</b> Non-incremental: Non-zero on success, 0 on failure.
 * Incremental: Return value of the next function, 0 on failure
 */
u8				hist_search(rl42_line *line, const rl42_direction direction, const u8 incremental);

/** @brief Fetches the nth argument (word) from node and inserts it into line
 *
 * @param line Current input line
 * @param node Node to yank from
 * @param n Number of argument to yank
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8				hist_yank_arg(rl42_line *line, const rl42_hist_node *node, const i64 n);

/** @brief Removes a node from history
 *
 * @param node Node to remove
 */
void			hist_remove_node(rl42_hist_node *node);

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

/** @brief Saves new entries and frees all resources
 *
 * Saves any new entries to the history file and frees
 * all resources used for storing the history
 */
void			hist_clean(void);
