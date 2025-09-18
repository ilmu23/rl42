// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<list.c>>

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_list.h"
#include "internal/_vector.h"

#define _INDEX_NONE	SIZE_MAX

#define _ALLOCA_MAX_SIZE	524288 // 512 KiB

#define get_node(_node)	((__list_node__ *)((uintptr_t)_node - offsetof(__list_node__, node)))

typedef struct {
	void				(*free)(void *);
	struct __lst_node	node;
	size_t				index;
	size_t				next;
	size_t				prev;
	u8					delete;
}	__list_node__;

struct __lst {
	void	(*free)(void *);
	size_t	highest_index;
	size_t	element_size;
	size_t	elements;
	size_t	first;
	size_t	last;
	vector	data;
};

static void	_free_node(__list_node__ *node);
static void	__free(void **blk);

list	__lst_new(const size_t size, const size_t count, void (*_free)(void *)) {
	list	out;

	out = (size) ? malloc(sizeof(*out)) : NULL;
	if (out) {
		out->data = vector(__list_node__, count, (void (*)(void *))_free_node);
		if (!out->data) {
			free(out);
			return NULL;
		}
		out->element_size = size;
		out->highest_index = 0;
		out->elements = 0;
		out->first = 0;
		out->last = 0;
		out->free = (_free != free) ? _free : (void (*)(void *))__free;
	}
	return out;
}

void	__lst_del(list list) {
	if (list) {
		__lst_clr(list);
		vector_delete(list->data);
		free(list);
	}
}

u8	__lst_psh_b(list list, const void *val) {
	__list_node__	*last;
	__list_node__	new;
	u8				rv;

	new = (__list_node__){
		.free = list->free,
		.delete = 0,
		.next = _INDEX_NONE,
		.prev = _INDEX_NONE,
		.node.data = malloc(list->element_size)
	};
	if (!new.node.data)
		return 0;
	last = vector_get(list->data, list->last);
	memcpy(new.node.data, val, list->element_size);
	new.prev = (last != VECTOR_OUT_OF_BOUNDS) ? last->index : _INDEX_NONE;
	new.index = vector_size(list->data);
	rv = vector_push(list->data, new);
	if (rv) {
		list->highest_index = new.index;
		if (new.prev != _INDEX_NONE) {
			last = vector_get(list->data, new.prev);
			last->next = new.index;
		}
		list->last = new.index;
		list->elements++;
	} else
		free(new.node.data);
	return rv;
}

u8	__lst_psh_f(list list, const void *val) {
	__list_node__	*first;
	__list_node__	new;
	u8				rv;

	new = (__list_node__){
		.free = list->free,
		.delete = 0,
		.next = _INDEX_NONE,
		.prev = _INDEX_NONE,
		.node.data = malloc(list->element_size)
	};
	if (!new.node.data)
		return 0;
	first = vector_get(list->data, list->first);
	memcpy(new.node.data, val, list->element_size);
	new.next = (first != VECTOR_OUT_OF_BOUNDS) ? first->index : _INDEX_NONE;
	new.index = vector_size(list->data);
	rv = vector_push(list->data, new);
	if (rv) {
		list->highest_index = new.index;
		if (new.next != _INDEX_NONE) {
			first = vector_get(list->data, new.next);
			first->prev = new.index;
		}
		list->first = new.index;
		list->elements++;
	} else
		free(new.node.data);
	return rv;
}

void	__lst_pop_b(list list) {
	__list_node__	*last;
	__list_node__	*tmp;

	if (!list->elements)
		return ;
	last = vector_get(list->data, list->last);
	if (!--list->elements) {
		vector_pop(list->data);
		list->highest_index = 0;
		return ;
	}
	list->last = last->prev;
	tmp = vector_get(list->data, list->last);
	tmp->next = _INDEX_NONE;
	last->delete = 1;
	if (last->index == list->highest_index) do {
		vector_pop(list->data);
		tmp = vector_get(list->data, --list->highest_index);
	} while (tmp->delete);
}

void	__lst_pop_f(list list) {
	__list_node__	*first;
	__list_node__	*tmp;

	if (!list->elements)
		return ;
	first = vector_get(list->data, list->first);
	if (!--list->elements) {
		vector_pop(list->data);
		list->highest_index = 0;
		return ;
	}
	list->first = first->next;
	tmp = vector_get(list->data, list->first);
	tmp->prev = _INDEX_NONE;
	first->delete = 1;
	if (first->index == list->highest_index) do {
		vector_pop(list->data);
		tmp = vector_get(list->data, --list->highest_index);
	} while (tmp->delete);
}

list_node	__lst_nth(const list list, const size_t i) {
	__list_node__	*out;
	size_t			_i;

	if (i == 0)
		return &((__list_node__ *)vector_get(list->data, list->first))->node;
	if (i >= list->elements - 1)
		return &((__list_node__ *)vector_get(list->data, list->last))->node;
	_i = i;
	if (_i < list->elements / 2) {
		out = vector_get(list->data, list->first);
		while (_i--)
			out = vector_get(list->data, out->next);
	} else {
		out = vector_get(list->data, list->last);
		while (++_i < list->elements)
			out = vector_get(list->data, out->prev);
	}
	return &out->node;
}

