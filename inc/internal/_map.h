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

typedef enum {
	STRING,
	INTEGER
}	map_key_type;

#define MAP_NOT_FOUND	((void *)1)

#define MAP_GROW_THRESHOLD	80

#define map(type, count, key_type, free)	(__map_new(sizeof(type), count, key_type, free))
map		__map_new(const size_t size, const size_t count, const map_key_type type, void (*free)(void *));

#define map_delete(map)	(__map_del(map))
void	__map_del(map map);

#define map_get(map, key)	(__map_get(map, (const uintptr_t)key))
void	*__map_get(cmap map, uintptr_t key);

#define map_set(map, key, value)	(__map_set(map, (const uintptr_t)key, (const void *)&value))
u8		__map_set(map map, uintptr_t key, const void *val);

#define map_erase(map, key)	(__map_ers(map, (const uintptr_t)key))
u8		__map_ers(map map, uintptr_t key);

#define map_size(map)	(__map_sze(map))
size_t	__map_sze(cmap map);

#define map_empty(map)	(__map_ety(map))
u8		__map_ety(cmap map);

#define	map_clear(map)	(__map_clr(map))
void	__map_clr(map map);
