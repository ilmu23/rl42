/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_wildcard_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:36:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 21:52:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_wc_checkalloc(t_rl_wc *cur, t_rl_wc *prv, char **patterns)
{
	if (!cur || !patterns)
	{
		ft_popblks(5, cur, prv, cur->pattern, prv->pattern, patterns);
		if (patterns)
		{
			while (*patterns)
				ft_popblk(patterns++);
		}
		return (0);
	}
	return (1);
}

void	ft_rl_wc_rmdot(t_list *matches)
{
	while (matches)
	{
		ft_strlcpy(matches->blk, matches->blk + 2, ft_strlen(matches->blk) - 1);
		matches = matches->next;
	}
}
