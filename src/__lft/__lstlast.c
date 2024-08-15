/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstlast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:17:50 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:15:33 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

t_list	*__lstlast(const t_list *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return ((t_list *)list);
}
