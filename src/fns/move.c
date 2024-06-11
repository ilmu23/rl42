/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:11:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 15:49:29 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint8_t	ft_rl_sol(rl_input_t *input)
{
	input->i = 0;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_eol(rl_input_t *input)
{
	input->i = input->len;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_sow(rl_input_t *input)
{
	ft_rl_word_start();
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_eow(rl_input_t *input)
{
	ft_rl_word_end();
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_fwd(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	input->i++;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_bck(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	input->i--;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_fwd_w(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	ft_rl_word_end();
	input->i++;
	while (input->i < input->len && ft_isspace(input->line[input->i]))
		input->i++;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_bck_w(rl_input_t *input)
{
	uint64_t	i;

	if (input->i == 0)
		return (1);
	i = input->i;
	ft_rl_word_start();
	if (input->i == i)
	{
		input->i--;
		while (input->i > 0 && ft_isspace(input->line[input->i]))
			input->i--;
		ft_rl_word_start();
	}
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_clr(rl_input_t *input)
{
	input->cursor->p_row = 1;
	input->cursor->p_col = 1;
	ft_printf("%s%s%s", TERM_CUR_RESET, TERM_CLEAR_END, input->prompt);
	ft_rl_cursor_getpos(&input->cursor->i_row, &input->cursor->i_col);
	ft_putstr_fd(input->line, 1);
	ft_rl_cursor_reset(input);
	return (1);
}
