/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstpop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:10:05 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

__t_list	*__lstpop(const __t_list *node)
{
	if (!node)
		return (NULL);
	__popblk(node);
	__popblk(node->blk);
	return ((__t_list *)node);
}
