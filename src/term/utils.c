/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 02:12:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:50:57 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_updatetermsize(void)
{
	int16_t	row;
	int16_t	col;

	ft_rl_cursor_getpos(&row, &col);
	ft_rl_cursor_move(INT16_MAX, INT16_MAX);
	ft_rl_cursor_getpos(&g_rows, &g_cols);
	ft_rl_cursor_move(row, col);
}
