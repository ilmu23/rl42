/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:14:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:32:28 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_eof(rl_input_t *input)
{
	if (input->len != 0)
		return (1);
	printf("%s%s", TERM_CLEAR_END, TERM_CRNL);
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
	if (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len && input->len)
		input->i--;
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
	if (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len && input->len)
		input->i--;
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
	int32_t		count;
	uint64_t	i;
	uint64_t	j;

	if (input->len == 0)
		return (1);
	i = 1;
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count < 0)
		i = input->i;
	while (count != 0)
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		j = g_mark_s.pos;
		while (j <= g_mark_e.pos)
		{
			input->line[j] = toupper(input->line[j]);
			j++;
		}
		ft_rl_unsetmark(_MARK_START | _MARK_END);
		if (count > 0)
		{
			if (--count > 0 && ft_rl_fwd_w(input) == 2)
				break ;
		}
		else if (count < 0)
		{
			ft_rl_word_start();
			if (++count < 0 && ft_rl_bck_w(input) == 2)
			{
				input->i = i;
				break ;
			}
			if (count == 0)
				input->i = i;
		}
	}
	input->i = MIN(input->i + 1, input->len);
	ft_rl_word_end();
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_dnw(rl_input_t *input)
{
	int32_t		count;
	uint64_t	i;
	uint64_t	j;

	if (input->len == 0)
		return (1);
	i = 1;
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count < 0)
		i = input->i;
	while (count != 0)
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		j = g_mark_s.pos;
		while (j <= g_mark_e.pos)
		{
			input->line[j] = tolower(input->line[j]);
			j++;
		}
		ft_rl_unsetmark(_MARK_START | _MARK_END);
		if (count > 0)
		{
			if (--count > 0 && ft_rl_fwd_w(input) == 2)
				break ;
		}
		else if (count < 0)
		{
			ft_rl_word_start();
			if (++count < 0 && ft_rl_bck_w(input) == 2)
			{
				input->i = i;
				break ;
			}
			if (count == 0)
				input->i = i;
		}
	}
	input->i = MIN(input->i + 1, input->len);
	ft_rl_word_end();
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_caw(rl_input_t *input)
{
	int32_t		count;
	uint64_t	i;
	uint64_t	j;

	if (input->len == 0)
		return (1);
	i = 1;
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count < 0)
		i = input->i;
	while (count != 0)
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		j = g_mark_s.pos;
		input->line[j] =  toupper(input->line[j]);
		while (++j <= g_mark_e.pos)
			input->line[j] = tolower(input->line[j]);
		ft_rl_unsetmark(_MARK_START | _MARK_END);
		if (count > 0)
		{
			if (--count > 0 && ft_rl_fwd_w(input) == 2)
				break ;
		}
		else if (count < 0)
		{
			ft_rl_word_start();
			if (++count < 0 && ft_rl_bck_w(input) == 2)
			{
				input->i = i;
				break ;
			}
			if (count == 0)
				input->i = i;
		}
	}
	input->i = MIN(input->i + 1, input->len);
	ft_rl_word_end();
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
	if (input->len == 0)
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

uint8_t	ft_rl_krg(rl_input_t *input)
{
	uint64_t	i;

	if (!g_mark_u.set)
		return (1);
	i = input->i;
	input->i = MIN(i + 1, g_mark_u.pos + 1);
	ft_rl_setmark(_MARK_START);
	input->i = MAX(i, g_mark_u.pos);
	ft_rl_setmark(_MARK_END);
	ft_rl_kill_region(input);
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	input->i = MIN(input->i, input->len);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_kws(rl_input_t *input)
{
	if (!isspace(input->line[input->i])
		&& (input->i == 0 || !isspace(input->line[input->i - 1])))
		return (1);
	if (!isspace(input->line[input->i]))
		input->i--;
	ft_rl_setmark(_MARK_START | _MARK_END);
	while (input->i > 0 && isspace(input->line[input->i]))
		input->i--;
	if (!isspace(input->line[input->i]))
		input->i++;
	ft_rl_setmark(_MARK_START);
	input->i = g_mark_e.pos;
	while (input->i < input->len && isspace(input->line[input->i]))
		input->i++;
	ft_rl_setmark(_MARK_END);
	ft_rl_kill_region(input);
	input->i = g_mark_s.pos;
	ft_rl_unsetmark(_MARK_START | _MARK_END);
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
	if (input->i == input->len && ft_rl_geteditmode() == _MD_VI_CMD)
		input->i--;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_tpw(rl_input_t *input)
{
	if (input->len < 3 || !strchr(input->line, ' ')
		|| input->i < __strclen(input->line, ' '))
		return (1);
	ft_rl_swap_words(input);
	if (input->i == input->len && ft_rl_geteditmode() == _MD_VI_CMD)
		input->i--;
	ft_rl_redisplay(input, INPUT);
	return (1);
}
