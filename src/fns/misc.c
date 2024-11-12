/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:16:59 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/12 16:14:41 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_globals.h"
#include "ft_rl_internal.h"

#define _FMT_NORMAL 0
#define _FMT_INITFILE 1

#define getfn	(f = ft_rl_getinput(&input->keyseq))
#define dargfn	(f == ft_rl_ins || (ft_rl_geteditmode() == _MD_VI_CMD && f == ft_rl_arg))

#define inrange(x, y, z)	(x >= y && x <= z)

static inline void	_print(const char *s);

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

uint8_t	ft_rl_dfn(rl_input_t *input)
{
	size_t		i;
	size_t		j;
	size_t		k;
	uint8_t		fmt;
	uint16_t	emode;
	__t_list	*tmp;
	rl_fninfo_t	*f;

	emode = ft_rl_geteditmode();
	switch (emode) {
		case _MD_EMACS:
			k = 0;
			break ;
		case _MD_VI_INS:
			k = 1;
			break ;
		case _MD_VI_CMD:
			k = 2;
			break ;
		default:
			return 1;
	}
	ft_ti_tputs(TERM_CRNL, 1, ft_rl_putc);
	fmt = (g_argument.set) ? _FMT_INITFILE : _FMT_NORMAL;
	if (_FMT_INITFILE)
		ft_rl_getarg();
	for (i = 0; i < _FNCOUNT; i++) {
		f = __mapget(g_funcs, g_fn_names[i]);
		if (fmt == _FMT_NORMAL)
			__printf("%s %s", f->name, (f->binds[k]) ? "is bound to " : "is not bound" TERM_CRNL);
		for (j = 0, tmp = f->binds[k]; tmp; j++, tmp = tmp->next) {
			if (fmt == _FMT_NORMAL) {
				_print(tmp->blk);
				if (j == 5) {
					ft_ti_tputs("..." TERM_CRNL, 1, ft_rl_putc);
					break ;
				}
				ft_ti_tputs((tmp->next) ? ", " : "." TERM_CRNL, 1, ft_rl_putc);
			} else {
				__printf("bind\t%s\t%s" TERM_CRNL, tmp->blk, f->name);
			}
		}
	}
	ft_rl_cursor_getpos(&input->cursor->p_row, &input->cursor->p_col);
	ft_ti_tputs(input->prompt, 1, ft_rl_putc);
	ft_rl_cursor_getpos(&input->cursor->i_row, &input->cursor->i_col);
	ft_rl_redisplay(input, INPUT);
	return 1;
}

#define _pvar(var, val, type, fmt)	((fmt == _FMT_INITFILE) \
		? __printf("set\t%s\t" type TERM_CRNL, var, val) \
		: __printf("%s is set to '" type "'" TERM_CRNL, var, val))

