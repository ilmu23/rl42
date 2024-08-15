/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstpopall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:50:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:22:36 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__lstpopall(const t_list *list)
{
	while (list)
	{
		__lstpop(list);
		list = list->next;
	}
}
