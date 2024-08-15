/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __alloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 11:47:38 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:18:09 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline t_obj	*_findblk(const size_t n);

void	*__alloc(const size_t n)
{
	t_obj			*obj;

	if (!n)
		return (NULL);
	obj = _findblk(n);
	if (!obj)
		obj = __newobj(n);
	if (!obj)
		return (NULL);
	obj->asize = n;
	return ((void *)obj->blk);
}

static inline t_obj	*_findblk(const size_t n)
{
	static t_vm	*vm = NULL;
	t_obj		*obj;
	t_obj		*out;

	if (!vm)
		vm = __getvm();
	obj = vm->free;
	out = NULL;
	while (obj)
	{
		if (obj->blksize >= n && !obj->marks)
		{
			if (!out || out->blksize > obj->blksize)
				out = obj;
			if (out->blksize == n)
				break ;
		}
		obj = obj->nfree;
	}
	return (out);
}