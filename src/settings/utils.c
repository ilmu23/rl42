/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:27:52 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/30 19:55:03 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint64_t	ft_rl_get(const uint64_t var)
{
	uint8_t	setting;

	ft_rl_init();
	setting = 0;
	switch(var)
	{
		case _BSTYLE_HASH:
			return (g_settings.bstyle);
		case _CMP_DWIDTH_HASH:
			return (g_settings.cmp_dwidth);
		case _CMP_QITEMS_HASH:
			return (g_settings.cmp_qitems);
		case _HIST_SIZE_HASH:
			return (g_settings.hist_size);
		case _KEY_TMEOUT_HASH:
			return (g_settings.keyseq_timeout);
		case _CMP_CASE_HASH:
			setting = _CMP_CASE;
			break ;
		case _CMP_MCASE_HASH:
			setting = _CMP_MCASE;
			break ;
		case _CMP_ENABLE_HASH:
			setting = _CMP_ON;
			break ;
		case _CMP_MDIRS_HASH:
			setting = _CMP_MARK_DIRS;
			break ;
		case _CMP_MLDIRS_HASH:
			setting = _CMP_MARK_LINKDIRS;
			break ;
		case _CMP_HFILES_HASH:
			setting = _CMP_HIDDEN;
			break ;
		case _CMP_HLIGHT_HASH:
			setting = _CMP_HIGHLIGHT;
			break ;
	}
	return (g_settings.completion & setting);
}

void	ft_rl_set(const char *var, const uint64_t val)
{
	uint8_t	setting;

	switch(__strhash(var, 347, UINT64_MAX))
	{
		case _BSTYLE_HASH:
			if (val <= BELL_AUDIBLE)
				g_settings.bstyle = val;
			return ;
		case _CMP_DWIDTH_HASH:
			g_settings.cmp_dwidth = val;
			return ;
		case _CMP_QITEMS_HASH:
			g_settings.cmp_qitems = val;
			return ;
		case _HIST_SIZE_HASH:
			g_settings.hist_size = val;
			return ;
		case _KEY_TMEOUT_HASH:
			g_settings.keyseq_timeout = val;
			return ;
		case _CMP_CASE_HASH:
			setting = _CMP_CASE;
			break ;
		case _CMP_MCASE_HASH:
			setting = _CMP_MCASE;
			break ;
		case _CMP_ENABLE_HASH:
			setting = _CMP_ON;
			break ;
		case _CMP_MDIRS_HASH:
			setting = _CMP_MARK_DIRS;
			break ;
		case _CMP_MLDIRS_HASH:
			setting = _CMP_MARK_LINKDIRS;
			break ;
		case _CMP_HFILES_HASH:
			setting = _CMP_HIDDEN;
			break ;
		case _CMP_HLIGHT_HASH:
			setting = _CMP_HIGHLIGHT;
			break ;
		default:
#ifndef RL42NOCOMPLAIN
			if (g_status & _READ_INITFILE)
				__dprintf(2, "rl42: init: set: unrecognized variable '%s'\n", var);
			else
				__dprintf(2, "rl42: ft_rl_set: unrecognized variable '%s'\n", var);
#endif
			return ;
	}
	switch (val)
	{
		case SET_ON:
			if (!(g_settings.completion & setting))
				g_settings.completion ^= setting;
			break ;
		case SET_OFF:
			if (g_settings.completion & setting)
				g_settings.completion ^= setting;
	}
}
