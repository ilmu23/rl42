/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 00:49:44 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:50:39 by ivalimak         ###   ########.fr       */
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
	printf("%s", TERM_STATUS);
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
	if (!cursor)
		return ;
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
		__putstr_fd(TERM_SCROLL_UP, 1);
		cursor->p_row--;
		cursor->i_row--;
		cursor->row--;
	}
	while (cursor->row < 1)
	{
		__putstr_fd(TERM_SCROLL_DOWN, 1);
		cursor->p_row++;
		cursor->i_row++;
		cursor->row++;
	}
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
	printf("\e[%d;%dH", row, col);
}
