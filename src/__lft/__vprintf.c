/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __vprintf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 02:23:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 18:20:48 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__vprintf(const char *f, va_list args)
{
	va_list	copy;
	ssize_t	rval;
	size_t	size;
	char	*out;

	va_copy(copy, args);
	rval = __vsnprintf(NULL, 42, f, copy);
	va_end(copy);
	if (rval != -1)
	{
		size = (size_t)rval + 1;
		out = __alloc(size);
		if (!out)
			return (-1);
		rval = __vsnprintf(out, size, f, args);
	}
	if (rval == -1 || write(1, out, (size_t)rval) == -1)
		return (-1);
	return (rval);
}
