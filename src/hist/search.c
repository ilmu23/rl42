/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:47:23 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 21:06:58 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline rl_fn_t	_search(rl_input_t *input, rl_input_t *search, const uint8_t direction);
static inline t_list	*_match(const char *pattern, const t_list *cur, const uint8_t direction);
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
			ft_memcpy(input, &(rl_input_t){.line = NULL,
					.prompt = _SEARCH_BCK_PROMPT, .keystr = NULL,
					.exittype = ACL, .cursor = ft_rl_cursor_init(), .plen = 14,
					.len = 0, .key = 0, .i = 0}, sizeof(*input));
			break ;
		case _SEARCH_FWD:
			ft_memcpy(input, &(rl_input_t){.line = NULL,
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
	fn = _search(input, &search, direction);
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
	while (fn == ft_rl_ins || fn == ft_rl_del)
	{
		fn(search);
		match = _match(search->line, match, direction);
		if (!match && search->plen == 14)
		{
			search->prompt -= 8;
			ft_memcpy((uint64_t *)&search->plen, &(uint64_t){22}, sizeof(search->plen));
		}
		else if (match && search->plen == 22)
		{
			search->prompt += 8;
			ft_memcpy((uint64_t *)&search->plen, &(uint64_t){14}, sizeof(search->plen));
		}
		ft_popblk(input->line);
		if (!match && search->len == 0)
			input->line = ft_rl_hist_get_line(g_hist_cur);
		else
			input->line = ft_rl_hist_get_line(match);
		input->len = ft_strlen(input->line);
		input->i = input->len;
		_display(input, search);
		search->key = ft_rl_getkey();
		fn = ft_rl_getmap(search->key);
	}
	return (fn);
}

static inline t_list	*_match(const char *pattern, const t_list *cur, const uint8_t direction)
{
	const t_list	*node;
	const char		*line;

	node = cur;
	if (!node)
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

static inline void	_display(rl_input_t *input, rl_input_t *search)
{
	ft_rl_redisplay(input, INPUT);
	ft_rl_redisplay(search, INPUT);
}
