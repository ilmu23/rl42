// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_complete.h>>

#pragma once

#ifndef __RL42_INTERNAL
#define __RL42_INTERNAL
#endif

#include "internal/_data.h"

/** @brief Returns the common part of the completions, if one is found
 *
 * @param completions Completions to analyze
 * @param pattern_len Length of the completed pattern
 * @returns @c <b>cdarray</b> Common part of the completions,
 * completions if no common part was found or if there were no completions,
 * NULL if completions was NULL
 */
cdarray	cmp_get_common(cdarray completions, const size_t pattern_len);

/** @brief Displays all completions
 *
 * @param line Line being completed
 * @param completions Completions to display
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		cmp_display(rl42_line *line, cdarray completions);

/** @brief Inserts a completion into line
 *
 * @param line Line to insert completion into
 * @param completion Completion to insert
 * @param stat_char Stat character to append to completion, if applicable
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		cmp_insert(rl42_line *line, const rl42_completion *completion, const u32 stat_char);
