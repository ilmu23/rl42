/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstpushall.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:52:29 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:21:52 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__lstpushall(const t_list *list)
{
	while (list)
	{
		__lstpop(list);
		list = list->next;
	}
}
