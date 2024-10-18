/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kring.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:57:34 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

char	*ft_rl_kring_yank(void)
{
	return ((g_kill_ring) ? (char *)g_kill_ring->blk : NULL);
}

void	ft_rl_kring_rotate(void)
{
	const __t_list	*fnode;

	if (!g_kill_ring)
		return ;
	fnode = g_kill_ring;
	__lstrmnode(&g_kill_ring, fnode);
	__lstadd_back(&g_kill_ring, (__t_list *)fnode);
}
