/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __getblksize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:38:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:15:49 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

size_t	__getblksize(const void *blk)
{
	t_obj	*obj;

	obj = __getobj(blk);
	if (!obj)
		return (0);
	return (obj->asize);
}
