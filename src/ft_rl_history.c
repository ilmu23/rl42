/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:08:53 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 09:12:33 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	commit_replace(t_list **hist, t_rl_input *input);

t_rl_input	*ft_rl_hist_getnext(t_rl_input *input, uint8_t cpy)
{
	t_list	*current;

	current = *ft_rl_hist_getcurrent();
	if (!current || !current->blk)
		return (NULL);
	if (!current->prev)
		return (input);
	if (cpy)
	{
		((t_rl_input *)current->blk)->cursor = input->cursor;
		ft_rl_updateinput(current->blk, input);
	}
	current = current->prev;
	ft_rl_hist_setcurrent(current);
	return (current->blk);
}

t_rl_input	*ft_rl_hist_getprev(t_rl_input *input, uint8_t cpy)
{
	t_list	*current;

	current = *ft_rl_hist_getcurrent();
	if (!current || !current->blk)
		return (NULL);
	if (!current->next)
		return (input);
	if (cpy)
	{
		((t_rl_input *)current->blk)->cursor = input->cursor;
		ft_rl_updateinput(current->blk, input);
	}
	current = current->next;
	ft_rl_hist_setcurrent(current);
	return (current->blk);
}

void	ft_rl_hist_commit(t_rl_input *input)
{
	t_rl_word	*w;
	t_list		**hist;

	if (!input || !input->head)
		return ;
	w = input->head;
	while (w)
	{
		w->i = w->len;
		w = w->next;
	}
	hist = ft_rl_hist_gethead();
	if (!*hist || *(*hist)->size < RL_HISTORY_SIZE)
		ft_lstadd_back(hist, ft_lstnew(ft_rl_dupinput(input)));
	else
		commit_replace(hist, input);
}

void	ft_rl_hist_add(t_list **hist, t_rl_input *input)
{
	t_list	*tmp;

	if (!input)
		return ;
	if (!*hist || *(*hist)->size < RL_HISTORY_SIZE)
		ft_lstadd_front(hist, ft_lstnew(input));
	else
	{
		tmp = ft_lstlast(*hist);
		if (tmp->prev)
			tmp->prev->next = NULL;
		ft_rl_popwords(tmp->blk);
		ft_popblk(tmp->blk);
		*tmp = (t_list){.next = *hist, .prev = NULL,
			.size = tmp->size, .blk = input};
		*hist = tmp;
	}
}

static inline void	commit_replace(t_list **hist, t_rl_input *input)
{
	t_list	*tmp;
	t_list	*last;

	tmp = *hist;
	last = ft_lstlast(tmp);
	if (tmp->next)
		tmp->next->prev = NULL;
	ft_rl_popwords(tmp->blk);
	ft_popblk(tmp->blk);
	*tmp = (t_list){.next = NULL, .prev = last,
		.size = tmp->size, .blk = input};
	*hist = tmp->next;
	last->next = tmp;
}
