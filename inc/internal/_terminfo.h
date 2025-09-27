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

/** @Unloads the currently loaded description
 *
 * Frees all resources used for storing the
 * currently loaded description. Does nothing
 * if no description is loaded
 */
void		ti_unload(void);

//TODO: ti_tgoto, ti_tparm, ti_tputs
