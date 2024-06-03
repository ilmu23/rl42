/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:07 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/03 14:35:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_cmp(rl_input_t *input)
{
	if (input->len == 0)
		return (1);
	input->key = 0;
	ft_rl_complete(input);
	if (ft_rl_ismapped(input->key) && ft_rl_getmap(input->key) != ft_rl_cmp)
	{
		input->keystr = ft_rl_keystr(input->key);
		return (ft_rl_execmap(input));
	}
	return (1);
}
