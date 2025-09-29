// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_function.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

#define FUNCTION_COUNT	64

/** @brief Gets function info by name
 *
 * @param f Name of the function
 * @returns @c <b>rl42_fn_info</b> Information about the function,
 * NULL if no function with the specified nname is found
 */
rl42_fn_info	*get_fn_info_name(const char *f);

/** @brief Gets function info
 *
 * @param f Function to fetch information about
 * @returns @c <b>rl42_fn_info</b> Information about the function,
 * NULL if the function hasn't been registered
 */
rl42_fn_info	*get_fn_info_fn(rl42_fn f);

/** @brief Frees all function information
 *
 * Only to be used on exit as a cleanup function
 */
void			clean_fns(void);
