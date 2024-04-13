/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:24:54 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 16:26:29 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

t_rl_input	*ft_rl_dupinput(t_rl_input *input)
{
	t_rl_input	*out;

	if (!input)
		return (NULL);
	out = ft_push(ft_calloc(1, sizeof(*out)));
	*out = (t_rl_input){.plen = input->plen,
		.prompt = ft_push(ft_strdup(input->prompt)), .cursor = input->cursor};
	out->head = ft_rl_dupwords(input->head);
	out->current = out->head;
	while (out->current && out->current->next)
		out->current = out->current->next;
	return (out);
}

t_rl_word	*ft_rl_dupwords(t_rl_word *words)
{
	t_rl_word	*head;
	t_rl_word	*word;

	head = NULL;
	while (words)
	{
		if (!head)
			word = ft_push(ft_calloc(1, sizeof(*word)));
		else
		{
			word->next = ft_push(ft_calloc(1, sizeof(*word)));
			if (word->next)
				word->next->prev = word;
			word->next->prev = word;
			word = word->next;
		}
		if (!word)
			return (NULL);
		*word = (t_rl_word){.i = words->len, .len = words->len, .prev = word->prev,
			.wtype = words->wtype, .word = ft_push(ft_strdup(words->word))};
		if (!word->word)
			return (NULL);
		if (!head)
			head = word;
		words = words->next;
	}
	return (head);
}

char	*ft_rl_inputstr(t_rl_input *input, uint8_t pop)
{
	char		*out;
	t_rl_word	*w;

	if (!input || !input->head)
	{
		if (input->exittype == EOF)
			return (NULL);
		return (ft_strdup(""));
	}
	w = input->head;
	out = NULL;
	while (w)
	{
		out = ft_strjoin(out, w->word);
		if (pop)
			ft_popblks(2, w, w->word);
		w = w->next;
	}
	if (pop)
		ft_popblk(input);
	return (out);
}

void	ft_rl_redisplay(t_rl_input *input, t_rl_rdmode mode)
{
	t_rl_word	*w;

	if (mode == ALL)
		ft_putstr_fd(TERM_CUR_RESET, 1);
	else
		ft_rl_inputcursor(input);
	ft_putstr_fd(TERM_CLEAR_END, 1);
	if (mode == PROMPT)
		ft_printf("%s%s", TERM_CUR_SOL, input->prompt);
	w = input->head;
	if (mode == ALL)
	{
		ft_putstr_fd(input->prompt, 1);
		w = input->head;
	}
	while (w)
	{
		ft_putstr_fd(w->word, 1);
		w = w->next;
	}
	ft_rl_resetcursor(input);
}

void	ft_rl_popwords(t_rl_word *words)
{
	while (words)
	{
		ft_popblks(2, words, words->word);
		words = words->next;
	}
}
