/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:55:57 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 17:22:11 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint64_t	ft_rl_comp_getlongest(const t_list *completions)
{
	uint64_t	longest;
	uint64_t	len;

	longest = 0;
	while (completions)
	{
		len = ft_strlen(completions->blk);
		if (len > longest)
			longest = len;
		completions = completions->next;
	}
	return (longest);
}

// disgusting
void	ft_rl_comp_display(rl_input_t *input, const t_list *completions, const void *cur, const void *prv)
{
	uint16_t	i;
	uint16_t	cpr;
	uint16_t	rows;
	uint64_t	llen;
	rl_cursor_t	curs;

	llen = ft_rl_comp_getlongest(completions);
	cpr = g_cols / (llen + 2);
	rows = *completions->size / cpr + 1;
	while (input->cursor->i_row + (int16_t)(input->len / g_cols) + rows > g_rows)
	{
		input->cursor->p_row--;
		input->cursor->i_row--;
		rows--;
	}
	ft_rl_redisplay(input, PROMPT);
	if (!cur)
	{
		i = 0;
		ft_putchar_fd('\n', 1);
		while (completions)
		{
			ft_printf("%-*s", llen, completions->blk);
			if (++i == cpr)
			{
				ft_putchar_fd('\n', 1);
				i = 0;
			}
			else
				ft_putchar_fd(' ', 1);
			completions = completions->next;
		}
	}
	else
	{
		i = 0;
		curs.row = input->cursor->i_row + 1;
		curs.col = input->cursor->i_col + input->len;
		while (curs.col >= g_cols)
		{
			curs.row++;
			curs.col -= g_cols;
		}
		curs.col = 1;
		while (completions && completions->blk != cur)
		{
			if (++i == cpr)
			{
				curs.row++;
				curs.col = 1;
			}
			else
				curs.col += llen + 1;
			completions = completions->next;
		}
		if (!completions)
			return ;
		if (prv && completions->prev)
		{
			curs.col -= llen + 1;
			ft_rl_cursor_setpos(&curs);
			ft_printf("%-*s", llen, prv);
			if (i == 0)
				ft_putchar_fd('\n', 1);
			else
				ft_putchar_fd(' ', 1);
			ft_printf("%s%-*s%s", ft_rl_hlcolor(), llen, cur, SGR_RESET);
		}
		else
		{
			ft_rl_cursor_setpos(&curs);
			ft_printf("%s%-*s%s", ft_rl_hlcolor(), llen, cur, SGR_RESET);
			if (prv)
			{
				curs.col = (llen + 1) * (*completions->size - 1) + 1;
				ft_rl_cursor_setpos(&curs);
				ft_printf("%-*s", llen, prv);
			}
		}
	}
	ft_rl_cursor_reset(input);
}
