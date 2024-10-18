/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __pushtrap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 22:22:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline void	ptrap_pop(__t_vm *vm);
static inline void	ptrap_clean(void);

uint8_t	__pushtrap_status(void)
{
	static __t_vm	*vm = NULL;

	if (!vm)
		vm = __getvm();
	if (vm->ptrap == 1)
		return (PTRAP_ENABLE);
	return (PTRAP_DISABLE);
}

void	__pushtrap(const uint8_t op)
{
	static __t_vm	*vm = NULL;

	if (op & PTRAP_ENABLE && op & PTRAP_DISABLE)
		return ;
	if (!vm)
		vm = __getvm();
	if (op & PTRAP_ENABLE)
		vm->ptrap = 1;
	if (op & PTRAP_DISABLE)
		vm->ptrap = 0;
	if (op & PTRAP_POP)
		ptrap_pop(vm);
	if (op & PTRAP_CLEAN)
		ptrap_clean();
}

static inline void	ptrap_pop(__t_vm *vm)
{
	__t_obj	*obj;

	obj = vm->head;
	while (obj)
	{
		while (obj->traps)
			__popblk(obj->blk);
		obj = obj->next;
	}
}

static inline void	ptrap_clean(void)
{
	__clean();
}
