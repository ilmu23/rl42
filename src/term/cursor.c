/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 00:49:44 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/18 15:09:23 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

rl_cursor_t	*ft_rl_cursor_init(void)
{
	rl_cursor_t	*out;

	out = __push(__alloc(sizeof(*out)));
	if (!out)
		exit(ft_rl_perror());
	ft_rl_cursor_getpos(&out->row, &out->col);
	out->p_row = out->row;
	out->p_col = out->col;
	return (out);
}

void	ft_rl_cursor_getpos(int16_t *row, int16_t *col)
{
	size_t	i;
	char	buf[17];
	
	if (!row || !col)
		return ;
	i = 0;
	memset(buf, 0, 17);
	__printf("%s", TERM_STATUS);
	if (read(0, buf, 16) == -1)
		exit(ft_rl_perror());
	while (buf[i] && !isdigit(buf[i]))
		i++;
	*row = atoi(&buf[i]);
	while (isdigit(buf[i]))
		i++;
	while (buf[i] && !isdigit(buf[i]))
		i++;
	*col = atoi(&buf[i]);
}

void	ft_rl_cursor_setpos(rl_cursor_t *cursor)
{
	size_t	up;
	size_t	down;

	if (!cursor)
		return ;
	up = 0;
	down = 0;
	while (cursor->col > g_cols)
	{
		cursor->row++;
		cursor->col -= g_cols;
	}
	while (cursor->col < 1)
	{
		cursor->row--;
		cursor->col += g_cols;
	}
	while (cursor->row > g_rows)
	{
		up++;
		cursor->p_row--;
		cursor->i_row--;
		cursor->row--;
	}
	while (cursor->row < 1)
	{
		down++;
		cursor->p_row++;
		cursor->i_row++;
		cursor->row++;
	}
	if (up)
		ft_ti_tputs(ft_ti_tparm(g_escapes.indn, up), up, ft_rl_putc);
	if (down)
		ft_ti_tputs(ft_ti_tparm(g_escapes.rin, down), down, ft_rl_putc);
	ft_rl_cursor_move(cursor->row, cursor->col);
}

void	ft_rl_cursor_reset(rl_input_t *input)
{
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
}

void	ft_rl_cursor_move(const int16_t row, const int16_t col)
{
	if (g_escapes.cup)
		ft_ti_tputs(ft_ti_tgoto(g_escapes.cup, row - 1, col - 1), 1, ft_rl_putc);
	else
		__printf("\e[%d;%dH", row, col);
}
