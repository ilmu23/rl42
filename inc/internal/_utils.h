// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_utils.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include <sys/types.h>

#include "internal/_data.h"

#define RL42STR_SUBSTR_NOT_FOUND	SIZE_MAX

#define max(x, y)	((x > y) ? x : y)
#define min(x, y)	((x < y) ? x : y)

#define in_range(x, min, max)	((x >= min && x <= max) ? 1 : 0)

typedef char	utf8_cbuf[5];

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
char		*rl42str_to_cstr(cvector s);

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
const char	*utf8_encode(const u32 ucp, utf8_cbuf buf);

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
u64			rl42str_hash(cvector s, const u64 salt, const size_t max);

/** @brief Appends s2 to the end of s1
 *
 * Allocates space for appending s1 to s2 and
 * returns the resulting string. NULL arguments
 * are treated as empty strings, hence cstr_join(NULL, NULL)
 * would return the string ""
 * @param s1 String to append to
 * @param s2 String to append
 * @returns @c <b>char *</b> s1 + s2,
 * NULL if allocation failed
 */
char		*cstr_join(const char *s1, const char *s2);

/** @brief Appends s2 to the end of s1
 *
 * Functions exactly like cstr_join, except instead of
 * allocating space for the result uses at most buf_size
 * bytes from buf to store the result. A buf_size of less than
 * strlen(s1) + strlen(s2) + 1 will result in truncation
 * @param s1 String to append to
 * @param s2 String to append
 * @param buf Buffer to store the result in
 * @param buf_size Size of buf
 * @returns @c <b>char *</b> s1 + s2,
 * NULL if buf was NULL or buf_size was 0
 */
char		*cstr_joinb(const char *s1, const char *s2, char *buf, const size_t buf_size);

/** @brief Makes a copy of a substring of s
 *
 * Allocates space for copying len characters
 * from s, starting at start
 * @param s String to copy from
 * @param start Index to start copying from
 * @param len Amount of characters to copy
 * @returns @c <b>char *</b> The resulting substring,
 * NULL if allocation failed
 */
char		*cstr_substr(const char *s, const size_t start, const size_t len);

/** @brief Makes a copy of a substring of s
 *
 * Functions exactly like cstr_substr, except instead of
 * allocating space for the result uses at most buf_size
 * bytes from buf to store the result
 * @param s String to copy from
 * @param start Index to start copying from
 * @param len Amount of characters to copy
 * @param buf Buffer to store the result in
 * @param buf_size Size of buf
 * @returns @c <b>char *</b> The resulting substring,
 * NULL if buf was NULL or buf_size was 0
 */
char		*cstr_substrb(const char *s, const size_t start, const size_t len, char *buf, const size_t buf_size);

/** @brief Splits s into sections separated by c
 *
 * @param s String to split
 * @param c Character to split on
 * @returns @c <b>vector</b> Vector containing all split substrings,
 * NULL on failure
 */
vector		cstr_split(const char *s, const char c);

/** @brief Finds the start of substr in s
 *
 * @param s String to look for substr in
 * @param substr String to look for
 * @returns @c <b>size_t</b> Start of substr in s,
 * RL42STR_SUBSTR_NOT_FOUND if substr is not found in s
 */
size_t		rl42str_find(cvector s, cvector substr);

/** @brief Prints a single character to stdout
 *
 * @returns @c <b>ssize_t</b> Bytes written,
 * -1 in case of an error
 */
ssize_t		__putchar(const char c);

/** @brief Calculates the correct position for the cursor
 *
 * @param line Information about the current line
 * @returns @c <b>size_t</b> Cursor offset from line->cursor.input_col
 */
size_t		calculate_cursor_offset(const rl42_line *line);

/** @brief Repeats the given functions
 *
 * Repeats the given functions according to the current
 * numeric argument. Assumes that an argument is set.
 * Sets the state flag STATE_REPEAT for the duration
 * of the call
 * @param line Current line to pass to positive/negative
 * @param positive Function to call if the numeric argument is positive
 * @param negative Function to call if the numeric argument is negative
 * @returns @c <b>u8</b> Return value of the called function
 */
u8			repeat(rl42_line *line, const rl42_fn positive, const rl42_fn negative);

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
