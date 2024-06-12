/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:59:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 22:15:25 by ivalimak         ###   ########.fr       */
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

void	ft_rl_swap_words(rl_input_t *input)
{
	uint64_t	tmp;
	uint64_t	marks[4];
	const char	*subs[2];

	while (input->i > 0 && ft_isspace(input->line[input->i]))
		input->i--;
	ft_rl_word_start();
	marks[0] = input->i;
	ft_rl_word_end();
	marks[1] = ft_min(input->i + 1, input->len);
	if (input->i < input->len)
		ft_rl_fwd_w(input);
	else
	{
		ft_rl_bck_w(input);
		ft_rl_bck_w(input);
	}
	marks[2] = input->i;
	ft_rl_word_end();
	input->i += (input->i < input->len);
	marks[3] = input->i;
	if (marks[1] > marks[2])
	{
		tmp = marks[0];
		marks[0] = marks[2];
		marks[2] = tmp;
		tmp = marks[1];
		marks[1] = marks[3];
		marks[3] = tmp;
	}
	subs[0] = ft_push(ft_substr(input->line, marks[2], marks[3] - marks[2]));
	subs[1] = ft_push(ft_substr(input->line, marks[0], marks[1] - marks[0]));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	ft_snprintf(&input->line[marks[0]], marks[3] - marks[0], "%-*s%s",
			marks[3] - marks[0] - ft_strlen(subs[1]), subs[0], subs[1]);
	ft_popblks(2, subs[0], subs[1]);
	input->i = marks[3];
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
		case _MARK_START | _MARK_END:
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
		case _MARK_START | _MARK_END:
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
