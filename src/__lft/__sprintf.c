/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __sprintf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 02:21:07 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 18:15:17 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__sprintf(char *s, const char *f, ...)
{
	va_list	args;
	ssize_t	rval;

	va_start(args, f);
	rval = __vsprintf(s, f, args);
	va_end(args);
	return (rval);
}
