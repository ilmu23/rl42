/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:12:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 12:14:17 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include <stdio.h>

uint8_t	ft_rl_acl(rl_input_t *input)
{
	ft_putstr_fd(TERM_CRNL, 1);
	input->exittype = ACL;
	return (0);
	(void)input;
}

uint8_t	ft_rl_pvh(rl_input_t *input)
{
	if (!ft_rl_hist_get_prev(input))
		return (1);
	input->i = 0;
	ft_rl_redisplay(input, LINE);
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
	input->i = 0;
	ft_rl_redisplay(input, LINE);
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
	input->i = 0;
	ft_rl_redisplay(input, LINE);
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
	input->i = 0;
	ft_rl_redisplay(input, LINE);
	input->i = input->len;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_rsh(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_fsh(rl_input_t *input)
{
	return (1);
	(void)input;
}
