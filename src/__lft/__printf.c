/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __printf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 05:31:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 18:14:04 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__printf(const char *f, ...)
{
	va_list	args;
	ssize_t	rval;

	va_start(args, f);
	rval = __vprintf(f, args);
	va_end(args);
	return (rval);
}
