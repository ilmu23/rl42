/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 19:55:23 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

/** globals **/
t_hmap	*g_keys;
t_hmap	*g_funcs;
t_hmap	*g_maps;

const t_list	*g_hist;
const t_list	*g_hist_cur;
/** globals **/

char	*ft_readline(const char *p, uint8_t opts)
{
	struct termios	old;
	struct termios	new;
	char			*out;
	uint8_t			ptstatus;

	ft_rl_init();
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(0, TCSANOW, &new);
	ptstatus = ft_pushtrap_status();
	ft_pushtrap(PTRAP_ENABLE);
	g_hist_cur = NULL;
	if (!(opts & FT_RL_HIST_OFF))
		g_hist_cur = g_hist;
	if (ptstatus != PTRAP_ENABLE)
		ft_pushtrap(PTRAP_POP);
	ft_pushtrap(ptstatus);
	tcsetattr(0, TCSANOW, &old);
	out = NULL;
	return (out);
	(void)opts;
	(void)p;
}
