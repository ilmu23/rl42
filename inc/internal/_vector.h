// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_vector.h>>

#pragma once

#include "internal/_defs.h"

#include "internal/_data.h"

#include <stdlib.h>
#include <string.h>

#define VECTOR_NOT_FOUND		SIZE_MAX
#define VECTOR_OUT_OF_BOUNDS	UINTPTR_MAX

/** @brief Creates a new vector capable of storing at least size elements
 *
 * @param size Amount of elements to reserve memory for
 * @param smode Default sorting for the vector
 * @returns @c <b>vector</b> The newly created map,
 * NULL if creation failed
 */
vector		*vector_new(const size_t size, const vector_sort_mode smode);

/** @brief Adds a new element to the vector
 *
 * @param vec Vector to operate on
 * @param data Element to add
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			vector_add(vector *vec, const uintptr_t data);

/** @brief Removes an element from a vector
 *
 * @param vec Vector to operate on
 * @param i Index of the element to remove
 * @param _free Function for freeing the element
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 if i is out of bounds,
 */
u8			vector_remove(vector *vec, const size_t i, void (*_free)(void *));

/** @brief Looks for an element in a vector
 *
 * @param vec Vector to operate on
 * @param data Element to look for
 * @param cmp Function to use for comparisons
 * @returns @c <b>u8</b> Index of the element,
 * VECTOR_NOT_FOUND if no match was found
 */
size_t		vector_find(const vector *vec, const uintptr_t data, u8 (*cmp)(const uintptr_t *, const uintptr_t *));

/** @brief Gets an element from a vector
 *
 * @param vec Vector to operate on
 * @param i index of the element to get
 * @returns @c <b>uintptr_t</b> The element,
 * VECTOR_OUT_OF_BOUNDS if i is out of bounds
 */
uintptr_t	vector_get(const vector *vec, const size_t i);

/** @brief Frees all resources associated with a vector
 *
 * @param vec Vector to free
 * @param _free Function for freeing the elements stored in the vector
 * @returns @c <b>u8</b> Non-zero on success,
 * 0 on failure
 */
u8			vector_delete(vector *vec, void (*_free)(void *));
