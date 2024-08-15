/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __objmap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:34:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 14:49:42 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline t_objpair	*_getpair(t_list *lst, const char *key);

t_obj	*__objmap_get(const char *key)
{
	static t_vm	*vm = NULL;
	t_objpair	*pair;
	size_t		i;

	if (!vm)
		vm = __getvm();
	i = __gethash(key, _GC_MAPSIZE, 5);
	pair = _getpair(vm->objmap.objs[i], key);
	if (pair)
		return ((t_obj *)pair->obj);
	return (NULL);
}

void	__objmap_rm(const char *key)
{
	static t_vm	*vm = NULL;
	t_objpair	*pair;
	t_list		*tmp;
	size_t		i;

	if (!vm)
		vm = __getvm();
	i = __gethash(key, _GC_MAPSIZE, 5);
	pair = _getpair(vm->objmap.objs[i], key);
	if (!pair)
		return ;
	tmp = vm->objmap.objs[i];
	while (tmp->blk != pair)
		tmp = tmp->next;
	if (tmp->next)
		tmp->next->prev = tmp->prev;
	if (tmp->prev)
		tmp->prev->next = tmp->next;
	else
		vm->objmap.objs[i] = tmp->next;
	free((void *)pair->key);
	free((void *)key);
	free(pair);
	free(tmp);
}

void	__objmap_add(const char *key, const t_obj *obj)
{
	static t_vm	*vm = NULL;
	t_objpair	*pair;
	size_t		i;

	if (!vm)
		vm = __getvm();
	pair = __newpair(key, obj);
	i = __gethash(key, _GC_MAPSIZE, 5);
	__objmap_addpair(&vm->objmap.objs[i], pair);
}

void	__objmap_addpair(t_list **lst, t_objpair *pair)
{
	t_list	*node;
	t_list	*last;

	node = malloc(sizeof(*node));
	if (!node)
	{
		__putendl_fd(E_ALLOC, 2);
		__exit(69);
	}
	last = __lstlast(*lst);
	*node = (t_list){.blk = pair, .next = NULL, .prev = last, .size = NULL};
	if (node->prev)
		node->prev->next = node;
	else
		*lst = node;
}

static inline t_objpair	*_getpair(t_list *lst, const char *key)
{
	t_objpair	*out;

	while (lst)
	{
		out = (t_objpair *)lst->blk;
		if (__strequals(out->key, key))
			return (out);
		lst = lst->next;
	}
	return (NULL);
}
