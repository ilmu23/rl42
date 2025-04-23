// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<map.c>>

#include "internal/_map.h"

static inline size_t	_findpair(const map *map, const u32 key);
static inline pair		*_newpair(void *item1, void *item2);
static inline u8		_grow(map *map);

map	*map_new(const size_t size) {
	map	*out;

	if (!size)
		return NULL;
	out = malloc(sizeof(*out));
	if (out) {
		*out = (map){
			.items = calloc(size, sizeof(*out->items)),
			.capacity = size,
			.itemcount = 0
		};
		if (!out->items) {
			free(out);
			out = NULL;
		}
	}
	return out;
}

u8	map_add(map *map, const u32 key, const void *val) {
	size_t	i;

	if ((map->itemcount * 100) / map->capacity > MAP_GROW_THRESHOLD && !_grow(map))
		return 0;
	for (i = key % map->capacity; map->items[i]; i = (i < map->capacity - 1) ? i + 1 : 0)
		;
	map->items[i] = _newpair((void *)(uintptr_t)key, (void *)val);
	if (!map->items[i])
		return 0;
	map->itemcount++;
	return 1;
}

u8	map_remove(map *map, const u32 key, void (*_free)(void *)) {
	size_t	i;

	i = _findpair(map, key);
	if (!map->items[i])
		return 0;
	if (_free)
		_free(map->items[i]->item2);
	free(map->items[i]);;
	map->items[i] = MAP_DELETED;
	return 1;
}

void	*map_find(const map * map, const u32 key) {
	size_t	i;

	i = _findpair(map, key);
	return (map->items[i]) ? map->items[i]->item2 : MAP_NOT_FOUND;
}

u8	map_delete(map *map, void (*_free)(void *)) {
	size_t	i;

	for (i = 0; i < map->capacity; i++)
		if (map->items[i] && map->items[i] != MAP_DELETED) {
			if (_free)
				_free(map->items[i]->item2);
			free(map->items[i]);
		}
	free(map->items);
	free(map);
	return 1;
}

static inline size_t	_findpair(const map *map, const u32 key) {
	size_t	i;

	for (i = key % map->capacity; map->items[i]; i = (i < map->capacity - 1) ? i + 1 : 0)
		if (map->items[i] != MAP_DELETED && (u32)(uintptr_t)map->items[i]->item1 == key)
			break ;
	return i;
}

static inline pair	*_newpair(void *item1, void *item2) {
	pair	*out;

	out = malloc(sizeof(*out));
	if (out) {
		*out = (pair){
			.item1 = item1,
			.item2 = item2
		};
	}
	return out;
}

static inline u8	_grow(map *map) {
	size_t	i;
	size_t	new_capacity;
	pair	**new_items;

	new_capacity = map->capacity * 2;
	new_items = calloc(new_capacity, sizeof(*new_items));
	if (!new_items)
		return 0;
	for (i = map->itemcount = 0; i < map->capacity; i++) {
		if (map->items[i] && map->items[i] != MAP_DELETED) {
			new_items[(u32)(uintptr_t)map->items[i]->item1 % new_capacity] = map->items[i];;
			map->itemcount++;
		}
	}
	free(map->items);
	map->capacity = new_capacity;
	map->items = new_items;
	return 1;
}
