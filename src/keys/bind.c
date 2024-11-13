/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bind.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:20:28 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/13 18:24:08 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define ismacro(x)	(x && *x == '"' && strchr(x, '"'))

static inline uint8_t	_macro(const char *eseq, const char *macro);
static inline uint8_t	_bind(const char *eseq, const char *fn, rl_keytree_t *node, const rl_fninfo_t *f);
static inline uint8_t	_rebind(const char *eseq, const char *fn, rl_keytree_t *node, const rl_fninfo_t *f, const rl_mapmode_t mode);

uint8_t	ft_rl_bind(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	size_t			i;
	rl_fninfo_t		*f;
	rl_keytree_t	*tmp;
	const char		*eseq;

	errno = 0;
	eseq = NULL;
	ft_rl_init();
	f = (ismacro(fn)) ? &g_macro : __mapget(g_funcs, fn);
	if (!f)
		goto err;
	eseq = ft_rl_parse_sequence(seq);
	if (!eseq)
		goto err;
	for (i = 0, tmp = ft_rl_getcurtree(); tmp && eseq[i]; i++)
	{
		if (!tmp->next[(uint8_t)eseq[i]])
			tmp->next[(uint8_t)eseq[i]] = __push(__calloc(1, sizeof(rl_keytree_t)));
		tmp = tmp->next[(uint8_t)eseq[i]];
	}
	if (eseq[i] || tmp->c)
		goto err;
	__popblk(eseq);
	return (!tmp->fn) ? _bind(eseq, fn, tmp, f) : _rebind(eseq, fn, tmp, f, mode);
	err:
#ifndef RL42NOCOMPLAIN
	if (!f)
		__dprintf(2, "ft_rl_bind(%s, %s): function not found\n", seq, fn);
	else if (tmp->c)
		__dprintf(2, "ft_rl_bind(%s, %s): sequence already const bound\n", seq, fn);
	else if (errno)
		ft_rl_perror();
#endif
	__popblk(eseq);
	return 0;
}

uint8_t	ft_rl_bind_emacs(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(_MD_EMACS);
	rv = ft_rl_bind(seq, fn, mode);
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_bind_vi_ins(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(_MD_VI_INS);
	rv = ft_rl_bind(seq, fn, mode);
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_bind_vi_cmd(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(_MD_VI_CMD);
	rv = ft_rl_bind(seq, fn, mode);
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_bind_hlcolor(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(_MD_HLCOLOR);
	rv = ft_rl_bind(seq, fn, mode);
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_const_bind(const char *seq, const char *fn)
{
	size_t			i;
	rl_fninfo_t		*f;
	rl_keytree_t	*tmp;
	const char		*eseq;

	errno = 0;
	eseq = NULL;
	ft_rl_init();
	f = __mapget(g_funcs, fn);
	if (!f)
		goto err;
	eseq = ft_rl_parse_sequence(seq);
	if (!eseq)
		goto err;
	for (i = 0, tmp = ft_rl_getcurtree(); tmp && eseq[i]; i++)
	{
		if (!tmp->next[(uint8_t)eseq[i]])
			tmp->next[(uint8_t)eseq[i]] = __push(__calloc(1, sizeof(rl_keytree_t)));
		tmp = tmp->next[(uint8_t)eseq[i]];
	}
	if (eseq[i] || tmp->c)
		goto err;
	__popblk(eseq);
	tmp->c = 1;
	return _bind(eseq, fn, tmp, f);
	err:
#ifndef RL42NOCOMPLAIN
	if (!f)
		__dprintf(2, "ft_rl_const_bind(%s, %s): function not found\n", seq, fn);
	else if (tmp->c)
		__dprintf(2, "ft_rl_const_bind(%s, %s): sequence already const bound\n", seq, fn);
	else if (errno)
		ft_rl_perror();
#endif
	__popblk(eseq);
	return 0;
}

uint8_t	ft_rl_const_bind_emacs(const char *seq, const char *fn)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(_MD_EMACS);
	rv = ft_rl_const_bind(seq, fn);
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_const_bind_vi_ins(const char *seq, const char *fn)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(_MD_VI_INS);
	rv = ft_rl_const_bind(seq, fn);
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_const_bind_vi_cmd(const char *seq, const char *fn)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(_MD_VI_CMD);
	rv = ft_rl_const_bind(seq, fn);
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(emode);
	return rv;
}

uint8_t	ft_rl_const_bind_hlcolor(const char *seq, const char *fn)
{
	uint16_t	emode;
	uint8_t		rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(_MD_HLCOLOR);
	rv = ft_rl_const_bind(seq, fn);
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(emode);
	return rv;
}

static inline uint8_t	_macro(const char *eseq ,const char *macro)
{
	size_t	i;

	switch (ft_rl_geteditmode()) {
		case _MD_EMACS:
			i = 0;
			break ;
		case _MD_VI_INS:
			i = 1;
			break ;
		case _MD_VI_CMD:
			i = 2;
			break ;
		default:
			return 1;
	}
	__mapadd(g_macrodata[i], eseq, ft_rl_parse_sequence(__substr(macro, 1, strlen(macro + 1) - 1)));
	return 1;
}

static inline uint8_t	_bind(const char *eseq,const char *fn, rl_keytree_t *node, const rl_fninfo_t *f)
{
	size_t		i;

	node->fn = f->f;
	if (f->f == ft_rl_mcr)
		return _macro(eseq, fn);
	switch (ft_rl_geteditmode()) {
		case _MD_EMACS:
			i = 0;
			break ;
		case _MD_VI_INS:
			i = 1;
			break ;
		case _MD_VI_CMD:
			i = 2;
			break ;
		default:
			return 1;
	}
	__lstadd_back((const __t_list **)&f->binds[i], __lstnew(__strdup(eseq)));
	return 1;
}

static inline uint8_t	_rebind(const char *eseq, const char *fn, rl_keytree_t *node, const rl_fninfo_t *f, const rl_mapmode_t mode)
{
	__t_list	*tmp;
	size_t		i;

	switch (mode) {
		case WARN:
			__dprintf(2, "ft_rl_bind: keysequence %s already mapped\n", eseq);
			__attribute__((fallthrough));
		case QUIET:
			return 0;
		case REMAP:
			__dprintf(2, "ft_rl_bind: remapping keysequence %s\n", eseq);
			__attribute__((fallthrough));
		case QREMAP:
			break ;
	}
	node->fn = f->f;
	if (f->f == ft_rl_mcr)
		return _macro(eseq, fn);
	switch (ft_rl_geteditmode()) {
		case _MD_EMACS:
			i = 0;
			break ;
		case _MD_VI_INS:
			i = 1;
			break ;
		case _MD_VI_CMD:
			i = 2;
			break ;
		default:
			return 1;
	}
	for (tmp = f->binds[i]; tmp && !__strequals(tmp->blk, eseq); tmp = tmp->next)
		;
	__lstrmnode((const __t_list **)&f->binds[i], tmp);
	__lstadd_back((const __t_list **)&f->binds[i], __lstnew(__strdup(eseq)));
	return 1;
}
