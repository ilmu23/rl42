/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lstnew.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:05:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:23:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

t_list	*__lstnew(const void *blk)
{
	t_list	*out;

	__push(blk);
	out = __calloc(1, sizeof(*out));
	__popblk(blk);
	if (!out)
		return (NULL);
	out->blk = blk;
	return (out);
}
