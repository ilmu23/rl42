/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstadd_front.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:19:16 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/30 17:09:36 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__lstadd_front(const t_list **list, t_list *node)
{
	t_list	*first;
	uint8_t	ptstatus;

	if (!list || !node)
		return ;
	ptstatus = __pushtrap_status();
	__pushtrap(PTRAP_DISABLE);
	__lstpush(node);
	first = __lstfirst(*list);
	if (!first)
	{
		node->size = __push(__calloc(1, sizeof(*node->size)));
		__pushtrap(ptstatus);
		if (!node->size)
			return ;
	}
	else
	{
		__pushtrap(ptstatus);
		node->size = first->size;
		first->prev = node;
		node->next = first;
	}
	*list = node;
	(*node->size)++;
	node->prev = NULL;
}
