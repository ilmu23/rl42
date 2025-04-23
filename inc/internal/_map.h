// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_map.h>>

#pragma once

#include "internal/_defs.h"

#include "internal/_data.h"

#include <stdlib.h>

#define MAP_DELETED		((void *)1)
#define MAP_NOT_FOUND	((void *)2)

#define MAP_GROW_THRESHOLD	70

/** @brief Creates a new map capable of storing at least size elements
 *
 * @param size Amount of elements to reserve memory for
 * @returns @c <b>map *</b> The newly created map,
 * NULL if creation failed
 */
map		*map_new(const size_t size);

/** @brief Adds a new element to the map
 *
 * @param map Map to operate on
 * @param key Key for accessing the element
 * @param val The element itself
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		map_add(map *map, const u32 key, const void *val);

/** @brief Removes an element from a map
 *
 * @param map Map to operate on
 * @param key Key for accessing the element
 * @param _free Function for freeing the element
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 if no element was found
 */
u8		map_remove(map *map, const u32 key, void (*_free)(void *));

/** @brief Looks for the element identified by key
 *
 * @param map Map to operate on
 * @param key Key for accessing the element
 * @returns @c <b>void *</b> The element referred to by key,
 * MAP_NOT_FOUND if no element corresponding to key was found
 */
void	*map_find(const map *map, const u32 key);

/** @brief Frees all resources associated with a map
 *
 * @param map Map to free
 * @param _free Function for freeing the elements stored in the map
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8		map_delete(map *map, void (*_free)(void *));
