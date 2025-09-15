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
#include "internal/_vector.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define in_range(x, min, max)	((x >= min && x <= max) ? 1 : 0)

/** @brief Converts a C string to a rl42 string
 *
 * @param s string to convert
 * @returns @c <b>rl42_string *</b> The converted string,
 * NULL if conversion failed
 */
vector		cstr_to_rl42str(const char *s);

/** @brief Converts a rl42 string to a C string
 *
 * @param s string to convert
 * @returns @c <b>char *</b> The converted string,
 * NULL if conversion failed
 */
char		*rl42str_to_cstr(const vector s);

/** @brief Counts the lenght of an utf-8 encoded C string
 *
 * @param s string to count
 * @returns @c <b>size_t</b> Length of the string
 */
size_t		strlen_utf8(const char *s);

/** @brief Gets the size of an utf-8 encoded character
 *
 * @param c Character to check
 * @returns @c <b>u8</b> Size of the character
 */
u8			charsize_utf8(const char c);

/** @brief Decodes an utf-8 encoded character into its unicode codepoint
 *
 * @param c Character to decode
 * @returns @c <b>u32</b> Unicode codepoint of the character
 */
u32			utf8_decode(const char *c);

/** @brief Encodes an unicode codepoint into a C string
 * @param ucp Codepoint to encode
 * @param buf Optional buffer for encoding
 * @returns @c <b>const char *</b> Pointer to encoded string
 * (buf or new allocation, depending on whether buf was NULL)
 * NULL if encoding failed
 */
const char	*utf8_encode(const u32 ucp, char buf[5]);

/** @brief Generates a hash from a cstr
 *
 * @param s String to hash
 * @param salt Salt used in the hashing
 * @param max Maximum value of the hash
 * @returns @c <b>u64</b> Hash of the string
 */
u64			cstr_hash(const char *s, const u64 salt, const size_t max);

/** @brief Generates a hash from a rl42_string
 *
 * @param s String to hash
 * @param salt Salt used in the hashing
 * @param max Maximum value of the hash
 * @returns @c <b>u64</b> Hash of the string
 */
u64			rl42str_hash(const vector s, const u64 salt, const size_t max);

/** @brief Displays an error message
 *
 * @param fmt Format string for the message
 * @param ... Variables required by the format
 * @returns @c <b>u8</b> 0
 */
u8			error(const char *fmt, ...);

/** @brief Displays a warning message
 *
 * @param fmt Format string for the message
 * @param ... Variables required by the format
 * @returns @c <b>u8</b> 0
 */
u8			warn(const char *fmt, ...);

/** @brief Displays an info message
 *
 * @param fmt Format string for the message
 * @param ... Variables required by the format
 * @returns @c <b>u8</b> 0
 */
u8			info(const char *fmt, ...);
