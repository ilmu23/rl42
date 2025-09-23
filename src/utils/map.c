// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<map.c>>

#include <stdlib.h>
#include <string.h>

#include "internal/_map.h"
#include "internal/_utils.h"

#define _DELETED	((void *)2)

#define _HASH_SALT	347

typedef struct {
	uintptr_t	key;
	u8			val[];
}	pair;

struct __map {
	map_key_type	key_type;
	size_t			capacity;
	size_t			elements;
	size_t			element_size;
	void			(*free)(void *);
	pair			**data;
};

static inline size_t	_find_pair(const map map, const uintptr_t key);
static inline u8		_grow(map map);

static void	__free(void **blk);

map	__map_new(const size_t size, const size_t count, const map_key_type type, void (*_free)(void *)) {
	map	out;

	out = (size) ? malloc(sizeof(*out)) : NULL;
	if (out) {
		out->data = calloc(count, sizeof(*out->data));
		if (!out->data) {
			free(out);
			return NULL;
		}
		out->element_size = size;
		out->capacity = count;
		out->key_type = type;
		out->elements = 0;
		out->free = (_free != free) ? _free : (void (*)(void *))__free;
	}
	return out;
}

void	__map_del(map map) {
	if (map) {
		__map_clr(map);
		free(map->data);
		free(map);
	}
}

void	*__map_get(const map map, uintptr_t key) {
	size_t	i;
	
	i = _find_pair(map, (map->key_type == INTEGER) ? key : cstr_hash((const char *)key, _HASH_SALT, UINTPTR_MAX));
	return (map->data[i] && map->data[i] != _DELETED) ? map->data[i]->val : MAP_NOT_FOUND;
}

u8	__map_set(map map, uintptr_t key, const void *val) {
	size_t	i;

	if ((map->elements * 100) / map->capacity > MAP_GROW_THRESHOLD && !_grow(map))
		return 0;
	if (map->key_type == STRING)
		key = cstr_hash((const char *)key, _HASH_SALT, UINTPTR_MAX);
	i = key % map->capacity;
	while (map->data[i] && map->data[i] != _DELETED)
		i = (i < map->capacity - 1) ? i + 1 : 0;
	if (!map->data[i]) {
		map->data[i] = malloc(sizeof(*map->data[i]) + map->element_size);
		if (!map->data[i])
			return 0;
	}
	map->data[i]->key = key;
	memcpy(map->data[i]->val, val, map->element_size);
	map->elements++;
	return 1;
}

u8	__map_ers(map map, uintptr_t key) {
	size_t	i;

	i = _find_pair(map, (map->key_type == INTEGER) ? key : cstr_hash((const char *)key, _HASH_SALT, UINTPTR_MAX));
	if (!map->data[i] || map->data[i] == _DELETED)
		return 0;
	if (map->free)
		map->free(map->data[i]->val);
	free(map->data[i]);
	map->data[i] = _DELETED;
	map->elements--;
	return 1;
}

size_t	__map_sze(const map map) {
	return map->elements;
}

u8	__map_ety(const map map) {
	return (map->elements == 0) ? 1 : 0;
}

void	__map_clr(map map) {
	size_t	i;

	for (i = 0; i < map->capacity; i++) {
		if (map->data[i] && map->data[i] != _DELETED) {
			if (map->free)
				map->free(map->data[i]->val);
			free(map->data[i]);
			map->data[i] = _DELETED;
			if (!--map->elements)
				break ;
		}
	}
}

static inline size_t	_find_pair(const map map, const uintptr_t key) {
	size_t	i;

	for (i = key % map->capacity; map->data[i]; i = (i < map->capacity - 1) ? i + 1 : 0)
		if (map->data[i] != _DELETED && map->data[i]->key == key)
			break ;
	return i;
}

static inline u8		_grow(map map) {
	size_t	i;
	size_t	j;
	size_t	new_capacity;
	pair	**new_data;

	new_capacity = map->capacity * 2;
	new_data = calloc(new_capacity, sizeof(*new_data));
	if (!new_data)
		return 0;
	for (i = 0; i < map->capacity; i++) {
		if (map->data[i] && map->data[i] != _DELETED) {
			j = map->data[i]->key % new_capacity;
			while (new_data[j])
				j = (j < new_capacity - 1) ? j + 1 : 0;
			new_data[j] = map->data[i];
		}
	}
	free(map->data);
	map->capacity = new_capacity;
	map->data = new_data;
	return 1;
}

static void	__free(void **blk) {
	free(*blk);
}
