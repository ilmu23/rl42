/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __maprm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 04:15:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

uint8_t	__maprm(__t_hmap *hmap, const char *key)
{
	__t_hmap_pair	*pair;
	size_t		i;
	size_t		j;

	if (!hmap || !key || !hmap->count)
		return (0);
	if ((hmap->count * 100) / hmap->size < 10 && !__shrinkmap(hmap))
		return (0);
	j = 0;
	i = __gethash(key, hmap->size, j++);
	pair = hmap->items[i];
	while (pair)
	{
		if (pair != (void *)&__hmap_sentinel && __strequals(pair->key, key))
		{
			__mappop(pair);
			hmap->items[i] = (void *)&__hmap_sentinel;
			break ;
		}
		i = __gethash(key, hmap->size, j++);
		pair = hmap->items[i];
	}
	hmap->count--;
	return (1);
}
