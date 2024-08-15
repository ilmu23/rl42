/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstpop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:10:05 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:21:34 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

t_list	*__lstpop(const t_list *node)
{
	if (!node)
		return (NULL);
	__popblk(node);
	__popblk(node->blk);
	return ((t_list *)node);
}
