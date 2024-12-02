/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 04:07:16 by ivalimak          #+#    #+#             */
/*   Updated: 2024/12/02 13:44:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_append(const char *s, const uint8_t direction);

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
	ft_rl_kill_region(input, direction);
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
			while (input->i < input->len && isspace(input->line[input->i]))
				input->i++;
			ft_rl_word_end();
			input->i++;
			while (isspace(input->line[input->i]))
				input->i++;
			ft_rl_setmark(_MARK_END);
			break ;
	}
	ft_rl_kill_region(input, direction);
	if (direction == _KILL_FWD)
		input->i = i;
	ft_rl_unsetmark(_MARK_START | _MARK_END);
}

void ft_rl_kill_region(rl_input_t *input, const uint8_t direction)
{
	const char	*subs[2];

	subs[0] = __push(__substr(input->line, 0, g_mark_s.pos));
	subs[1] = __push(__substr(input->line, g_mark_e.pos, input->len - g_mark_e.pos));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	if (!(g_status & _KILL_NOUPDATE)) {
		if (g_status & _KILL_APPEND)
			_append(__substr(input->line, g_mark_s.pos, g_mark_e.pos - g_mark_s.pos), direction);
		else
			__lstadd_front(&g_kill_ring, __lstnew(__substr(input->line, g_mark_s.pos, g_mark_e.pos - g_mark_s.pos)));
	}
	__popblk(input->line);
	input->line = __push(__strjoin(subs[0], subs[1]));
	__popblks(2, subs[0], subs[1]);
	input->len = strlen(input->line);
}

static inline void	_append(const char *s, const uint8_t direction)
{
	const char	*content;

	content = __strdup(g_kill_ring->blk);
	__lstrmnode(&g_kill_ring, g_kill_ring);
	if (direction == _KILL_FWD)
		__lstadd_front(&g_kill_ring, __lstnew(__strjoin(content, s)));
	else
		__lstadd_front(&g_kill_ring, __lstnew(__strjoin(s, content)));
}
