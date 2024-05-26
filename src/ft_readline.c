/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 02:21:56 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"


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
	if (ptstatus != PTRAP_ENABLE)
		ft_pushtrap(PTRAP_POP);
	ft_pushtrap(ptstatus);
	tcsetattr(0, TCSANOW, &old);
	out = NULL;
	return (out);
	(void)opts;
	(void)p;
}
