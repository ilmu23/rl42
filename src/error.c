/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 01:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 19:18:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

int32_t	ft_rl_perror(void)
{
	const int32_t	err = errno;
	const char		*msg;

	msg = __push(__strlower(__strdup(strerror(err))));
	if (!msg || !*msg)
		msg = strerror(err);
	__dprintf(2, "rl42: %s\n", msg);
	return (err);
}
