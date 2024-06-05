/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:12:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 15:49:25 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include <stdio.h>

uint8_t	ft_rl_acl(rl_input_t *input)
{
	input->exittype = ACL;
	return (0);
	(void)input;
}

uint8_t	ft_rl_pvh(rl_input_t *input)
{
	if (!ft_rl_hist_get_prev(input))
		return (1);
	ft_rl_redisplay(input, INPUT);
	input->i = input->len;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_nxh(rl_input_t *input)
{
	if (!ft_rl_hist_get_next(input))
		return (1);
	ft_rl_redisplay(input, INPUT);
	input->i = input->len;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_soh(rl_input_t *input)
{
	if (!ft_rl_hist_get_last(input))
		return (1);
	ft_rl_redisplay(input, INPUT);
	input->i = input->len;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_eoh(rl_input_t *input)
{
	if (!ft_rl_hist_get_first(input))
		return (1);
	ft_rl_redisplay(input, INPUT);
	input->i = input->len;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_rsh_i(rl_input_t *input)
{
	return (ft_rl_hist_isearch(input, _SEARCH_BCK));
}

uint8_t	ft_rl_fsh_i(rl_input_t *input)
{
	return (ft_rl_hist_isearch(input, _SEARCH_FWD));
}
