/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 01:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 10:27:41 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

int32_t	ft_rl_perror(void)
{
	const char		*msg;

	msg = __push(__strlower(__strdup(strerror(errno))));
	if (!msg || !*msg)
		msg = strerror(errno);
	__dprintf(2, "rl42: %s\n", msg);
	return (errno);
}
