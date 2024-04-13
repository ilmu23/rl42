/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_history_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:07:09 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 17:35:30 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

t_list	**ft_rl_hist_gethead(void)
{
	static t_list	*current = NULL;

	return (&current);
}

t_list	**ft_rl_hist_getcurrent(void)
{
	static t_list	*current = NULL;

	return (&current);
}

t_list	*ft_rl_hist_duphist(t_list *hist)
{
	t_list	*newhist;

	if (!hist)
		return (NULL);
	newhist = NULL;
	while (hist)
	{
		ft_lstadd_front(&newhist, ft_lstnew(ft_rl_dupinput(hist->blk)));
		hist = hist->next;
	}
	return (newhist);
}

void	ft_rl_hist_setcurrent(t_list *node)
{
	*ft_rl_hist_getcurrent() = node;
}

void	ft_rl_hist_pop(t_list *hist)
{
	t_rl_input	*input;
	t_rl_word	*w;

	while (hist)
	{
		input = hist->blk;
		if (input)
		{
			w = input->head;
			while (w)
			{
				ft_popblks(2, w, w->word);
				w = w->next;
			}
			ft_popblk(input);
		}
		ft_popblk(hist);
		hist = hist->next;
	}
}
