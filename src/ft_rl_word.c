/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:35 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 15:36:10 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

t_rl_word	*ft_rl_splitword(t_rl_word *w1)
{
	char		*s1;
	char		*s2;
	t_rl_word	*w2;

	s1 = ft_push(ft_substr(w1->word, 0, w1->i));
	s2 = ft_push(ft_substr(w1->word, w1->i, w1->len - w1->i));
	w2 = ft_alloc(sizeof(*w2));
	if (!s1 || !s2 || !w2)
	{
		ft_popblks(2, s1, s2);
		return (w1);
	}
	*w2 = (t_rl_word){.i = 0, .len = w1->len - w1->i, .word = s2,
		.wtype = w1->wtype, .prev = w1, .next = w1->next};
	w1->len = w1->i;
	w1->next = ft_push(w2);
	ft_popblk(w1->word);
	w1->word = s1;
	return (w1);
}

t_rl_word	*ft_rl_joinword(t_rl_word *w1, t_rl_word *w2)
{
	char	*word;

	word = ft_strjoin(w1->word, w2->word);
	if (word)
	{
		ft_popblks(3, w1->word, w2->word, w2);
		w1->word = ft_push(word);
		w1->len += w2->len;
		w1->next = w2->next;
		if (w1->next)
			w1->next->prev = w1;
	}
	return (w1);
}

void	ft_rl_insertword(t_rl_input *input, t_rl_word *newword)
{
	if (!input->head)
		input->head = newword;
	else
	{
		newword->prev = input->current;
		newword->next = input->current->next;
		input->current->next = newword;
		if (newword->next)
			newword->next->prev = newword;
	}
	input->current = newword;
}

void	ft_rl_addword(t_rl_input *input, uint8_t c)
{
	t_rl_word	*newword;

	newword = ft_push(ft_calloc(1, sizeof(*newword)));
	if (!newword)
		return ;
	newword->word = ft_calloc(2, sizeof(c));
	ft_popblk(newword);
	if (!newword->word)
		return ;
	ft_pushn(2, newword, newword->word);
	if (c == KEY_SPACE)
		newword->wtype = SPACE;
	else
		newword->wtype = NORMAL;
	*newword->word = c;
	newword->len = 1;
	newword->i = 1;
	if (input->current && input->current->i != input->current->len)
		input->current = ft_rl_splitword(input->current);
	ft_rl_insertword(input, newword);
}

void	ft_rl_rmword(t_rl_input *input, t_rl_word *dword, uint64_t key)
{
	if (dword->prev)
		dword->prev->next = dword->next;
	else
		input->head = dword->next;
	if (dword->next)
		dword->next->prev = dword->prev;
	ft_popblks(2, dword, dword->word);
	if (key == KEY_DEL && dword->next)
		input->current = dword->next;
	else
		input->current = dword->prev;
	if (input->current && input->current->prev
		&& input->current->wtype == input->current->prev->wtype)
		input->current = ft_rl_joinword(input->current->prev, input->current);
	if (input->current && input->current->next
		&& input->current->wtype == input->current->next->wtype)
		input->current = ft_rl_joinword(input->current, input->current->next);
}
