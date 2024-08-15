/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __objpair.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 01:02:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 15:18:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

t_objpair	*__newpair(const char *key, const t_obj *obj)
{
	t_objpair	*out;

	out = malloc(sizeof(*out));
	if (!out)
	{
		__putendl_fd(E_ALLOC, 2);
		__exit(69);
	}
	out->key = key;
	out->obj = obj;
	return (out);
}
