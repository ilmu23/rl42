/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:40 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/07 20:13:09 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_execmap(rl_input_t *input)
{
	rl_fn_t	f;

	f = ft_rl_getmap(input->key);
	if (!f)
		return (1);
	return (f(input));
}
