/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_keymap_lists.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:25:16 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 15:47:28 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

t_list	**ft_rl_getkeys(void)
{
	static t_list	*keys = NULL;

	return (&keys);
}

t_list	**ft_rl_getfuncs(void)
{
	static t_list	*funcs = NULL;

	return (&funcs);
}

t_list	**ft_rl_getkeymaps(void)
{
	static t_list	*keymaps = NULL;

	return (&keymaps);
}
