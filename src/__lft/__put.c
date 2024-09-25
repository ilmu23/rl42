/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __put.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:30:14 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/18 15:40:59 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

ssize_t	__putchar_fd(const uint8_t c, const int32_t fd)
{
	return (write(fd, &c, 1));
}

ssize_t	__putendl_fd(const char *s, const int32_t fd)
{
	ssize_t	rv1;
	ssize_t	rv2;

	rv1 = __putstr_fd(s, fd);
	if (rv1 >= 0)
		rv2 = __putchar_fd('\n', fd);
	else
		rv2 = -1;
	return ((rv1 == -1 || rv2 == -1) ? -1 : rv1 + rv2);
}

ssize_t	__putstr_fd(const char *s, const int32_t fd)
{
	return (write(fd, s, strlen(s)));
}
