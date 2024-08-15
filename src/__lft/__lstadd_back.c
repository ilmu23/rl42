/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstadd_back.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:29:29 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:24:41 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__lstadd_back(const t_list **list, t_list *node)
{
	t_list	*last;
	uint8_t	ptstatus;

	if (!list || !node)
		return ;
	ptstatus = __pushtrap_status();
	__pushtrap(PTRAP_DISABLE);
	__lstpush(node);
	last = __lstlast(*list);
	if (!last)
	{
		node->size = __push(__calloc(1, sizeof(*node->size)));
		__pushtrap(ptstatus);
		if (!node->size)
			return ;
		*list = node;
	}
	else
	{
		__pushtrap(ptstatus);
		node->size = last->size;
		node->prev = last;
		last->next = node;
	}
	(*node->size)++;
}
