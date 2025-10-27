// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42.h>>

#pragma once

#include "defs.h"

#include "data.h"

#define RL42_VERSION "3.7.8-misc"

/** @brief Gets a line from the user with editing
 *
 * @param prompt Prompt to be displayed
 * @returns @c <b>char *</b> Line entered by the user
 * NULL if EOF is reached with an empty line
 */
char				*ft_readline(const char *prompt);

/** @brief Binds a key sequence to a function
 *
 * @param seq Sequence to bind
 * @param f Function to bind
 * @param bmode Binding mode
 * @param emode Editing mode to apply the bind to
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8					rl42_bind(const char *seq, const char *f, const rl42_bind_mode bmode, const rl42_editing_mode emode);

/** @brief Unbinds a key sequence
 *
 * @param seq Sequence to unbind
 * @param emode Editing mode in which to look for the bind in
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8					rl42_unbind(const char *seq, const rl42_editing_mode emode);

/** @brief Sets the value of a setting
 *
 * @param setting Setting to set
 * @param value Value to set the setting to
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 if setting was not a valid setting
 */
u8					rl42_set(const rl42_setting setting, const rl42_setting_val value);

/** @brief Gets the value of a setting
 *
 * @param setting Setting to get
 * @returns @c <b>rl42_setting_val</b> Value of the setting,
 * .u64 = 0 if setting was not a valid setting
 */
rl42_setting_val	rl42_get(const rl42_setting setting);
