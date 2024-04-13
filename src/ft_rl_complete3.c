/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_complete3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:23:27 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/10 13:43:06 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	compare(t_list *completions, char *common);

void	ft_rl_complete_inc(t_list *completions, const char *pattern)
{
	char	*common;
	size_t	maxlen;

	maxlen = ft_rl_complete_getlongest(completions);
	common = ft_calloc(maxlen + 1, sizeof(*common));
	if (!common)
		return ;
	compare(completions, common);
	if (!*common || ft_strequals(common, pattern))
		return ;
	ft_popblk(completions->blk);
	completions->blk = ft_push(common);
	ft_lstpopall(completions->next);
	completions->next = NULL;
}

static inline void	compare(t_list *completions, char *common)
{
	t_list	*start;
	uint8_t	current;
	size_t	i;

	i = 0;
	start = completions;
	current = ((uint8_t *)completions->blk)[i];
	while (current)
	{
		completions = completions->next;
		if (!completions)
		{
			common[i++] = current;
			completions = start;
			current = ((uint8_t *)completions->blk)[i];
			continue ;
		}
		if (((uint8_t *)completions->blk)[i] == current)
			continue ;
		break ;
	}
}
