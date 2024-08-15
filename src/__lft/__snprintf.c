/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __snprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 08:54:05 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 18:14:44 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__snprintf(char *s, size_t size, const char *f, ...)
{
	va_list	args;
	ssize_t	rval;

	va_start(args, f);
	rval = __vsnprintf(s, size, f, args);
	va_end(args);
	return (rval);
}
