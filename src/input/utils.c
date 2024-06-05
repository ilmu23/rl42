/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:59:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 15:44:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

void	ft_rl_redisplay(const rl_input_t *input, const rl_rdmode_t mode)
{
	uint16_t	rows;
	const char	*clr;

	switch (mode)
	{
		case LINE:
			input->cursor->row = input->cursor->i_row;
			input->cursor->col = input->cursor->i_col + input->i - (input->i != 0);
			break ;
		case INPUT:
			input->cursor->row = input->cursor->i_row;
			input->cursor->col = input->cursor->i_col;
			break ;
		case PROMPT:
			input->cursor->row = input->cursor->p_row;
			input->cursor->col = input->cursor->p_col;
			break ;
		case CLEAR:
			input->cursor->row = input->cursor->i_row;
			input->cursor->col = input->cursor->i_col + input->len;
			ft_rl_cursor_setpos(input->cursor);
			ft_putstr_fd(TERM_CLEAR_END, 1);
			input->cursor->col = input->cursor->i_col + input->i;
			ft_rl_cursor_setpos(input->cursor);
			return ;
	}
	rows = (input->cursor->col + input->len) / g_cols + 1;
	clr = ft_strdup(TERM_CLEAR_LINE_END);
	while (--rows)
		clr = ft_strnjoin(3, clr, TERM_CRNL, TERM_CLEAR_LINE_END);
	ft_push(clr);
	ft_rl_cursor_setpos(input->cursor);
	if (mode == LINE)
		ft_printf("%s%s", clr, &input->line[input->i - (input->i != 0)]);
	else if (mode == INPUT)
		ft_printf("%s%s", clr, input->line);
	else
		ft_printf("%s%s%s", clr, input->prompt, input->line);
	ft_popblk(clr);
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
}

void	ft_rl_unsetmark(uint8_t type)
{
	switch (type)
	{
		case _MARK_START:
			g_mark_s = 0;
			break ;
		case _MARK_END:
			g_mark_e = 0;
			break ;
		case _MARK_START & _MARK_END:
			g_mark_s = 0;
			g_mark_e = 0;
			break ;
	}
}

void	ft_rl_setmark(uint8_t type)
{
	switch (type)
	{
		case _MARK_START:
			g_mark_s = g_input.i;
			break ;
		case _MARK_END:
			g_mark_e = g_input.i;
			break ;
		case _MARK_START & _MARK_END:
			g_mark_s = g_input.i;
			g_mark_e = g_input.i;
			break ;
	}
}

void	ft_rl_word_start(void)
{
	if (g_input.i == 0)
		return ;
	while (g_input.i > 0 && !ft_isspace(g_input.line[g_input.i - 1]))
		g_input.i--;
}

void	ft_rl_word_end(void)
{
	if (g_input.i == g_input.len)
		return ;
	while (g_input.i < g_input.len && !ft_isspace(g_input.line[g_input.i + 1]))
		g_input.i++;
}
