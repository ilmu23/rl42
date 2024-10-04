/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keymap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/04 18:43:32 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline rl_map_t	*_newmapping(const uint64_t *key, const rl_fn_t *func);
static inline t_hmap	*_getcurmap(void);
static inline void		_err(const uint64_t *key, const rl_fn_t *func);
static inline void		_remap(const char *key, const rl_map_t *mapping, rl_mapmode_t mode);

void	ft_rl_map(const char *key, const char *func, rl_mapmode_t mode)
{
	const uint64_t	*keyval;
	const rl_fn_t	*funcval;

	keyval = __mapget(g_keys, key);
	funcval = __mapget(g_funcs, func);
	if (!keyval || !funcval)
	{
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "ft_rl_map(%s, %s): ", key, func);
		_err(keyval, funcval);
#endif
		return ;
	}
	if (ft_rl_ismapped(*keyval))
		_remap(key, _newmapping(keyval, funcval), mode);
	else
		__mapadd(_getcurmap(), key, _newmapping(keyval, funcval));
}

void	ft_rl_map_emacs(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(_MD_EMACS);
	ft_rl_map(key, func, mode);
	if (emode != _MD_EMACS)
		ft_rl_seteditmode(emode);
}

void	ft_rl_map_vi_ins(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(_MD_VI_INS);
	ft_rl_map(key, func, mode);
	if (emode != _MD_VI_INS)
		ft_rl_seteditmode(emode);
}

void	ft_rl_map_vi_cmd(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(_MD_VI_CMD);
	ft_rl_map(key, func, mode);
	if (emode != _MD_VI_CMD)
		ft_rl_seteditmode(emode);
}

void	ft_rl_map_hlcolor(const char *key, const char *func, rl_mapmode_t mode)
{
	uint8_t	emode;

	emode = ft_rl_geteditmode();
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(_MD_HLCOLOR);
	ft_rl_map(key, func, mode);
	if (emode != _MD_HLCOLOR)
		ft_rl_seteditmode(emode);
}

void	ft_rl_addkey(const char *key, const uint64_t value)
{
	uint64_t	*val;

	val = __push(__alloc(sizeof(*val)));
	if (!val)
		exit(ft_rl_perror());
	*val = value;
	__mapadd(g_keys, key, val);
	__popblk(val);
}

void	ft_rl_addfunc(const char *func, const rl_fn_t f)
{
	rl_fn_t	*fn;

	fn = __push(__alloc(sizeof(*fn)));
	if (!fn)
		exit(ft_rl_perror());
	*fn = f;
	__mapadd(g_funcs, func, fn);
	__popblk(fn);
}

static inline rl_map_t	*_newmapping(const uint64_t *key, const rl_fn_t *func)
{
	rl_map_t	*out;

	out = __alloc(sizeof(*out));
	if (!out)
		exit(ft_rl_perror());
	return (memcpy(out, &(rl_map_t){.key = *key, .f = *func}, sizeof(*out)));
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
		case _MD_HLCOLOR:
			return (g_map_hlcolor);
	}
	return (NULL);
}

static inline void	_err(const uint64_t *key, const rl_fn_t *func)
{
	if (!key)
	{
		__putstr_fd("key not found", 2);
		if (!func)
			__putstr_fd("; ", 2);
	}
	if (!func)
		__putstr_fd("function not found", 2);
	__putchar_fd('\n', 2);
}

static inline void	_remap(const char *key, const rl_map_t *mapping, rl_mapmode_t mode)
{
	switch (mode)
	{
		case WARN:
			__dprintf(2, "ft_rl_map: key %s already mapped\n", key);
			return ;
		case QUIET:
			return ;
		case REMAP:
			__dprintf(2, "ft_rl_map: remapping key %s\n", key);
			break ;
		case QREMAP:
			break ;
	}
	__mapadd(_getcurmap(), key, mapping);
}
