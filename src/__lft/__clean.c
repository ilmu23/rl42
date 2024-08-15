/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __clean.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 11:29:27 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:16:54 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__clean(void)
{
	static t_vm	*vm = NULL;

	if (!vm)
		vm = __getvm();
	if (!vm->head)
		return ;
	__sweep();
	if (vm->objcount > _GC_START / 2)
		vm->maxobjs = vm->objcount * 2;
	else
		vm->maxobjs = _GC_START;
}