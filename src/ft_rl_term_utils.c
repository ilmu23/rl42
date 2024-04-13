/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_term_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:17:14 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/12 15:23:45 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	initcursor(t_rl_cursor *cursor, size_t plen);
static inline void	getcurpos(int16_t *row, int16_t *col);

t_rl_cursor	*ft_rl_getcursor(t_rl_input *input)
{
	static t_rl_cursor	cursor;

	if (input)
		initcursor(&cursor, input->plen);
	return (&cursor);
}

void	ft_rl_inputcursor(t_rl_input *input)
{
	if (!input || !input->cursor)
		return ;
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col;
	ft_rl_updatecursor(input->cursor);
}

void	ft_rl_resetcursor(t_rl_input *input)
{
	t_rl_word	*w;
	int16_t		n;

	if (!input || !input->cursor)
		return ;
	n = 0;
	w = input->head;
	while (w && w->i)
	{
		n += w->i;
		w = w->next;
	}
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + n;
	ft_rl_updatecursor(input->cursor);
}

void	ft_rl_updatecursor(t_rl_cursor *cursor)
{
	if (!cursor)
		return ;
	while (cursor->col > cursor->t_cols)
	{
		cursor->row++;
		cursor->col -= cursor->t_cols;
	}
	while (cursor->col < 1)
	{
		cursor->row--;
		cursor->col += cursor->t_cols;
	}
	while (cursor->row > cursor->t_rows && cursor->i_row > 1)
	{
		ft_putstr_fd(TERM_SCROLL_UP, 1);
		cursor->i_row--;
		cursor->row--;
	}
	if ((cursor->row == cursor->i_row && cursor->col < cursor->i_col)
		|| cursor->row < cursor->i_row)
	{
		cursor->row = cursor->i_row;
		cursor->col = cursor->i_col;
	}
	ft_printf("\e[%d;%dH", cursor->row, cursor->col);
}

static inline void	initcursor(t_rl_cursor *cursor, size_t plen)
{
	if (!cursor)
		return ;
	getcurpos(&cursor->row, &cursor->col);
	cursor->i_row = cursor->row;
	cursor->i_col = cursor->col + plen;
	ft_printf("\e[99999;99999H");
	getcurpos(&cursor->t_rows, &cursor->t_cols);
	ft_printf("\e[%d;%dH", cursor->row, cursor->col);
}

static inline void	getcurpos(int16_t *row, int16_t *col)
{
	size_t	i;
	char	buf[17];

	i = 0;
	ft_bzero(buf, 17);
	ft_putstr_fd(TERM_STATUS, 1);
	read(0, buf, 16);
	while (buf[i] && !ft_isdigit(buf[i]))
		i++;
	*row = ft_atoi16(&buf[i]);
	while (ft_isdigit(buf[i++]))
		;
	while (buf[i] && !ft_isdigit(buf[i]))
		i++;
	*col = ft_atoi16(&buf[i]);
}
