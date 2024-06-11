/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:47:23 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 02:36:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline rl_fn_t	_isearch(rl_input_t *input, rl_input_t *search, const uint8_t direction);
static inline rl_fn_t	_search(rl_input_t *input, rl_input_t *search, const uint8_t direction);
static inline t_list	*_match(const char *pattern, const uint8_t direction);
static inline void		_init(rl_input_t *search, const uint8_t direction);
static inline void		_replace(rl_input_t *input, const t_list *node);
static inline void		_display(rl_input_t *input, rl_input_t *search);

uint8_t	ft_rl_hist_search(rl_input_t *input, const uint8_t direction)
{
	rl_input_t	search;
	rl_fn_t		fn;

	ft_rl_eol(input);
	ft_putstr_fd(TERM_CRNL, 1);
	_init(&search, direction);
	fn = _search(input, &search, direction);
	ft_rl_redisplay(input, CLEAR);
	ft_popblk(search.cursor);
	if (direction == _SEARCH_BCK)
	{
		if (fn != ft_rl_rsh)
			return (fn(input));
	}
	else if (fn != ft_rl_fsh)
		return (fn(input));
	return (1);
}

uint8_t	ft_rl_hist_isearch(rl_input_t *input, const uint8_t direction)
{
	rl_input_t	search;
	rl_fn_t		fn;

	ft_rl_eol(input);
	ft_putstr_fd(TERM_CRNL, 1);
	_init(&search, direction);
	fn = _isearch(input, &search, direction);
	ft_rl_redisplay(input, CLEAR);
	ft_popblk(search.cursor);
	if (direction == _I_SEARCH_BCK)
	{
		if (fn != ft_rl_rsh_i)
			return (fn(input));
	}
	else if (fn != ft_rl_fsh_i)
		return (fn(input));
	return (1);
}

static inline rl_fn_t	_isearch(rl_input_t *input, rl_input_t *search, const uint8_t direction)
{
	const t_list	*match;
	rl_fn_t			fn;

	match = NULL;
	search->key = ft_rl_getkey();
	fn = ft_rl_getmap(search->key);
	while (fn == ft_rl_ins || fn == ft_rl_del || fn == ft_rl_bdl)
	{
		fn(search);
		match = _match(search->line, direction);
		if (!match)
		{
			match = g_hist_cur;
			if (search->len > 0 && search->plen == 14)
			{
				search->prompt -= 8;
				search->cursor->i_col += 8;
				ft_memcpy((uint64_t *)&search->plen, &(uint64_t){22}, sizeof(search->plen));
			}
		}
		else if (match && search->plen == 22)
		{
			search->prompt += 8;
			search->cursor->i_col -= 8;
			ft_memcpy((uint64_t *)&search->plen, &(uint64_t){14}, sizeof(search->plen));
		}
		if (search->plen == 14)
			_replace(input, match);
		_display(input, search);
		search->key = ft_rl_getkey();
		fn = ft_rl_getmap(search->key);
	}
	if (match && g_hist_cur)
		g_hist_cur = match;
	return (fn);
}

static inline rl_fn_t	_search(rl_input_t *input, rl_input_t *search, const uint8_t direction)
{
	const t_list	*match;
	rl_fn_t			fn;

	match = NULL;
	search->key = ft_rl_getkey();
	fn = ft_rl_getmap(search->key);
	while (fn == ft_rl_ins || fn == ft_rl_del || fn == ft_rl_bdl)
	{
		fn(search);
		search->key = ft_rl_getkey();
		fn = ft_rl_getmap(search->key);
	}
	match = _match(search->line, direction);
	if (match)
	{
		_replace(input, match);
		if (g_hist_cur)
			g_hist_cur = match;
		_display(input, search);
	}
	return (fn);
}

static inline t_list	*_match(const char *pattern, const uint8_t direction)
{
	const t_list	*node;
	const char		*line;

	node = g_hist_cur;
	while (node)
	{
		line = ft_rl_hist_get_line(node);
		if (ft_strnstr(line, pattern, ft_strlen(line)))
			break ;
		if (direction % 2 == 0)
			node = node->next;
		else
			node = node->prev;
	}
	return ((t_list *)node);
}

static inline void	_init(rl_input_t *search, const uint8_t direction)
{
	ft_memcpy(search, &(rl_input_t){.line = NULL,
			.keystr = NULL, .exittype = ACL, .cursor = ft_rl_cursor_init(),
			.plen = 14 - ((direction < _I_SEARCH_BCK) * 2),
			.len = 0, .key = 0, .i = 0}, sizeof(*search));
	switch (direction)
	{
		case _SEARCH_BCK:
			search->prompt = _SEARCH_BCK_PROMPT;
			break ;
		case _SEARCH_FWD:
			search->prompt = _SEARCH_FWD_PROMPT;
			break ;
		case _I_SEARCH_BCK:
			search->prompt = _I_SEARCH_BCK_PROMPT;
			break ;
		case _I_SEARCH_FWD:
			search->prompt = _I_SEARCH_FWD_PROMPT;
			break ;
	}
	if (!search->cursor)
		exit(ft_rl_perror());
	if (search->plen == 14)
		search->prompt += 8;
	ft_putstr_fd(search->prompt, 1);
	ft_rl_cursor_getpos(&search->cursor->row, &search->cursor->col);
	search->cursor->i_row = search->cursor->row;
	search->cursor->i_col = search->cursor->col;
}

static inline void	_replace(rl_input_t *input, const t_list *node)
{
	ft_popblk(input->line);
	input->line = ft_rl_hist_get_line(node);
	input->len = ft_strlen(input->line);
	input->i = input->len;
}

static inline void	_display(rl_input_t *input, rl_input_t *search)
{
	ft_rl_redisplay(input, INPUT);
	ft_rl_redisplay(search, PROMPT);
}
