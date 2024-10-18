/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __obj.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:47:53 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

__t_objpair	*__newpair(const char *key, const __t_obj *obj)
{
	__t_objpair	*out;

	out = __gc_calloc(1, sizeof(*out));
	if (!out)
	{
		__putendl_fd(E_ALLOC, 2);
		__exit(69);
	}
	*out = (__t_objpair){.key = key, .obj = obj};
	return (out);
}

__t_obj	*__newobj(const size_t n)
{
	static __t_vm	*vm = NULL;
	__t_obj		*obj;

	if (!vm)
		vm = __getvm();
	if (vm->objcount == vm->maxobjs)
		__clean();
	obj = malloc(sizeof(*obj));
	if (!obj)
		return (NULL);
	*obj = (__t_obj){.marks = 0, .traps = 0, .next = vm->head, .blksize = n,};
	obj->blk = malloc(n);
	if (!obj->blk)
		return (NULL);
	vm->head = obj;
	if (vm->free)
		vm->free->pfree = obj;
	obj->nfree = vm->free;
	__objmap_add(__blkkey(obj->blk), obj);
	vm->objcount++;
	return (obj);
}

__t_obj	*__getobj(const void *blk)
{
	const char	*key;
	__t_obj		*out;

	if (!blk)
		return (NULL);
	key = __blkkey(blk);
	out = __objmap_get(key);
	free((void *)key);
	return (out);
}

char	*__blkkey(const void *blk)
{
	size_t		i;
	uintptr_t	addr;
	char		*out;

	addr = (uintptr_t)blk;
	i = __uintlen_base(addr, HEX);
	out = malloc((i + 1) * sizeof(*out));
	if (!out)
	{
		__putendl_fd(E_ALLOC, 2);
		__exit(69);
	}
	out[i--] = '\0';
	while (addr > 15)
	{
		out[i--] = _HEXARR[addr % 16];
		addr /= 16;
	}
	out[i] = _HEXARR[addr];
	return (out);
}
