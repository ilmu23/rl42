/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/30 20:41:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

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
	input->i = input->len;
	ft_rl_cursor_reset(input);
	__popblks(2, input->cursor, input->line);
	__printf("%s%s", TERM_CLEAR_END, TERM_CRNL);
	ft_rl_init_input(input->prompt, input->plen);
	ft_ti_tputs(input->prompt, 1, ft_rl_putc);
	ft_rl_cursor_getpos(&input->cursor->row, &input->cursor->col);
	input->cursor->i_row = input->cursor->row;
	input->cursor->i_col = input->cursor->col;
	return (1);
}

uint8_t ft_rl_smk(rl_input_t *input)
{
	uint64_t	i;

	if (g_argument.set)
	{
		i = input->i;
		input->i = MIN((uint64_t)ft_rl_getarg(), input->len);
		ft_rl_setmark(_MARK_USR);
		input->i = i;
	}
	else
		ft_rl_setmark(_MARK_USR);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_umk(rl_input_t *input)
{
	ft_rl_unsetmark(_MARK_USR);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_xmk(rl_input_t *input)
{
	uint64_t	i;

	if (!g_mark_u.set)
		return (1);
	i = MIN(g_mark_u.pos, input->len);
	ft_rl_setmark(_MARK_USR);
	input->i = i;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_abt(__UNUSED rl_input_t *input)
{
	ft_rl_bell();
	return (1);
}

uint8_t	ft_rl_arg(__UNUSED rl_input_t *input) // not actually unused
{
	return (1);
}

uint8_t	ft_rl_arg_n(__UNUSED rl_input_t *input) // not actually unused
{
	return (1);
}

uint8_t	ft_rl_md_em(__UNUSED rl_input_t *input)
{
	ft_rl_seteditmode(_MD_EMACS);
	return (1);
}

uint8_t	ft_rl_md_vi(__UNUSED rl_input_t *input)
{
	ft_rl_seteditmode(_MD_VI_INS);
	return (1);
}

uint8_t	ft_rl_md_va(rl_input_t *input)
{
	ft_rl_seteditmode(_MD_VI_INS);
	input->i = MIN(input->i + 1, input->len);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_md_vI(rl_input_t *input)
{
	ft_rl_seteditmode(_MD_VI_INS);
	input->i = 0;
	ft_rl_cursor_reset(input);
	return (1);
}
uint8_t	ft_rl_md_vA(rl_input_t *input)
{
	ft_rl_seteditmode(_MD_VI_INS);
	input->i = input->len;
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_md_vc(rl_input_t *input)
{
	ft_rl_seteditmode(_MD_VI_CMD);
	input->i = MAX((int64_t)input->i - 1, 0);
	ft_rl_redisplay(input, INPUT);
	return (1);
}
