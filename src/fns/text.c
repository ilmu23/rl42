/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:14:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 06:54:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_eof(rl_input_t *input)
{
	ft_putstr_fd(TERM_CRNL, 1);
	exit (0);
	return (1);
	(void)input;
}

uint8_t	ft_rl_del(rl_input_t *input)
{
	if (input->len == 0)
		return (1);
	ft_rl_rmchar(input);
	ft_rl_redisplay(input);
	return (1);
}

uint8_t	ft_rl_ins(rl_input_t *input)
{
	ft_rl_addchar(input, input->key);
	if (input->i != input->len)
		ft_rl_redisplay(input);
	else
	{
		ft_putchar_fd(input->key, 1);
//		input->cursor->col++;
//		ft_rl_cursor_setpos(input->cursor);
	}
	return (1);
}

uint8_t	ft_rl_upw(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_dnw(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_caw(rl_input_t *input)
{
	return (1);
	(void)input;
}
