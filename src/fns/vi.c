/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:21:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/08 12:55:37 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_globals.h"
#include "ft_rl_internal.h"

uint8_t	ft_rl_vi_del(rl_input_t *input)
{
	int32_t	count;
	rl_fn_t	f;

	input->key = ft_rl_getkey();
	f = ft_rl_getmap(input->key);
	if (f == ft_rl_arg)
	{
		g_status ^= _VI_ARG;
		f(input);
		g_status ^= _VI_ARG;
		f = ft_rl_getmap(input->key);
	}
	if (f == ft_rl_sol)
		return (ft_rl_bkl(input));
	if (f == ft_rl_eol)
		return (ft_rl_fkl(input));
	if (f == ft_rl_fwd_w)
		f = ft_rl_fkw;
	else if (f == ft_rl_bck_w)
		f = ft_rl_bkw;
	else if (f == ft_rl_fwd)
		f = ft_rl_del;
	else if (f == ft_rl_bck)
		f = ft_rl_bdl;
	else
		return (1);
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	while (count--)
		f(input);
	return (1);
}
