/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/14 00:27:17 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint8_t	ft_rl_mta(rl_input_t *input)
{
	uint16_t	key;

	ft_putstr_fd(TERM_CUR_SHOW, 1);
	if (read(0, &key, 1) == -1)
		exit(ft_rl_perror());
	ft_putstr_fd(TERM_CUR_HIDE, 1);
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
	ft_rl_eol(input);
	ft_rl_cursor_reset(input);
	ft_popblks(2, input->cursor, input->line);
	ft_printf("%s%s", TERM_CLEAR_END, TERM_CRNL);
	ft_rl_init_input(input->prompt, input->plen);
	ft_putstr_fd(input->prompt, 1);
	ft_rl_cursor_getpos(&input->cursor->row, &input->cursor->col);
	input->cursor->i_row = input->cursor->row;
	input->cursor->i_col = input->cursor->col;
	return (1);
}

uint8_t ft_rl_smk(rl_input_t *input)
{
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
	i = ft_min(g_mark_u.pos, input->len);
	ft_rl_setmark(_MARK_USR);
	input->i = i;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_hlc(rl_input_t *input)
{
	uint64_t	key;

	key = 0;
	ft_putstr_fd(TERM_CUR_SHOW, 1);
	if (read(0, &key, sizeof(key)) == -1)
		exit(ft_rl_perror());
	ft_putstr_fd(TERM_CUR_HIDE, 1);
	switch (key)
	{
		case KEY_NUM_0:
			g_hlcolor.sgr = SGR_FG0;
			break ;
		case KEY_NUM_1:
			g_hlcolor.sgr = SGR_FG1;
			break ;
		case KEY_NUM_2:
			g_hlcolor.sgr = SGR_FG2;
			break ;
		case KEY_NUM_3:
			g_hlcolor.sgr = SGR_FG3;
			break ;
		case KEY_NUM_4:
			g_hlcolor.sgr = SGR_FG4;
			break ;
		case KEY_NUM_5:
			g_hlcolor.sgr = SGR_FG5;
			break ;
		case KEY_NUM_6:
			g_hlcolor.sgr = SGR_FG6;
			break ;
		case KEY_NUM_7:
			g_hlcolor.sgr = SGR_FG7;
			break ;
		case KEY_DN_R:
			g_hlcolor.sgr = NULL;
			break ;
		case KEY_UP_R:
			g_hlcolor.sgr = NULL;
			break ;
	}
	return (1);
	(void)input;
}

uint8_t	ft_rl_hlm(rl_input_t *input)
{
	switch (g_hlcolor.mode)
	{
		case FT_RL_HL_FG:
			ft_rl_sethlcolor_mode(FT_RL_HL_BG);
			break ;
		case FT_RL_HL_BG:
			ft_rl_sethlcolor_mode(FT_RL_HL_FG);
			break ;
	}
	return (1);
	(void)input;
}
