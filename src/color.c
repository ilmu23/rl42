/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:57:08 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/27 00:12:35 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_sethlcolor(const char *s)
{
	ft_popblk(g_hlcolor);
	g_hlcolor = ft_push(ft_strdup(s));
	if (!g_hlcolor)
		exit(ft_rl_perror());
}
