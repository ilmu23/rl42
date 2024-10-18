/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __mappop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 02:22:56 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	__mappop(__t_hmap_pair *pair)
{
	if (!pair || pair == (void *)&__hmap_sentinel)
		return ;
	__popblks(3, pair, pair->key, pair->value);
}

void	__mappop_all(__t_hmap *hmap)
{
	size_t	i;

	if (!hmap)
		return ;
	i = 0;
	while (i < hmap->size)
		__mappop(hmap->items[i++]);
	__popblks(2, hmap, hmap->items);
}
