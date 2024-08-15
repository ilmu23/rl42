/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstrmnode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:47:17 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:21:14 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__lstrmnode(const t_list **list, const t_list *node)
{
	if (!list || !node)
		return ;
	if (node->next)
		node->next->prev = node->prev;
	if (node->prev)
		node->prev->next = node->next;
	else
		*list = node->next;
	(*node->size)--;
	__lstpop(node);
}
