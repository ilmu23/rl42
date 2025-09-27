// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_terminfo.h>>

#pragma once

#include "defs.h"

#include "internal/_terminfo_caps.h"

#include <stddef.h>
#include <sys/types.h>

// Invalid boolean capability name
#define TI_NOT_BOOL	-1
// Invalid numeric capability name
#define TI_NOT_NUM	-2
// Invalid string capability name
#define TI_NOT_STR	((const char *)-1)

// Absent or cancelled numeric capability
#define TI_ABS_NUM	0xFFFFFFFF
// Absent or cancelled string capability
#define TI_ABS_STR	NULL

// Boolean capability count
#define TI_BOOLEAN_CAPS	ti_OTXR
// Numeric capability count
#define TI_NUMERIC_CAPS	ti_OTkn
// string capability count
#define TI_STRING_CAPS	ti_slength

/** @brief Loads a terminal description
 *
 * Looks for a terminfo description corresponding to the
 * given terminal and loads it if one is found. If a description
 * is already loaded, unloads the old one only if a new one was found
 * @param term Name of the terminal
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			ti_load(const char *term);

/** @brief Gets the terminal name string
 *
 * @returns @c <b>const char *</b> Terminal name string
 */
const char	*ti_getname(void);

/** @brief Gets the value of a boolean capability
 *
 * @param name Name of the capability to get
 * @returns @c <b>i32</b> 1 if present,
 * 0 if the capability is absent or cancelled,
 * TI_NOT_BOOL if name was not a valid capability name
 */
i32			ti_getflag(const u8 name);

/** @brief Gets the value of a numeric capability
 *
 * @param name Name of the capability to get
 * @returns @c <b>i32</b> Value of the capability if present,
 * TI_ABS_NUM if the capability is absent or cancelled,
 * TI_NOT_NUM if name was not a valid capability name
 */
i32			ti_getnum(const u8 name);

/** @brief Gets the value of a string capability
 *
 * @param name Name of the capability to get
 * @returns @c <b>const char *</b> Value of the capability if present,
 * TI_ABS_STR if the capability is absent or cancelled,
 * TI_NOT_STR if name was not a valid capability name
 */
const char	*ti_getstr(const u16 name);

/** @brief Returns an escape sequence for moving the cursor to the desired location
 *
 * @apram seq Cursor move sequence (ti_cup)
 * @param row Desired row
 * @param col Desired column
 * @returns @c <b>const char *</b> Sequence for moving to the desired location,
 * NULL if an error occurred
 */
const char	*ti_tgoto(const char *seq, const i32 row, const i32 col);

/** @brief Instantiates an escape sequence with the given parameters
 *
 * @param seq Sequence to apply the parameters on
 * @param ... Parameters to apply
 * @returns @c <b>const char *</b> seq with the parameters applied,
 * NULL if an error occurred
 */
const char	*ti_tparm(const char *seq, ...);

/** @brief Outputs the string s, executing any delays found in it
 *
 * @param s Output string
 * @param affln Number of lines affected / 1 if not applicable
 * @param putc Function for outputting the characters
 * @returns @c <b>ssize_t</b> Amount of bytes written,
 * -1 if an error occurred
 */
ssize_t		ti_tputs(const char *s, const size_t affln, ssize_t (*putc)(const char));

/** @Unloads the currently loaded description
 *
 * Frees all resources used for storing the
 * currently loaded description. Does nothing
 * if no description is loaded
 */
void		ti_unload(void);
