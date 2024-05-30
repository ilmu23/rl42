/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 04:13:31 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_mta(rl_input_t *input)
{
	uint16_t	key;

	if (read(0, &key, 1) == -1)
		exit(ft_rl_perror());
	key = key << 8 | '\e';
	input->keystr = ft_rl_keystr(key);
	return (ft_rl_execmap(input));
}

uint8_t	ft_rl_dcl(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_hlc(rl_input_t *input)
{
	return (1);
	(void)input;
}
