/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __dprintf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 02:17:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 18:43:42 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__dprintf(int32_t fd, const char *f, ...)
{
	va_list	args;
	ssize_t	rval;

	va_start(args, f);
	rval = __vdprintf(fd, f, args);
	va_end(args);
	return (rval);
}
