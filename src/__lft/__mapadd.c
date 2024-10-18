/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __mapadd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 03:18:50 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline __t_hmap_pair	*_newpair(const char *key, const void *val);
static inline uint8_t		_replace(__t_hmap *hmap, __t_hmap_pair *pair, size_t i);
static inline void		*_dupval(const void *val);

uint8_t	__mapadd(__t_hmap *hmap, const char *key, const void *val)
{
	__t_hmap_pair	*pair;
	__t_hmap_pair	*cur;
	size_t		i;
	size_t		j;

	__pushn(2, key, val);
	if (!hmap || ((hmap->count * 100) / hmap->size > 70 && !__growmap(hmap)))
		return (0);
	pair = _newpair(key, val);
	__popblks(2, key, val);
	if (!pair)
		return (0);
	j = 0;
	i = __gethash(key, hmap->size, j++);
	cur = hmap->items[i];
	while (cur && cur != (void *)&__hmap_sentinel)
	{
		if (_replace(hmap, pair, i))
			return (1);
		i = __gethash(key, hmap->size, j++);
		cur = hmap->items[i];
	}
	hmap->items[i] = pair;
	hmap->count++;
	return (1);
}

static inline __t_hmap_pair	*_newpair(const char *key, const void *val)
{
	__t_hmap_pair	*pair;
	uint8_t		ptstatus;

	if (!key)
		return (NULL);
	pair = __push(__calloc(1, sizeof(*pair)));
	if (!pair)
		return (NULL);
	pair->key = __push(__strdup(key));
	if (val)
		pair->value = __push(_dupval(val));
	else
		pair->value = NULL;
	__popblks(2 + (val != NULL), pair, pair->key, pair->value);
	if (!pair->key || !pair->value)
		return (NULL);
	ptstatus = __pushtrap_status();
	__pushtrap(PTRAP_DISABLE);
	__pushn(2 + (val != NULL), pair, pair->key, pair->value);
	__pushtrap(ptstatus);
	return (pair);
}

static inline uint8_t	_replace(__t_hmap *hmap, __t_hmap_pair *pair, size_t i)
{
	if (__strequals(hmap->items[i]->key, pair->key))
	{
		__mappop(hmap->items[i]);
		hmap->items[i] = pair;
		return (1);
	}
	return (0);
}

static inline void	*_dupval(const void *val)
{
	size_t		vsize;
	void		*out;

	vsize = __getblksize(val);
	out = __calloc(1, vsize);
	if (!out)
		return (NULL);
	return (memcpy(out, val, vsize));
}
