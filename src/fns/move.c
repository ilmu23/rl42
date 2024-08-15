/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:11:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:29:45 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_sol(rl_input_t *input)
{
	if (!input->len)
		return (1);
	input->i = 0;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_eol(rl_input_t *input)
{
	if (!input->len)
		return (1);
	input->i = input->len - (ft_rl_geteditmode() == _MD_VI_CMD);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_sow(rl_input_t *input)
{
	if (!input->len)
		return (1);
	ft_rl_word_start();
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_eow(rl_input_t *input)
{
	if (!input->len)
		return (1);
	ft_rl_word_end();
	input->i -= (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_fwd(rl_input_t *input)
{
	int32_t	count;

	if (!input->len)
		return (1);
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->i == input->len && count > 0)
		return (1);
	while (count > 0 && input->i < input->len)
	{
		input->i++;
		count--;
	}
	while (count < 0 && input->i > 0)
	{
		input->i--;
		count++;
	}
	input->i -= (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_bck(rl_input_t *input)
{
	int32_t	count;

	if (!input->len)
		return (1);
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	while (count > 0 && input->i > 0)
	{
		input->i--;
		count--;
	}
	while (count < 0 && input->i < input->len)
	{
		input->i++;
		count++;
	}
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_fwd_w(rl_input_t *input)
{
	int32_t	count;

	if (!input->len)
		return (1);
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->i == input->len && count > 0)
		return (2);
	while (count > 0)
	{
		ft_rl_word_end();
		if (input->i == input->len)
			break ;
		input->i++;
		while (input->i < input->len && isspace(input->line[input->i]))
			input->i++;
		count--;
	}
	while (count < 0)
	{
		if (ft_rl_bck_w(input) == 2)
			break ;
		count++;
	}
	input->i -= (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_bck_w(rl_input_t *input)
{
	uint64_t	i;
	int32_t		count;

	if (!input->len)
		return (1);
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->i == 0 && count > 0)
		return (2);
	while (count > 0)
	{
		i = input->i;
		ft_rl_word_start();
		if (input->i == 0)
			break ;
		if (input->i == i)
		{
			input->i--;
			while (input->i > 0 && isspace(input->line[input->i]))
				input->i--;
			ft_rl_word_start();
		}
		count--;
	}
	while (count < 0)
	{
		if (ft_rl_fwd_w(input) == 2)
			break ;
		count++;
	}
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_clr(rl_input_t *input)
{
	input->cursor->p_row = 1;
	input->cursor->p_col = 1;
	printf("%s%s%s", TERM_CUR_RESET, TERM_CLEAR_END, input->prompt);
	ft_rl_cursor_getpos(&input->cursor->i_row, &input->cursor->i_col);
	__putstr_fd(input->line, 1);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_ffc(rl_input_t *input)
{
	uint64_t	i;
	uint8_t		c;

	if (g_argument.set && ft_rl_getarg() < 0)
		return (ft_rl_bfc(input));
	i = input->i + 1;
	c = ft_rl_getkey();
	while (i < input->len && input->line[i] != c)
		i++;
	if (input->line[i] == c)
	{
		input->i = i;
		ft_rl_cursor_reset(input);
	}
	return (1);
}

uint8_t	ft_rl_bfc(rl_input_t *input)
{
	uint64_t	i;
	uint8_t		c;

	if (g_argument.set && ft_rl_getarg() < 0)
		return (ft_rl_ffc(input));
	i = input->i - 1;
	c = ft_rl_getkey();
	while (i > 0 && input->line[i] != c)
		i--;
	if (input->line[i] == c)
	{
		input->i = i;
		ft_rl_cursor_reset(input);
	}
	return (1);
}
