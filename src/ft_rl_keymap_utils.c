/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_keymap_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 10:58:41 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_ismapped(uint64_t key)
{
	t_list	*keymaps;

	keymaps = *ft_rl_getkeymaps();
	while (keymaps)
	{
		if (((t_rl_map *)keymaps->blk)->key == key)
			return (1);
		keymaps = keymaps->next;
	}
	return (0);
}

t_rl_fn	ft_rl_getmap(uint64_t key)
{
	t_list	*keymaps;

	keymaps = *ft_rl_getkeymaps();
	while (keymaps)
	{
		if (((t_rl_map *)keymaps->blk)->key == key)
			return (((t_rl_map *)keymaps->blk)->f);
		keymaps = keymaps->next;
	}
	return (NULL);
}

void	ft_rl_map_err(const char *k, const char *fn, void *ks, void *fns)
{
	if (k && !ks)
		ft_dprintf(2, "ft_readline: unrecognized key: %s\n", k);
	if (fn && !fns)
		ft_dprintf(2, "ft_readline: unrecognized function: %s\n", fn);
}

void	ft_rl_unmap(const char *key)
{
	t_list		*keys;
	t_list		*keymaps;
	uint64_t	keyval;

	keys = *ft_rl_getkeys();
	while (keys)
	{
		if (ft_strequals(((t_rl_key *)keys->blk)->name, key))
			break ;
		keys = keys->next;
	}
	if (!keys)
	{
		ft_rl_map_err(key, NULL, keys, NULL);
		return ;
	}
	keyval = ((t_rl_key *)keys->blk)->value;
	keymaps = *ft_rl_getkeymaps();
	while (keymaps)
	{
		if (((t_rl_map *)keymaps->blk)->key == keyval)
			break ;
		keymaps = keymaps->next;
	}
	ft_lstrmnode(ft_rl_getkeymaps(), keymaps);
}

void	ft_rl_unmap_fn(const char *func)
{
	t_list	*funcs;
	t_list	*keymaps;
	t_rl_fn	fn;

	funcs = *ft_rl_getfuncs();
	while (funcs)
	{
		if (ft_strequals(((t_rl_func *)funcs->blk)->name, func))
			break ;
		funcs = funcs->next;
	}
	if (!funcs)
	{
		ft_rl_map_err(NULL, func, NULL, funcs);
		return ;
	}
	fn = ((t_rl_func *)funcs->blk)->f;
	keymaps = *ft_rl_getkeymaps();
	while (keymaps)
	{
		if (((t_rl_map *)keymaps->blk)->f == fn)
			ft_lstrmnode(ft_rl_getkeymaps(), keymaps);
		keymaps = keymaps->next;
	}
}
