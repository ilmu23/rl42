// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_utils.h>>

#pragma once

#include "internal/_defs.h"

#include "internal/_data.h"

#include <stdlib.h>
#include <string.h>

/** @brief Converts a C string to a rl42 string
 *
 * @param s string to convert
 * @returns @c <b>rl42_string *</b> The converted string,
 * NULL if conversion failed
 */
rl42_string	*cstr_to_rl42str(const char *s);

/** @brief Converts a rl42 string to a C string
 *
 * @param s string to convert
 * @returns @c <b>char *</b> The converted string,
 * NULL if conversion failed
 */
char		*rl42str_to_cstr(const rl42_string *s);

/** @brief Counts the lenght of an utf-8 encoded C string
 *
 * @param s string to count
 * @returns @c <b>size_t</b> Length of the string
 */
size_t		strlen_utf8(const char *s);
