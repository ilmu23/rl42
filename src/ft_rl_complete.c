/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_complete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 13:18:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 11:21:06 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline uint8_t	complete_mult(t_rl_input *input, t_list *completions);
static inline t_list	*completew(t_rl_word *w);
static inline void		display(t_rl_input *input, t_list *st, t_list *c);
static inline void		putcompletions(t_rl_cursor *cur, t_list *st, t_list *c);

uint8_t	ft_rl_complete(t_rl_input *input)
{
	t_list	*completions;
	uint8_t	rv;

	if (!input->head)
		return (1);
	rv = 1;
	completions = completew(input->current);
	ft_rl_initreplace(input->current->prev, input->current->next);
	if (!completions)
		input->current->i = input->current->len;
	else if (!completions->next)
		ft_rl_replaceword(input, ft_rl_strword(completions->blk));
	else
	{
		display(input, completions, NULL);
		rv = complete_mult(input, completions);
	}
	ft_rl_endreplace();
	if (rv)
		ft_rl_redisplay(input, LINE);
	return (rv);
}

static inline uint8_t	complete_mult(t_rl_input *input, t_list *completions)
{
	const t_list	*start = completions;

	ft_lstlast(completions)->next = completions;
	while (completions)
	{
		input->key = 0;
		if (read(0, &input->key, sizeof(input->key)) < 0)
			return (-1);
		if (ft_rl_getmap(input->key) == ft_rl_cmp)
		{
			ft_rl_replaceword(input, ft_rl_strword(completions->blk));
			display(input, (t_list *)start, completions);
			completions = completions->next;
			continue ;
		}
		if (ft_rl_ismapped(input->key))
			return (ft_rl_execmap(input, input->key));
		return (1);
	}
	return (1);
}

static inline t_list	*completew(t_rl_word *w)
{
	t_list	*completions;

	if (ft_strchr(w->word, '*'))
		completions = ft_rl_complete_wc(w->word);
	else if (*w->word == '$')
		completions = ft_rl_complete_env(w->word + 1);
	else if (w->wtype == NORMAL)
		completions = ft_rl_complete_file(w->word);
	else
		completions = ft_rl_complete_file("");
	if (completions && completions->next)
		ft_rl_complete_inc(completions, w->word + (*w->word == '$'));
	return (completions);
}

static inline void	display(t_rl_input *input, t_list *st, t_list *c)
{
	uint8_t		cpr;
	uint16_t	rows;
	t_rl_cursor	cursor;

	cpr = input->cursor->t_cols / (ft_rl_complete_getlongest(st) + 2);
	rows = *st->size / cpr + 1;
	cursor = *input->cursor;
	cursor.i_row++;
	cursor.i_row += (cursor.i_col + ft_rl_getinputlen(input)) / cursor.t_cols;
	cursor.i_col = 1;
	cursor.row = cursor.i_row;
	cursor.col = cursor.i_col;
	ft_rl_redisplay(input, LINE);
	while (cursor.i_row + rows - 1 > cursor.t_rows && input->cursor->i_row > 1)
	{
		cursor.i_row--;
		input->cursor->i_row--;
	}
	putcompletions(&cursor, st, c);
	ft_rl_resetcursor(input);
}

static inline void	putcompletions(t_rl_cursor *cur, t_list *st, t_list *c)
{
	uint8_t		i;
	uint8_t		cpr;
	size_t		mlen;

	i = 0;
	mlen = (ft_rl_complete_getlongest(st) + 2);
	cpr = cur->t_cols / mlen;
	ft_rl_updatecursor(cur);
	while (st)
	{
		ft_rl_putcompletion(st, c, ft_rl_complete_basename(st->blk), mlen);
		if (++i == cpr)
		{
			i = 0;
			ft_putchar_fd('\n', 1);
		}
		st = st->next;
		if (!st || !st->prev)
			break ;
	}
}
