/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __vsprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 02:34:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 18:26:11 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__vsprintf(char *s, const char *f, va_list args)
{
	return (__vsnprintf(s, SIZE_MAX, f, args));
}
