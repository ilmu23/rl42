/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __mapnew.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 02:18:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

__t_hmap	*__mapnew_size(size_t size)
{
	uint8_t	ptstatus;
	__t_hmap	*hmap;

	hmap = __push(__calloc(1, sizeof(*hmap)));
	if (!hmap)
		return (NULL);
	hmap->bsize = size;
	hmap->size = __nextprime(size);
	hmap->items = __calloc(hmap->size, sizeof(__t_hmap_pair *));
	__popblk(hmap);
	if (!hmap->items)
		return (NULL);
	ptstatus = __pushtrap_status();
	__pushtrap(PTRAP_DISABLE);
	__pushn(2, hmap, hmap->items);
	__pushtrap(ptstatus);
	hmap->count = 0;
	return (hmap);
}

__t_hmap	*__mapnew(void)
{
	return (__mapnew_size(HMAP_DEF_SIZE));
}
