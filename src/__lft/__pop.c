/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __pop.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 19:39:17 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:11:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__popall(void)
{
	static t_vm	*vm = NULL;

	if (!vm)
		vm = __getvm();
	__popn(vm->objcount);
}

void	__popn(size_t blks)
{
	static t_vm	*vm = NULL;
	t_obj		*obj;

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
	static t_vm	*vm = NULL;

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
