/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __push.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 19:35:34 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:13:12 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

void	*__push(const void *blk)
{
	if (blk)
		__mark(blk);
	return ((void *)blk);
}

void	*__pusharr(const void *blk)
{
	const void	**arr;

	arr = (const void **)blk;
	__push(arr);
	while (arr && *arr)
		__push(*arr++);
	return ((void **)blk);
}

void	__pushn(size_t blks, ...)
{
	va_list	args;

	va_start(args, blks);
	while (blks)
	{
		__push(va_arg(args, void *));
		blks--;
	}
	va_end(args);
}
