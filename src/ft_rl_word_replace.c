/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_word_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 12:16:32 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 18:28:56 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline t_rl_word	**getend(void);
static inline t_rl_word	**getstart(void);

void	ft_rl_initreplace(t_rl_word *start, t_rl_word *end)
{
	*getend() = end;
	*getstart() = start;
}

void	ft_rl_replaceword(t_rl_input *input, t_rl_word *w)
{
	t_rl_word	*wl;
	t_rl_word	*tmp;
	t_rl_word	*end;

	if (!input || !w)
		return ;
	wl = w;
	while (wl->next)
		wl = wl->next;
	wl->next = *getend();
	w->prev = *getstart();
	tmp = w->prev->next;
	end = wl->next;
	while (tmp != end)
	{
		ft_popblks(2, tmp, tmp->word);
		tmp = tmp->next;
	}
	if (wl->next)
		wl->next->prev = wl;
	if (w->prev)
		w->prev->next = w;
	input->current = w;
}

void	ft_rl_endreplace(void)
{
	*getend() = NULL;
	*getstart() = NULL;
}

static inline t_rl_word	**getend(void)
{
	static t_rl_word	*end = NULL;

	return (&end);
}

static inline t_rl_word	**getstart(void)
{
	static t_rl_word	*start = NULL;

	return (&start);
}
