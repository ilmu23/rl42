/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __pop.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 19:39:17 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__popall(void)
{
	static __t_vm	*vm = NULL;

	if (!vm)
		vm = __getvm();
	__popn(vm->objcount);
}

void	__popn(size_t blks)
{
	static __t_vm	*vm = NULL;
	__t_obj		*obj;

	if (!vm)
		vm = __getvm();
	obj = vm->head;
	while (obj && blks--)
	{
		obj->marks = 0;
		obj->traps = 0;
		obj = obj->next;
	}
}

void	__popblk(const void *blk)
{
	static __t_vm	*vm = NULL;

	if (!blk)
		return ;
	if (!vm)
		vm = __getvm();
	__unmark(blk);
}

void	__popblks(size_t blks, ...)
{
	va_list	args;

	va_start(args, blks);
	while (blks)
	{
		__popblk(va_arg(args, void *));
		blks--;
	}
	va_end(args);
}
