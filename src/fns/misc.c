/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/08 18:44:13 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_globals.h"
#include "ft_rl_internal.h"

#define getfn	(f = ft_rl_getinput(&input->keyseq))
#define dargfn	(f == ft_rl_ins || (ft_rl_geteditmode() == _MD_VI_CMD && f == ft_rl_arg))

#define inrange(x, y, z)	(x >= y && x <= z)

uint8_t	ft_rl_rri(rl_input_t *input)
{
	uint64_t	i;

	i = input->i;
	ft_rl_cursor_reset(input);
	ft_ti_tputs(TERM_CRNL, 1, ft_rl_putc);
	ft_rl_read_initfile();
	input->i = i;
	ft_rl_cursor_reset(input);
	return 1;
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

uint8_t	ft_rl_arg(__UNUSED rl_input_t *input)
{
	uint8_t	offset;
	rl_fn_t	f;

	offset = (input->keyseq[0] == '\e') ? 1 : 0;
	if (inrange(input->keyseq[offset], '0', '9') && input->keyseq[offset + 1] == '\0')
		g_argument.arg = input->keyseq[offset] - '0';
	else
		g_argument.arg = 0;
	g_argument.set = 1;
	input->sprompt = __push(__itoa_base(g_argument.arg, DECIMAL));
	ft_rl_redisplay(input, SPROMPT);
	for (getfn; g_argument.arg != _ARG_MAX && dargfn && inrange(*input->keyseq, '0', '9'); getfn) {
		g_argument.arg = MIN(g_argument.arg * 10 + *input->keyseq - '0', _ARG_MAX);
		__popblk(input->sprompt);
		input->sprompt = __push(__itoa_base(g_argument.arg, DECIMAL));
		ft_rl_redisplay(input, SPROMPT);
	}
	ft_rl_redisplay(input, PROMPT);
	if (f != ft_rl_arg && f != ft_rl_arg_n && !(g_status & _VI_ARG)
		&& (!(g_status & _YLA_RUNNING) || f != ft_rl_yla))
		return (f(input));
	return (1);
}

uint8_t	ft_rl_arg_n(__UNUSED rl_input_t *input)
{
	rl_fn_t	f;

	g_argument.arg = 0;
	g_argument.set = 1;
	input->sprompt = __push(__itoa_base(g_argument.arg, DECIMAL));
	ft_rl_redisplay(input, SPROMPT);
	for (getfn; g_argument.arg != _ARG_MIN && f == ft_rl_ins && inrange(*input->keyseq, '0', '9'); getfn) {
		g_argument.arg = MAX((int32_t)(g_argument.arg * 10 - (*input->keyseq - '0')), _ARG_MIN);
		input->sprompt = __push(__itoa_base(g_argument.arg, DECIMAL));
		ft_rl_redisplay(input, SPROMPT);
	}
	g_argument.arg = (g_argument.arg != 0) ? g_argument.arg : -1;
	ft_rl_redisplay(input, PROMPT);
	if (f != ft_rl_arg && f != ft_rl_arg_n
		&& (!(g_status & _YLA_RUNNING) || f != ft_rl_yla))
		return (f(input));
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
