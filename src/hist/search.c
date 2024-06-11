/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:47:23 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/11 22:02:59 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline rl_fn_t	_search(rl_input_t *input, rl_input_t *search, const uint8_t direction);
static inline t_list	*_match(const char *pattern, const uint8_t direction);
static inline void		_replace(rl_input_t *input, const t_list *node);
static inline void		_display(rl_input_t *input, rl_input_t *search);

uint8_t	ft_rl_hist_isearch(rl_input_t *input, const uint8_t direction)
{
	rl_input_t	search;
	rl_fn_t		fn;

	ft_rl_eol(input);
	ft_putstr_fd(TERM_CRNL, 1);
	switch (direction)
	{
		case _SEARCH_BCK:
			ft_memcpy(&search, &(rl_input_t){.line = NULL,
					.prompt = _SEARCH_BCK_PROMPT, .keystr = NULL,
					.exittype = ACL, .cursor = ft_rl_cursor_init(), .plen = 14,
					.len = 0, .key = 0, .i = 0}, sizeof(*input));
			break ;
		case _SEARCH_FWD:
			ft_memcpy(&search, &(rl_input_t){.line = NULL,
					.prompt = _SEARCH_FWD_PROMPT, .keystr = NULL,
					.exittype = ACL, .cursor = ft_rl_cursor_init(), .plen = 14,
					.len = 0, .key = 0, .i = 0}, sizeof(*input));
			break ;
	}
	if (!search.cursor)
		exit(ft_rl_perror());
	search.prompt += 8;
	ft_putstr_fd(search.prompt, 1);
	ft_rl_cursor_getpos(&search.cursor->row, &search.cursor->col);
	search.cursor->i_row = search.cursor->row;
	search.cursor->i_col = search.cursor->col;
	input->line = ft_push(ft_strdup(input->line));
	fn = _search(input, &search, direction);
	ft_rl_redisplay(input, CLEAR);
	ft_popblk(search.cursor);
	if (direction == _SEARCH_BCK)
	{
		if (fn != ft_rl_rsh_i)
			return (fn(input));
	}
	else if (fn != ft_rl_fsh_i)
		return (fn(input));
	return (1);
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
		if (direction == _SEARCH_BCK)
			node = node->next;
		else
			node = node->prev;
	}
	return ((t_list *)node);
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
