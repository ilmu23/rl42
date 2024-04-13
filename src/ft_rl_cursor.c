/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_cursor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:20:40 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 15:37:21 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_shiftcursor(size_t n, uint64_t direction)
{
	t_rl_cursor	*cursor;

	cursor = ft_rl_getcursor(NULL);
	while (n--)
	{
		if (direction == KEY_LEFT)
			cursor->col--;
		else
			cursor->col++;
	}
	ft_rl_updatecursor(cursor);
}
