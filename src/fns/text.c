/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:14:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 12:17:48 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint8_t	ft_rl_eof(rl_input_t *input)
{
	if (input->len != 0)
		return (1);
	ft_putstr_fd(TERM_CRNL, 1);
	input->exittype = EOF;
	return (0);
}

uint8_t	ft_rl_del(rl_input_t *input)
{
	if (input->len == 0 || input->i == input->len)
		return (1);
	ft_rl_rmchar_back(input);
	ft_rl_redisplay(input, LINE);
	return (1);
}

uint8_t	ft_rl_bdl(rl_input_t *input)
{
	if (input->len == 0 || input->i == 0)
		return (1);
	ft_rl_rmchar(input);
	ft_rl_redisplay(input, LINE);
	return (1);
}

uint8_t	ft_rl_ins(rl_input_t *input)
{
	ft_rl_addchar(input, input->key);
	if (input->i != input->len)
		ft_rl_redisplay(input, LINE);
	else
		ft_putchar_fd(input->key, 1);
	return (1);
}

uint8_t	ft_rl_upw(rl_input_t *input)
{
	uint64_t	i;

	if (input->len == 0)
		return (1);
	ft_rl_word_start();
	ft_rl_setmark(_MARK_START);
	ft_rl_word_end();
	ft_rl_setmark(_MARK_END);
	i = g_mark_s;
	while (i <= g_mark_e)
	{
		input->line[i] = ft_toupper(input->line[i]);
		i++;
	}
	ft_rl_unsetmark(_MARK_START);
	ft_rl_unsetmark(_MARK_END);
	ft_rl_redisplay(input, ALL);
	return (1);
}

uint8_t	ft_rl_dnw(rl_input_t *input)
{
	uint64_t	i;

	if (input->len == 0)
		return (1);
	ft_rl_word_start();
	ft_rl_setmark(_MARK_START);
	ft_rl_word_end();
	ft_rl_setmark(_MARK_END);
	i = g_mark_s;
	while (i <= g_mark_e)
	{
		input->line[i] = ft_tolower(input->line[i]);
		i++;
	}
	ft_rl_unsetmark(_MARK_START);
	ft_rl_unsetmark(_MARK_END);
	ft_rl_redisplay(input, ALL);
	return (1);
}

uint8_t	ft_rl_caw(rl_input_t *input)
{
	uint64_t	i;

	if (input->len == 0)
		return (1);
	ft_rl_word_start();
	ft_rl_setmark(_MARK_START);
	ft_rl_word_end();
	ft_rl_setmark(_MARK_END);
	i = g_mark_s;
	input->line[i] = ft_toupper(input->line[i]);
	while (++i <= g_mark_e)
		input->line[i] = ft_tolower(input->line[i]);
	ft_rl_unsetmark(_MARK_START);
	ft_rl_unsetmark(_MARK_END);
	ft_rl_redisplay(input, ALL);
	return (1);
}
