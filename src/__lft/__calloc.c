/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __calloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:59:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:17:59 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	*__calloc(const size_t n, const size_t size)
{
	void	*out;
	size_t	asize;

	asize = n * size;
	out = __alloc(asize);
	if (!out)
		return (NULL);
	memset(out, 0, asize);
	return (out);
}

void	*__gc_calloc(const size_t n, const size_t size)
{
	void	*out;
	size_t	asize;

	asize = n * size;
	out = malloc(asize);
	if (!out)
		return (NULL);
	memset(out, 0, asize);
	return (out);
}
