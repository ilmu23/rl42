/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_complete_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:43:29 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 19:12:07 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

size_t	ft_rl_complete_getlongest(t_list *completions)
{
	size_t	llen;
	size_t	len;

	llen = 0;
	while (completions)
	{
		len = ft_strlen(ft_rl_complete_basename(completions->blk));
		len += (ft_strchr(completions->blk, ' ') != NULL) * 2;
		if (len > llen)
			llen = len;
		completions = completions->next;
		if (completions && !completions->prev)
			break ;
	}
	return (llen);
}

char	*ft_rl_complete_basename(const char *s)
{
	const char	*tmp;

	tmp = ft_strchr(s, '/');
	while (tmp && tmp[1])
	{
		s = tmp + 1;
		tmp = ft_strchr(s, '/');
	}
	return (ft_strdup(s));
}

void	ft_rl_putcompletion(t_list *st, t_list *c, const char *s, size_t m)
{
	if (ft_strchr(s, ' '))
		s = ft_strnjoin(3, "'", s, "'");
	if (!s)
		return ;
	if (st != c)
		ft_printf("%-*s", m, ft_push(s));
	else
		ft_printf("%s%s%s%*s", ft_rl_gethlcolor(), ft_push(s), SGR_RESET,
			m - ft_strlen(s), "");
	ft_popblk(s);
}
