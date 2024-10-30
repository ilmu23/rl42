/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bind.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:20:28 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/30 18:49:33 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline uint8_t	_bind(rl_keytree_t *node, const rl_fn_t *f);
static inline uint8_t	_rebind(const char *seq, rl_keytree_t *node, const rl_fn_t *f, const rl_mapmode_t mode);

uint8_t	ft_rl_bind(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	size_t			i;
	rl_fn_t			*f;
	rl_keytree_t	*tmp;
	const char		*eseq;

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
	if (eseq[i])
		goto err;
	return (!tmp->fn) ? _bind(tmp, f) : _rebind(seq, tmp, f, mode);
	err:
#ifndef RL42NOCOMPLAIN
	if (!f)
		__dprintf(2, "ft_rl_map(%s, %s): function not found\n", seq, fn);
	else
		ft_rl_perror();
#endif
	__popblk(eseq);
	return 0;
}

uint8_t	ft_rl_bind_emacs(const char *seq, const char *fn, const rl_mapmode_t mode)
{
	uint8_t	emode;
	uint8_t	rv;

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
	uint8_t	emode;
	uint8_t	rv;

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
	uint8_t	emode;
	uint8_t	rv;

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
	uint8_t	emode;
	uint8_t	rv;

	emode = ft_rl_geteditmode();
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(_MD_HLCOLOR);
	rv = ft_rl_bind(seq, fn, mode);
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(emode);
	return rv;
}

static inline uint8_t	_bind(rl_keytree_t *node, const rl_fn_t *f)
{
	node->fn = *f;
	return 1;
}

static inline uint8_t	_rebind(const char *seq, rl_keytree_t *node, const rl_fn_t *f, const rl_mapmode_t mode)
{
	switch (mode)
	{
		case WARN:
			__dprintf(2, "ft_rl_map: keysequence %s already mapped\n", seq);
		case QUIET:
			return 0;
		case REMAP:
			__dprintf(2, "ft_rl_map: remapping keysequence %s\n", seq);
		case QREMAP:
			break ;
	}
	node->fn = *f;
	return 1;
}
