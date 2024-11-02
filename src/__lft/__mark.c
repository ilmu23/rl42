/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __mark.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 11:08:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/02 06:17:04 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__unmark(const void *blk)
{
	static __t_vm	*vm = NULL;
	__t_obj		*obj;

	if (!vm)
		vm = __getvm();
	obj = __getobj(blk);
	if (obj && obj->marks)
	{
		obj->marks--;
		if (obj->traps)
			obj->traps--;
		if (!obj->marks && obj->blksize)
		{
			if (vm->free && vm->free != obj)
				vm->free->pfree = obj;
			if (vm->free != obj)
				obj->nfree = vm->free;
			vm->free = obj;
		}
	}
}

void	__mark(const void *blk)
{
	static __t_vm	*vm = NULL;
	__t_obj		*obj;

	if (!vm)
		vm = __getvm();
	obj = __getobj(blk);
	if (obj)
	{
		obj->marks++;
		if (vm->ptrap)
			obj->traps++;
		if (obj->nfree)
		{
			obj->nfree->pfree = obj->pfree;
			obj->nfree = NULL;
		}
		if (obj->pfree)
		{
			obj->pfree->nfree = obj->nfree;
			obj->pfree = NULL;
		}
		else if (obj->marks == 1)
			vm->free = obj->nfree;
	}
}
