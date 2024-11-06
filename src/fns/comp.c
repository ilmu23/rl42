/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:07 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/06 14:44:15 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_cmp(rl_input_t *input)
{
	rl_fn_t	f;

	if (input->len == 0 || !ft_rl_get(_CMP_ENABLE_HASH))
		return (1);
	f = ft_rl_complete(input);
	return (f && f != ft_rl_cmp) ? f(input) : 1;
}
