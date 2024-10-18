/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __map_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 04:01:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

#define _floor(x) ((double)(int64_t)x)

const uint8_t	__hmap_sentinel = 42;

static inline uint64_t	_upow(const uint64_t n, int64_t p);
static inline uint8_t	_isprime(const uint64_t n);
static inline double	_estimate(const double n);
static inline double	_sqrt(const double n);
static inline void	_swap(__t_hmap *hmap, __t_hmap *tmp);

uint64_t	__nextprime(uint64_t n)
{
	while (!_isprime(n))
		n++;
	return (n);
}

uint64_t	__strhash(const char *s, const uint64_t salt, const size_t size)
{
	uint64_t	hash;
	size_t		slen;
	size_t		i;

	if (!s)
		return (0);
	i = 0;
	hash = 0;
	slen = strlen(s);
	while (i < slen)
	{
		hash += _upow(salt, slen - i + 1) * s[i];
		hash %= size;
		i++;
	}
	return (hash);
}

uint8_t	__shrinkmap(__t_hmap *hmap)
{
	return (__resizemap(hmap, hmap->bsize / 2));
}

uint8_t	__growmap(__t_hmap *hmap)
{
	return (__resizemap(hmap, hmap->bsize * 2));
}

uint8_t	__resizemap(__t_hmap *hmap, size_t bsize)
{
	__t_hmap	*tmp;
	size_t	i;

	if (bsize < HMAP_DEF_SIZE)
		return (1);
	tmp = __mapnew_size(bsize);
	if (!tmp)
		return (0);
	i = 0;
	while (i < hmap->size)
	{
		if (hmap->items[i] && hmap->items[i] != (void *)&__hmap_sentinel)
		{
			if (!__mapadd(tmp, hmap->items[i]->key, hmap->items[i]->value))
			{
				__mappop_all(tmp);
				return (0);
			}
		}
		i++;
	}
	hmap->bsize = tmp->bsize;
	hmap->count = tmp->count;
	_swap(hmap, tmp);
	return (1);
}

size_t	__gethash(const char *s, const size_t size, const size_t attempt)
{
	uint64_t	a;
	uint64_t	b;

	a = __strhash(s, HMAP_SALT_1, size);
	b = __strhash(s, HMAP_SALT_2, size);
	if (attempt >= 5)
		b *= __strhash(s, HMAP_SALT_3, size);
	return ((a + (attempt * (b + 1))) % size);
}

static inline uint64_t	_upow(const uint64_t n, int64_t p)
{
	uint64_t	out;

	if (p < 1)
		return ((p < 0) ? 0 : 1);
	out = 1;
	while (p--)
		out *= n;
	return (out);
}

static inline uint8_t	_isprime(const uint64_t n)
{
	size_t		i;
	uint64_t	root;

	if (n < 2 || (n % 2) == 0)
		return (0);
	if (n < 4)
		return (1);
	i = 3;
	root = _floor(_sqrt(n));
	while (i < root)
	{
		if ((n % i) == 0)
			return (0);
		i += 2;
	}
	return (1);
}

static inline double	_estimate(const double n)
{
	double	d1;
	double	d2;
	double	i;

	i = 1;
	d1 = i * i;
	if (n < d1)
		return (i);
	i++;
	d2 = i * i;
	while (n > d2)
	{
		i++;
		d1 = d2;
		d2 = i * i;
	}
	return ((d2 - n > n - d1) ? i - 1 : i);
}

static inline double	_sqrt(const double n)
{
	double	e;
	size_t	i;

	if (n <= 0.0)
		return (0.0);
	e = _estimate(n);
	i = 0;
	while (i++ < 5)
		e = 0.5 * (e + (n / e));
	return (e);
}

static inline void	_swap(__t_hmap *hmap, __t_hmap *tmp)
{
	__t_hmap_pair	**tmpitems;
	size_t		tmpsize;

	tmpsize = hmap->size;
	hmap->size = tmp->size;
	tmp->size = tmpsize;
	tmpitems = hmap->items;
	hmap->items = tmp->items;
	tmp->items = tmpitems;
	__mappop_all(tmp);
}
