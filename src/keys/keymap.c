/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keymap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/07 19:34:10 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

static inline rl_map_t	*_newmapping(const uint64_t *key, const rl_fn_t *func);
static inline t_hmap	*_getcurmap(void);
static inline void		_err(const uint64_t *key, const rl_fn_t *func);
static inline void		_remap(const char *key, const rl_map_t *mapping, rl_mapmode_t mode);

void	ft_rl_map(const char *key, const char *func, rl_mapmode_t mode)
{
	const uint64_t	*keyval;
	const rl_fn_t	*funcval;

	keyval = ft_mapget(g_keys, key);
	funcval = ft_mapget(g_funcs, func);
	if (!keyval || !funcval)
	{
		ft_dprintf(2, "ft_rl_map(%s, %s): ", key, func);
		_err(keyval, funcval);
		return ;
	}
	if (ft_rl_ismapped(*keyval))
		_remap(key, _newmapping(keyval, funcval), mode);
	else
		ft_mapadd(_getcurmap(), key, _newmapping(keyval, funcval));
}

void	ft_rl_map_emacs(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(_MD_EMACS);;
	ft_rl_map(key, func, mode);
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(emode);;
}

void	ft_rl_map_vi_ins(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(_MD_VI_INS);;
	ft_rl_map(key, func, mode);
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(emode);;
}

void	ft_rl_map_vi_cmd(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(_MD_VI_CMD);;
	ft_rl_map(key, func, mode);
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(emode);;
}

void	ft_rl_addkey(const char *key, const uint64_t value)
{
	uint64_t	*val;

	val = ft_push(ft_alloc(sizeof(*val)));
	if (!val)
		exit(ft_rl_perror());
	*val = value;
	ft_mapadd(g_keys, key, val);
	ft_popblk(val);
}

void	ft_rl_addfunc(const char *func, const rl_fn_t f)
{
	rl_fn_t	*fn;

	fn = ft_push(ft_alloc(sizeof(*fn)));
	if (!fn)
		exit(ft_rl_perror());
	*fn = f;
	ft_mapadd(g_funcs, func, fn);
	ft_popblk(fn);
}

static inline rl_map_t	*_newmapping(const uint64_t *key, const rl_fn_t *func)
{
	rl_map_t	*out;

	out = ft_alloc(sizeof(*out));
	if (!out)
		exit(ft_rl_perror());
	return (ft_memcpy(out, &(rl_map_t){.key = *key, .f = *func}, sizeof(*out)));
}

static inline t_hmap	*_getcurmap(void)
{
	switch (ft_rl_geteditmode())
	{
		case _MD_EMACS:
			return (g_map_emacs);
		case _MD_VI_INS:
			return (g_map_vi_ins);
		case _MD_VI_CMD:
			return (g_map_vi_cmd);
	}
	return (NULL);
}

static inline void	_err(const uint64_t *key, const rl_fn_t *func)
{
	if (!key)
	{
		ft_dprintf(2, "key not found");
		if (!func)
			ft_dprintf(2, "; ");
	}
	if (!func)
		ft_dprintf(2, "function not found");
	ft_dprintf(2, "\n");
}

static inline void	_remap(const char *key, const rl_map_t *mapping, rl_mapmode_t mode)
{
	switch (mode)
	{
		case WARN:
			ft_dprintf(2, "ft_rl_map: key %s already mapped\n", key);
			return ;
		case QUIET:
			return ;
		case REMAP:
			ft_dprintf(2, "ft_rl_map: remapping key %s\n", key);
			break ;
		case QREMAP:
			break ;
	}
	ft_mapadd(_getcurmap(), key, mapping);
}
