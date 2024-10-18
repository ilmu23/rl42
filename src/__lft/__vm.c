/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __vm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:35:52 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

__t_vm	*__getvm(void)
{
	static __t_vm		vm;
	static uint8_t	init = 0;

	if (!init)
	{
		vm.objmap.bsize = _GC_MAPSIZE;
		vm.objmap.size = __nextprime(_GC_MAPSIZE);
		vm.objmap.objs = malloc(vm.objmap.size * sizeof(__t_list *));
		if (!vm.objmap.objs)
		{
			__putendl_fd(E_ALLOC, 2);
			exit(69);
		}
		memset(vm.objmap.objs, 0, vm.objmap.size * sizeof(__t_list *));
		vm.maxobjs = _GC_START;
		vm.objmap.count = 0;
		vm.objcount = 0;
		vm.head = NULL;
		vm.free = NULL;
		vm.ptrap = 0;
		init = 1;
	}
	return (&vm);
}
