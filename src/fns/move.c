/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:11:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 11:42:48 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_sol(rl_input_t *input)
{
	input->i = 0;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_eol(rl_input_t *input)
{
	input->i = input->len;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_fwd(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	input->i++;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_bck(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	input->i--;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
	return (1);
}

uint8_t	ft_rl_fwd_w(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_bck_w(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_clr(rl_input_t *input)
{
	return (1);
	(void)input;
}
