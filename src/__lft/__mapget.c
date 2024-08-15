/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __mapget.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 04:06:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:20:51 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	*__mapget(t_hmap *hmap, const char *key)
{
	t_hmap_pair	*pair;
	size_t		i;
	size_t		j;

	if (!hmap || !key || !hmap->count)
		return (NULL);
	j = 0;
	i = __gethash(key, hmap->size, j++);
	pair = hmap->items[i];
	while (pair)
	{
		if (pair != (void *)&__hmap_sentinel && __strequals(pair->key, key))
			return (pair->value);
		i = __gethash(key, hmap->size, j++);
		pair = hmap->items[i];
	}
	return (NULL);
}
