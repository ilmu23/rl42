/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __sweep.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 11:14:57 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 14:48:25 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__sweep(void)
{
	static t_vm	*vm = NULL;
	t_obj		**obj;
	t_obj		*tmp;

	if (!vm)
		vm = __getvm();
	obj = &vm->head;
	while (*obj)
	{
		if (!(*obj)->marks)
		{
			tmp = *obj;
			*obj = tmp->next;
			vm->objcount--;
			__objmap_rm(__blkkey(tmp->blk));
			free((void *)tmp->blk);
			free(tmp);
		}
		else
			obj = &(*obj)->next;
	}
}
