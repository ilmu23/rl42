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

/** @brief Gets function info by key sequence
 *
 * @param expanded_seq Expanded key sequence the function is bound to
 * @param emode Editing mode to look for the binding in
 * @returns @c <b>rl42_fn_infor</b> Information about the function,
 * NULL if no function is bound to the specified sequence
 */
rl42_fn_info	*get_fn_info_keyseq(cvector expanded_seq, const rl42_editing_mode emode);

/** @brief Gets function info by name
 *
 * @param f Name of the function
 * @returns @c <b>rl42_fn_info</b> Information about the function,
 * NULL if no function with the specified name is found
 */
rl42_fn_info	*get_fn_info_name(const char *f);

/** @brief Gets function info
 *
 * @param f Function to fetch information about
 * @returns @c <b>rl42_fn_info</b> Information about the function,
 * NULL if the function hasn't been registered
 */
rl42_fn_info	*get_fn_info_fn(rl42_fn f);

/** @brief Creates a new macro function
 *
 * @param id Id of the macro
 * @param content Macro content
 * @param emode Editing mode to operate in
 * @returns @c <b>rl42_fn_info</b> Information about the new macro,
 * NULL if creating the macro failed
 */
rl42_fn_info	*create_macro(const size_t id, const char *content, const rl42_editing_mode emode);

/** @brief Edits the content of an existing macro
 *
 * @param macro Function info of the macro to edit
 * @param content New macro content
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8				edit_macro(rl42_fn_info *macro, const char *content);

/** @brief Gets a list of all registered functions
 *
 * @returns @c <b>cvector</b> List of all registered functions
 */
cvector			get_fn_list(void);

/** @brief Gets the next avaialable macro id
 *
 * @param emode Editing mode to operate in
 * @returns @c <b>size_t</b> Amount of currently defined macros
 */
size_t			get_next_macro_id(const rl42_editing_mode emode);

/** @brief Initializes macro caller code
 */
void			init_caller(void);

/** @brief Deletes all macro functions
 */
void			delete_macros(void);

/** @brief Deletes a specific macro
 *
 * @param id Id of the macro to delete
 * @param emode Editing mode to operate in
 */
void			delete_macro_name(const size_t id, const rl42_editing_mode emode);

/** @brief Frees all function information
 *
 * Only to be used on exit as a cleanup function
 */
void			clean_fns(void);
