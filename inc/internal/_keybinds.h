// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_keybinds.h>>

#pragma once

#include "internal/_defs.h"

#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_function.h"

#include <errno.h>
#include <string.h>

#define EXPAND_INVALID_SEQ	((void *)1)

#define KEY_TREE_DEFAULT_MAP_SIZE	16

/** @brief Gets the current editing mode
 *
 * @returns @c <b>rl42_editing_mode</b> Current editing mode
 */
rl42_editing_mode	get_editing_mode(void);

/** @brief Sets the current editing mode
 *
 * @param mode Editing mode to switch to
 */
void				set_editing_mode(const rl42_editing_mode mode);

/** @brief Gets the root of the key tree for mode
 *
 * @param mode Desired editing mode
 * @returns @c <b>rl42_key_tree *</b> Key tree of the desired editing mode
 */
rl42_key_tree		*get_key_tree(const rl42_editing_mode mode);

/** @brief Creates a new key tree node
 *
 * @returns @c <b>rl42_key_tree *</b> The new node,
 * NULL if the creation failed
 */
rl42_key_tree		*new_key_tree_node(void);

/** @brief Frees a key tree node
 *
 * @param node Node to free
 */
void				free_key_tree_node(rl42_key_tree *node);

/** @brief Frees all keybind information
 *
 * Only to be used on exit as a cleanup function
 */
void				clean_key_trees(void);

/** @brief Allocates the roots of all key trees
 *
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8					init_key_trees(void);

/** @brief Expands a key sequence
 *
 * @param seq Sequence to expand
 * @returns @c <b>rl42_string *</b> Fully expanded seq,
 * EXPAND_INVALID_SEQ if seq was invalid,
 * NULL if an error occurred
 */
vector			expand_seq(const char *seq);
