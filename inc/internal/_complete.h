// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_complete.h>>

#pragma once

#include "internal/_data.h"

/** @brief Returns the common part of the completions, if one is found
 *
 * @param completions Completions to analyze
 * @returns @c <b>cvector</b> Common part of the completions,
 * completions if no common part was found or if there were no completions,
 * NULL if completions was NULL
 */
cvector	cmp_get_common(cvector completions);

/** @brief Displays all completions
 *
 * @param line Line being completed
 * @param completions Completions to display
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		cmp_display(rl42_line *line, cvector completions);

/** @brief Inserts a completion into line
 *
 * @param line Line to insert completion into
 * @param completion Completion to insert
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		cmp_insert(rl42_line *line, const char *completion);
