/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 00:49:44 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/27 02:05:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

rl_cursor_t	*ft_rl_cursor_init(void)
{
	rl_cursor_t	*out;

	out = ft_alloc(sizeof(*out));
	if (!out)
		exit(ft_rl_perror());
	ft_rl_cursor_getpos(&out->row, &out->col);
	out->i_row = out->row;
	out->i_col = out->col;
	return (out);
}

void	ft_rl_cursor_getpos(int16_t *row, int16_t *col)
{
	size_t	i;
	char	buf[17];
	
	if (!row || !col)
		return ;
	i = 0;
	ft_memset(buf, 0, 17);
	ft_printf("%s", TERM_STATUS);
	if (read(0, buf, 16) == -1)
		exit(ft_rl_perror());
	while (buf[i] && !ft_isdigit(buf[i]))
		i++;
	*row = ft_atoi16(&buf[i]);
	while (ft_isdigit(buf[i]))
		i++;
	while (buf[i] && !ft_isdigit(buf[i]))
		i++;
	*col = ft_atoi16(&buf[i]);
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
		ft_putstr_fd(TERM_SCROLL_UP, 1);
		cursor->row--;
	}
	while (cursor->row < 1)
	{
		ft_putstr_fd(TERM_SCROLL_DOWN, 1);
		cursor->row++;
	}
	ft_printf("\e[%d;%dH", cursor->row, cursor->col);
}
