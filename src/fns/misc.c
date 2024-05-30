/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 07:47:50 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_mta(rl_input_t *input)
{
	uint16_t	key;

	if (read(0, &key, 1) == -1)
		exit(ft_rl_perror());
	key = key << 8 | '\e';
	input->keystr = ft_rl_keystr(key);
	return (ft_rl_execmap(input));
}

uint8_t	ft_rl_dcl(rl_input_t *input)
{
	input = &g_input;
	g_hist_cur = g_hist;
	if (g_hist_cur)
	{
		ft_rl_hist_restore();
		ft_rl_hist_rmnode();
		ft_rl_hist_newnode();
		g_hist_cur = g_hist;
	}
	ft_popblks(2, input->cursor, input->line);
	ft_putstr_fd(TERM_CRNL, 1);
	ft_rl_init_input(input->prompt, input->plen);
	ft_putstr_fd(input->prompt, 1);
	ft_rl_cursor_getpos(&input->cursor->row, &input->cursor->col);
	input->cursor->i_row = input->cursor->row;
	input->cursor->i_col = input->cursor->col;
	return (1);
}

uint8_t	ft_rl_hlc(rl_input_t *input)
{
	return (1);
	(void)input;
}
