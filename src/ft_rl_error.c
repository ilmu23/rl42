/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 01:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 02:02:00 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

int32_t	ft_rl_perror(void)
{
	const int32_t	err = errno;
	const char		*msg;

	msg = ft_push(ft_strupper(ft_strdup(strerror(err))));
	if (!msg || !*msg)
		msg = strerror(err);
	ft_dprintf(2, "ft_readline: %s\n", msg);
	return (err);
}
