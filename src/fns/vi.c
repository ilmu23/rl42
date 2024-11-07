/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:21:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/02 09:12:01 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_globals.h"
#include "ft_rl_internal.h"

uint8_t	ft_rl_vi_del(rl_input_t *input)
{
	int32_t	count;
	rl_fn_t	f;

	f = ft_rl_getinput(&input->keyseq);
	if (f == ft_rl_arg)
	{
		g_status ^= _VI_ARG;
		f(input);
		g_status ^= _VI_ARG;
		f = ft_rl_getinput(&input->keyseq);
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

uint8_t	ft_rl_vi_sub(rl_input_t	*input)
{
	ft_rl_del(input);
	ft_rl_md_vi(input);
	return (1);
}

uint8_t	ft_rl_vi_sbl(rl_input_t *input)
{
	ft_rl_kln(input);
	ft_rl_md_vi(input);
	return (1);
}

uint8_t	ft_rl_vi_rep(rl_input_t *input)
{
	uint64_t	i;
	rl_fn_t		f;
	int32_t		count;

	if (input->len == 0)
		return (1);
	i = input->i;
	f = ft_rl_getinput(&input->keyseq);
	if (f == ft_rl_arg)
	{
		g_status ^= _VI_ARG;
		f(input);
		g_status ^= _VI_ARG;
	}
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (strlen(input->keyseq) > 1 || *input->keyseq < ' ' || *input->keyseq > '~')
		return (1);
	while (count-- && i < input->len)
		input->line[i++] = *input->keyseq;
	ft_rl_redisplay(input, INPUT);
	return (1);
}
