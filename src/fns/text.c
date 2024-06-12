/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:14:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 23:50:46 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint8_t	ft_rl_eof(rl_input_t *input)
{
	if (input->len != 0)
		return (1);
	ft_printf("%s%s", TERM_CLEAR_END, TERM_CRNL);
	input->exittype = EOF;
	return (0);
}

uint8_t	ft_rl_del(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->len == 0 || (input->i == input->len && count > 0))
		return (2);
	while (count > 0)
	{
		ft_rl_rmchar_back(input);
		count--;
	}
	while (count < 0)
	{
		if (ft_rl_bdl(input) == 2)
			break ;
		count++;
	}
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_bdl(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->len == 0 || (input->i == 0 && count > 0))
		return (2);
	while (count > 0)
	{
		ft_rl_rmchar(input);
		count--;
	}
	while (count < 0)
	{
		if (ft_rl_del(input) == 2)
			break ;
		count++;
	}
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_ins(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	while (count-- > 0)
		ft_rl_addchar(input, input->key);
	ft_rl_redisplay(input, INPUT);
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
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	ft_rl_redisplay(input, INPUT);
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
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	ft_rl_redisplay(input, INPUT);
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
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_fkl(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	ft_rl_kill_line(input, _KILL_FWD);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_bkl(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	ft_rl_kill_line(input, _KILL_BCK);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_kln(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	ft_rl_kill_line(input, _KILL_FWD | _KILL_BCK);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_fkw(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	ft_rl_kill_word(input, _KILL_FWD);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_bkw(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	ft_rl_kill_word(input, _KILL_BCK);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_tpc(rl_input_t *input)
{
	uint8_t	c;

	if (input->len < 2)
		return (1);
	if (input->i == 0)
		input->i++;
	else if (input->i == input->len)
		input->i--;
	c = input->line[input->i - 1];
	input->line[input->i - 1] = input->line[input->i];
	input->line[input->i++] = c;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_tpw(rl_input_t *input)
{
	if (input->len < 3 || !ft_strchr(input->line, ' ')
		|| input->i < ft_strclen(input->line, ' '))
		return (1);
	ft_rl_swap_words(input);
	ft_rl_redisplay(input, INPUT);
	return (1);
}
