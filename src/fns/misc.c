/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:34:31 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_mta(rl_input_t *input)
{
	uint16_t	key;

	__putstr_fd(TERM_CUR_SHOW, 1);
	if (read(0, &key, 1) == -1)
		exit(ft_rl_perror());
	__putstr_fd(TERM_CUR_HIDE, 1);
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
	input->i = input->len;
	ft_rl_cursor_reset(input);
	__popblks(2, input->cursor, input->line);
	printf("%s%s", TERM_CLEAR_END, TERM_CRNL);
	ft_rl_init_input(input->prompt, input->plen);
	__putstr_fd(input->prompt, 1);
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
		input->i = MIN(ft_rl_getarg(), input->len);
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

uint8_t	ft_rl_abt(rl_input_t *input)
{
	ft_rl_bell();
	return (1);
	(void)input;
}

uint8_t	ft_rl_hlc(rl_input_t *input)
{
	uint64_t	key;

	key = ft_rl_getkey();
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

uint8_t	ft_rl_arg(rl_input_t *input)
{
	rl_fn_t		fn;

	g_argument.set = 1;
	if (input->key >= KEY_NUM_0 && input->key <= KEY_NUM_9)
		g_argument.arg = input->key - KEY_NUM_0;
	else
	{
		input->key >>= 8;
		if (input->key >= KEY_NUM_0 && input->key <= KEY_NUM_9)
			g_argument.arg = input->key - KEY_NUM_0;
		else
			g_argument.arg = 0;
	}
	input->sprompt = __push(__itoa(g_argument.arg));
	ft_rl_redisplay(input, SPROMPT);
	input->key = ft_rl_getkey();
	while (input->key >= KEY_NUM_0 && input->key <= KEY_NUM_9)
	{
		g_argument.arg = MIN(g_argument.arg * 10 + (input->key - KEY_NUM_0), _ARG_MAX);
		input->sprompt = __push(__itoa(g_argument.arg));
		ft_rl_redisplay(input, SPROMPT);
		input->key = ft_rl_getkey();
		if (g_argument.arg == _ARG_MAX)
			break ;
	}
	fn = ft_rl_getmap(input->key);
	if (fn != ft_rl_arg && fn != ft_rl_arg_n
		&& (!(g_status & _YLA_RUNNING) || fn != ft_rl_yla)
		&& !(g_status & _VI_ARG))
		return (fn(input));
	return (1);
}

uint8_t	ft_rl_arg_n(rl_input_t *input)
{
	rl_fn_t		fn;

	g_argument.set = 1;
	g_argument.arg = 0;
	input->sprompt = __push(__strdup("-"));
	ft_rl_redisplay(input, SPROMPT);
	input->key = ft_rl_getkey();
	while (input->key >= KEY_NUM_0 && input->key <= KEY_NUM_9)
	{
		g_argument.arg = MAX(g_argument.arg * 10 - (input->key - KEY_NUM_0), _ARG_MIN);
		input->sprompt = __push(__itoa(g_argument.arg));
		ft_rl_redisplay(input, SPROMPT);
		input->key = ft_rl_getkey();
		if (g_argument.arg == _ARG_MIN)
			break ;
	}
	if (g_argument.arg == 0)
		g_argument.arg = -1;
	fn = ft_rl_getmap(input->key);
	if (fn != ft_rl_arg && fn != ft_rl_arg_n
		&& (!(g_status & _YLA_RUNNING) || fn != ft_rl_yla))
		return (fn(input));
	return (1);
}

uint8_t	ft_rl_md_em(rl_input_t *input)
{
	ft_rl_seteditmode(_MD_EMACS);
	return (1);
	(void)input;
}

uint8_t	ft_rl_md_vi(rl_input_t *input)
{
	ft_rl_seteditmode(_MD_VI_INS);
	return (1);
	(void)input;
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
	input->i = MAX(input->i - 1, 0);
	ft_rl_redisplay(input, INPUT);
	return (1);
}
