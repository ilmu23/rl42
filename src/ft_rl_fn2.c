/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_fn2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 22:41:48 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 22:06:11 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_fwd_w(t_rl_input *input)
{
	t_rl_word	*w;

	if (!input || !input->current)
		return (1);
	w = input->current;
	w->i = w->len;
	if (w->next)
	{
		w = w->next;
		if (w->wtype == SPACE)
		{
			w->i = w->len;
			if (w->next)
				w = w->next;
		}
	}
	input->current = w;
	ft_rl_resetcursor(input);
	return (1);
}

uint8_t	ft_rl_bck_w(t_rl_input *input)
{
	t_rl_word	*w;

	if (!input || !input->current)
		return (1);
	w = input->current;
	if (w->i > 0)
		w->i = 0;
	else if (w->prev)
		w = w->prev;
	if (w->wtype == SPACE && w->prev)
		w = w->prev;
	if (w->i > 0)
		w->i = 0;
	input->current = w;
	ft_rl_resetcursor(input);
	return (1);
}

uint8_t	ft_rl_acl(t_rl_input *input)
{
	if (!input)
		return (0);
	ft_rl_eol(input);
	ft_printf("\n%s", TERM_CLEAR_END);
	input->exittype = ACL;
	return (0);
}

uint8_t	ft_rl_eof(t_rl_input *input)
{
	if (!input || input->head)
		return (1);
	ft_putchar_fd('\n', 1);
	input->exittype = EOF;
	return (0);
}

uint8_t	ft_rl_pvh(t_rl_input *input)
{
	t_rl_input	*newinput;

	newinput = ft_rl_hist_getprev(input, 1);
	if (!newinput || input == newinput)
		return (1);
	ft_rl_popwords(input->head);
	input->head = ft_rl_dupwords(newinput->head);
	input->current = input->head;
	while (input->current && input->current->next)
		input->current = input->current->next;
	ft_rl_redisplay(input, LINE);
	return (1);
}