list_node	__lst_nxt(const list list, const list_node node) {
	if (get_node(node)->next == _INDEX_NONE)
		return NULL;
	return &((__list_node__ *)vector_get(list->data, get_node(node)->next))->node;
}

list_node	__lst_prv(const list list, const list_node node) {
	if (get_node(node)->prev == _INDEX_NONE)
		return NULL;
	return &((__list_node__ *)vector_get(list->data, get_node(node)->prev))->node;
}

u8	__lst_ins_a(list list, const list_node node, const void *val) {
	__list_node__	*prev;
	__list_node__	new;
	u8				rv;

	new = (__list_node__){
		.free = list->free,
		.delete = 0,
		.node.data = malloc(list->element_size)
	};
	if (!new.node.data)
		return 0;
	prev = get_node(node);
	memcpy(new.node.data, val, list->element_size);
	new.prev = prev->index;
	new.next = prev->next;
	new.index = vector_size(list->data);
	rv = vector_push(list->data, new);
	if (rv) {
		list->highest_index = new.index;
		prev = vector_get(list->data, prev->index);
		prev->next = new.index;
		if (new.next != _INDEX_NONE)
			((__list_node__ *)vector_get(list->data, new.next))->prev = new.index;
		else
			list->last = new.index;
		list->elements++;
	} else
		free(new.node.data);
	return rv;
}

u8	__lst_ins_b(list list, const list_node node, const void *val) {
	__list_node__	*next;
	__list_node__	new;
	u8				rv;

	new = (__list_node__){
		.free = list->free,
		.delete = 0,
		.node.data = malloc(list->element_size)
	};
	if (!new.node.data)
		return 0;
	next = get_node(node);
	memcpy(new.node.data, val, list->element_size);
	new.next = next->index;
	new.prev = next->prev;
	new.index = vector_size(list->data);
	rv = vector_push(list->data, new);
	if (rv) {
		list->highest_index = new.index;
		next = vector_get(list->data, next->index);
		next->prev = new.index;
		if (new.prev != _INDEX_NONE)
			((__list_node__ *)vector_get(list->data, new.prev))->next = new.index;
		else
			list->first = new.index;
		list->elements++;
	} else
		free(new.node.data);
	return rv;
}

void	__lst_ers(list list, const list_node node) {
	__list_node__	*_node;

	_node = get_node(node);
	if (_node->next == _INDEX_NONE) {
		__lst_pop_b(list);
		return ;
	}
	if (_node->prev == _INDEX_NONE) {
		__lst_pop_f(list);
		return ;
	}
	((__list_node__ *)vector_get(list->data, _node->next))->prev = _node->prev;
	((__list_node__ *)vector_get(list->data, _node->prev))->next = _node->next;
	_node->delete = 1;
	list->elements--;
	if (_node->index == list->highest_index) do {
		vector_pop(list->data);
		_node = vector_get(list->data, --list->highest_index);
	} while (_node->delete);
}

size_t	__lst_sze(const list list) {
	return list->elements;
}

size_t	__lst_cap(const list list) {
	return vector_capacity(list->data);
}

u8	__lst_rsz(list list, const size_t size) {
	__list_node__	*tmp;
	__list_node__	*node;
	size_t			vec_size;
	size_t			tmp_size;
	size_t			i;

	vec_size = vector_size(list->data);
	if (size < vec_size) {
		tmp_size = vec_size * sizeof(*tmp);
		tmp = (tmp_size <= _ALLOCA_MAX_SIZE) ? alloca(tmp_size) : malloc(tmp_size);
		if (!tmp)
			return 0;
		for (i = 0, node = vector_get(list->data, list->first); node != VECTOR_OUT_OF_BOUNDS; node = vector_get(list->data, node->next))
			tmp[i++] = *node;
		for (i = 0; i < vec_size; i++) {
			if (i < size) {
				tmp[i].next = (i + 1 < size) ? i + 1 : _INDEX_NONE;
				tmp[i].prev = (i != 0) ? i - 1 : _INDEX_NONE;
			}
			vector_set(list->data, i, tmp[i]);
		}
		if (tmp_size > _ALLOCA_MAX_SIZE)
			free(tmp);
		list->highest_index = size;
		list->elements = size;
		list->last = size;
		list->first = 0;
	}
	return vector_resize(list->data, size);
}

u8	__lst_stf(list list) {
	return __lst_rsz(list, list->elements);
}

void	__lst_clr(list list) {
	vector_clear(list->data);
	list->highest_index = 0;
	list->elements = 0;
	list->first = 0;
	list->last = 0;
}

static void	_free_node(__list_node__ *node) {
	if (node->free)
		node->free(node->node.data);
	free(node->node.data);
}

static void	__free(void **blk) {
	free(*blk);
}