uint8_t	ft_rl_dvr(rl_input_t *input)
{
	const char	*tmp;
	uint64_t	val;
	__UNUSED uint8_t		fmt;

	ft_ti_tputs(TERM_CRNL, 1, ft_rl_putc);
	fmt = (g_argument.set) ? _FMT_INITFILE : _FMT_NORMAL;
	if (fmt == _FMT_INITFILE)
		ft_rl_getarg();
	val = ft_rl_get(_BSTYLE_HASH);
	tmp = (val == BELL_NONE) ? "" : (val == BELL_AUDIBLE) ? "audible" : "visible";
	_pvar("bell-style", tmp, "%s", fmt);
	_pvar("completion-display-width", ft_rl_get(_CMP_DWIDTH_HASH), "%hd", fmt);
	_pvar("completion-ignore-case", ((ft_rl_get(_CMP_CASE_HASH)) ? "on" : "off"), "%s", fmt);
	_pvar("completion-map-case", ((ft_rl_get(_CMP_MCASE_HASH)) ? "on" : "off"), "%s", fmt);
	_pvar("completion-query-items", ft_rl_get(_CMP_QITEMS_HASH), "%lld", fmt);
	val = ft_rl_geteditmode();
	switch (val) {
		case _MD_EMACS:
			tmp = "emacs";
			break ;
		case _MD_VI_INS:
			tmp = "emacs";
			break ;
		case _MD_VI_CMD:
			tmp = "emacs";
			break ;
		default:
			tmp = NULL;
	}
	if (tmp)
		_pvar("editing-mode", tmp, "%s", fmt);
	_pvar("enable-completion", ((ft_rl_get(_CMP_ENABLE_HASH)) ? "on" : "off"), "%s", fmt);
	_pvar("history-size", ft_rl_get(_HIST_SIZE_HASH), "%lld", fmt);
	_pvar("keyseq-timeout", ft_rl_get(_KEY_TMEOUT_HASH), "%lld", fmt);
	_pvar("mark-directories", ((ft_rl_get(_CMP_MDIRS_HASH)) ? "on" : "off"), "%s", fmt);
	_pvar("mark-symlinked-directories", ((ft_rl_get(_CMP_MLDIRS_HASH)) ? "on" : "off"), "%s", fmt);
	_pvar("match-hidden-files", ((ft_rl_get(_CMP_HFILES_HASH)) ? "on" : "off"), "%s", fmt);
	_pvar("highlight-current-completion", ((ft_rl_get(_CMP_HLIGHT_HASH)) ? "on" : "off"), "%s", fmt);
	ft_rl_cursor_getpos(&input->cursor->p_row, &input->cursor->p_col);
	ft_ti_tputs(input->prompt, 1, ft_rl_putc);
	ft_rl_cursor_getpos(&input->cursor->i_row, &input->cursor->i_col);
	ft_rl_redisplay(input, INPUT);
	return 1;
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

static inline void	_print(const char *s)
{
	for (; *s; s++) {
		if (!isspace(*s) && isprint(*s))
			ft_rl_putc(*s);
		else switch (*s) {
			case '\e':
				ft_ti_tputs("<ESC>", 1, ft_rl_putc);
				break ;
			case '\t':
				ft_ti_tputs("<TAB>", 1, ft_rl_putc);
				break ;
			case '\n':
				ft_ti_tputs("<ENT>", 1, ft_rl_putc);
				break ;
			case '\r':
				ft_ti_tputs("<RET>", 1, ft_rl_putc);
				break ;
			case ' ':
				ft_ti_tputs("<SPC>", 1, ft_rl_putc);
				break ;
			case 0:
				ft_ti_tputs("<C-@>", 1, ft_rl_putc);
				break ;
			case 1:
				ft_ti_tputs("<C-a>", 1, ft_rl_putc);
				break ;
			case 2:
				ft_ti_tputs("<C-b>", 1, ft_rl_putc);
				break ;
			case 3:
				ft_ti_tputs("<C-c>", 1, ft_rl_putc);
				break ;
			case 4:
				ft_ti_tputs("<C-d>", 1, ft_rl_putc);
				break ;
			case 5:
				ft_ti_tputs("<C-e>", 1, ft_rl_putc);
				break ;
			case 6:
				ft_ti_tputs("<C-f>", 1, ft_rl_putc);
				break ;
			case 7:
				ft_ti_tputs("<C-g>", 1, ft_rl_putc);
				break ;
			case 8:
				ft_ti_tputs("<C-h>", 1, ft_rl_putc);
				break ;
			case 11:
				ft_ti_tputs("<C-k>", 1, ft_rl_putc);
				break ;
			case 12:
				ft_ti_tputs("<C-l>", 1, ft_rl_putc);
				break ;
			case 14:
				ft_ti_tputs("<C-n>", 1, ft_rl_putc);
				break ;
			case 15:
				ft_ti_tputs("<C-o>", 1, ft_rl_putc);
				break ;
			case 16:
				ft_ti_tputs("<C-p>", 1, ft_rl_putc);
				break ;
			case 17:
				ft_ti_tputs("<C-q>", 1, ft_rl_putc);
				break ;
			case 18:
				ft_ti_tputs("<C-r>", 1, ft_rl_putc);
				break ;
			case 19:
				ft_ti_tputs("<C-s>", 1, ft_rl_putc);
				break ;
			case 20:
				ft_ti_tputs("<C-t>", 1, ft_rl_putc);
				break ;
			case 21:
				ft_ti_tputs("<C-u>", 1, ft_rl_putc);
				break ;
			case 22:
				ft_ti_tputs("<C-v>", 1, ft_rl_putc);
				break ;
			case 23:
				ft_ti_tputs("<C-w>", 1, ft_rl_putc);
				break ;
			case 24:
				ft_ti_tputs("<C-x>", 1, ft_rl_putc);
				break ;
			case 25:
				ft_ti_tputs("<C-y>", 1, ft_rl_putc);
				break ;
			case 26:
				ft_ti_tputs("<C-z>", 1, ft_rl_putc);
				break ;
			case 28:
				ft_ti_tputs("<C-\\>", 1, ft_rl_putc);
				break ;
			case 29:
				ft_ti_tputs("<C-]>", 1, ft_rl_putc);
				break ;
			case 30:
				ft_ti_tputs("<C-^>", 1, ft_rl_putc);
				break ;
			case 31:
				ft_ti_tputs("<C-_>", 1, ft_rl_putc);
				break ;
		}
	}
}
