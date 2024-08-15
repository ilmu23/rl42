/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstfirst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 12:45:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:23:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

t_list	*__lstfirst(const t_list *list)
{
	if (!list)
		return (NULL);
	while (list->prev)
		list = list->prev;
	return ((t_list *)list);
}
