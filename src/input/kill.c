/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 04:07:16 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/14 16:07:33 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_kill(rl_input_t *input);

void	ft_rl_kill_line(rl_input_t *input, const uint8_t direction)
{
	ft_rl_setmark(_MARK_START | _MARK_END);
	if (direction & _KILL_BCK)
	{
		input->i = 0;
		ft_rl_setmark(_MARK_START);
	}
	if (direction & _KILL_FWD)
	{
		input->i = input->len;
		ft_rl_setmark(_MARK_END);
	}
	_kill(input);
	if (direction & _KILL_BCK)
		input->i = 0;
	else
		input->i = input->len;
	ft_rl_unsetmark(_MARK_START | _MARK_END);
}

void	ft_rl_kill_word(rl_input_t *input, const uint8_t direction)
{
	uint64_t	i;

	i = input->i;
	ft_rl_setmark(_MARK_START | _MARK_END);
	switch (direction)
	{
		case _KILL_BCK:
			ft_rl_bck_w(input);
			ft_rl_setmark(_MARK_START);
			break ;
		case _KILL_FWD:
			while (input->i < input->len && ft_isspace(input->line[input->i]))
				input->i++;
			ft_rl_word_end();
			input->i++;
			ft_rl_setmark(_MARK_END);
			break ;
	}
	_kill(input);
	if (direction == _KILL_FWD)
		input->i = i;
	ft_rl_unsetmark(_MARK_START | _MARK_END);
}

static inline void	_kill(rl_input_t *input)
{
	const char	*subs[2];

	subs[0] = ft_push(ft_substr(input->line, 0, g_mark_s.pos));
	subs[1] = ft_push(ft_substr(input->line, g_mark_e.pos, input->len - g_mark_e.pos));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	ft_popblk(input->line);
	input->line = ft_push(ft_strjoin(subs[0], subs[1]));
	ft_popblks(2, subs[0], subs[1]);
	input->len = ft_strlen(input->line);
}
