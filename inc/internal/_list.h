// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_list.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

typedef struct __lst_node *	list_node;

struct __lst_node {
	u8	*data;
};

#define		list(type, count, free)	(__lst_new(sizeof(type), count, free))
list   		__lst_new(const size_t size, const size_t count, void (*free)(void *));

#define		list_delete(list)	(__lst_del(list))
void   		__lst_del(list list);

#define		list_push_back(list, value)	(__lst_psh_b(list, (const void *)&value))
u8	   		__lst_psh_b(list list, const void *val);

#define		list_push_front(list, value)	(__lst_psh_f(list, (const void *)&value))
u8	   		__lst_psh_f(list list, const void *val);

#define		list_pop_back(list)	(__lst_pop_b(list))
void   		__lst_pop_b(list list);

#define		list_pop_front(list)	(__lst_pop_f(list))
void   		__lst_pop_f(list list);

#define		list_last(list)		(__lst_nth(list, SIZE_MAX))
#define		list_first(list)	(__lst_nth(list, 0))
#define		list_nth(list, i)	(__lst_nth(list, i))
list_node	__lst_nth(clist list, const size_t i);

#define		list_next(list, node)	(__lst_nxt(list, node))
list_node  	__lst_nxt(clist list, const list_node node);

#define		list_prev(list, node)	(__lst_prv(list, node))
list_node	__lst_prv(clist list, const list_node node);

#define		list_insert_after(list, ref, value)	(__lst_ins_a(list, ref, (const void *)&value))
u8	   		__lst_ins_a(list list, const list_node ref, const void *val);

#define		list_insert_before(list, ref, value)	(__lst_ins_b(list, ref, (const void *)&value))
u8	   		__lst_ins_b(list list, const list_node ref, const void *val);

#define		list_move_after(list, ref, node)	(__lst_mve_a(list, ref, node))
void		__lst_mve_a(list list, const list_node ref, const list_node node);

#define		list_move_before(list, ref, node)	(__lst_mve_b(list, ref, node))
void		__lst_mve_b(list list, const list_node ref, const list_node node);

#define		list_erase(list, node)	(__lst_ers(list, node))
void   		__lst_ers(list list, const list_node node);

#define		list_size(list)	(__lst_sze(list))
size_t 		__lst_sze(clist list);

#define		list_capacity(list)	(__lst_cap)
size_t 		__lst_cap(clist list);

#define		list_resize(list, size)	(__lst_rsz(list, size))
u8	   		__lst_rsz(list list, const size_t size);

#define		list_shrink_to_fit(list)	(__lst_stf(list))
u8	   		__lst_stf(list list);

#define		list_clear(list)	(__lst_clr(list))
void		__lst_clr(list list);
