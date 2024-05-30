/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 01:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 08:34:49 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

int32_t	ft_rl_perror(void)
{
	const int32_t	err = errno;
	const char		*msg;

	msg = ft_push(ft_strlower(ft_strdup(strerror(err))));
	if (!msg || !*msg)
		msg = strerror(err);
	ft_dprintf(2, "rl42: %s\n", msg);
	return (err);
}
