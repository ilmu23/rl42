/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 00:04:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

uintptr_t	__nextarg(__t_list **args)
{
	uintptr_t	*val;

	val = (uintptr_t *)(*args)->blk;
	*args = (*args)->next;
	return (*val);
}

uintptr_t	__getarg(const __t_list *args, size_t argn)
{
	while (--argn)
		args = args->next;
	return (*(uintptr_t *)args->blk);
}

ssize_t	__getlen(__t_list *strings)
{
	ssize_t	len;

	len = 0;
	while (strings)
	{
		if (strings->blk)
			len += __getblksize(strings->blk) - 1;
		strings = strings->next;
	}
	return (len);
}

char	*__getcnvstr(const char *f)
{
	size_t	i;

	i = 0;
	while (f[i] && !strchr(PF_FORMAT_SPEC, f[i]))
		i++;
	return (__substr(f, 0, i + 1));
}

char	*__cstr(const uint8_t c, const size_t slen)
{
	char	*out;

	out = __calloc(slen + 1, sizeof(*out));
	if (!out)
		return (NULL);
	memset(out, c, slen);
	return (out);
}
