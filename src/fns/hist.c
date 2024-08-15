/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:12:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 04:16:40 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_acl(rl_input_t *input)
{
	__putstr_fd(TERM_CRNL, 1);
	input->exittype = E_ACL;
	return (0);
}

uint8_t	ft_rl_pvh(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count == 0)
		return (1);
	while (count > 0)
	{
		if (!ft_rl_hist_get_prev(input))
			break ;
		count--;
	}
	while (count < 0)
	{
		if (!ft_rl_hist_get_next(input))
			break ;
		count++;
	}
	ft_rl_redisplay(input, INPUT);
	input->i = MAX(input->len - (ft_rl_geteditmode() == _MD_VI_CMD), 0);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_nxh(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count == 0)
		return (1);
	while (count > 0)
	{
		if (!ft_rl_hist_get_next(input))
			break ;
		count--;
	}
	while (count < 0)
	{
		if (!ft_rl_hist_get_prev(input))
			break ;
		count++;
	}
	ft_rl_redisplay(input, INPUT);
	input->i = MAX(input->len - (ft_rl_geteditmode() == _MD_VI_CMD), 0);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_soh(rl_input_t *input)
{
	if (!ft_rl_hist_get_last(input))
		return (1);
	ft_rl_redisplay(input, INPUT);
	input->i = MAX(input->len - (ft_rl_geteditmode() == _MD_VI_CMD), 0);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_eoh(rl_input_t *input)
{
	if (!ft_rl_hist_get_first(input))
		return (1);
	ft_rl_redisplay(input, INPUT);
	input->i = MAX(input->len - (ft_rl_geteditmode() == _MD_VI_CMD), 0);
	ft_rl_cursor_reset(input);
	return (1);
}

uint8_t	ft_rl_rsh(rl_input_t *input)
{
	if (!g_hist_cur)
		return (1);
	__popblk(((rl_histnode_t *)g_hist_cur->blk)->edit);
	((rl_histnode_t *)g_hist_cur->blk)->edit = __push(__strdup(input->line));
	return (ft_rl_hist_search(input, _SEARCH_BCK));
}

uint8_t	ft_rl_fsh(rl_input_t *input)
{
	if (!g_hist_cur)
		return (1);
	__popblk(((rl_histnode_t *)g_hist_cur->blk)->edit);
	((rl_histnode_t *)g_hist_cur->blk)->edit = __push(__strdup(input->line));
	return (ft_rl_hist_search(input, _SEARCH_FWD));
}

uint8_t	ft_rl_rsh_i(rl_input_t *input)
{
	if (!g_hist_cur)
		return (1);
	__popblk(((rl_histnode_t *)g_hist_cur->blk)->edit);
	((rl_histnode_t *)g_hist_cur->blk)->edit = __push(__strdup(input->line));
	return (ft_rl_hist_isearch(input, _I_SEARCH_BCK));
}

uint8_t	ft_rl_fsh_i(rl_input_t *input)
{
	if (!g_hist_cur)
		return (1);
	__popblk(((rl_histnode_t *)g_hist_cur->blk)->edit);
	((rl_histnode_t *)g_hist_cur->blk)->edit = __push(__strdup(input->line));
	return (ft_rl_hist_isearch(input, _I_SEARCH_FWD));
}

uint8_t	ft_rl_yna(rl_input_t *input)
{
	int32_t		n;

	if (!g_hist_cur || !g_hist_cur->next)
		return (1);
	n = 1;
	if (g_argument.set)
		n = ft_rl_getarg();
	ft_rl_hist_yank_arg(input, g_hist_cur->next->blk, n);
	return (1);
}

uint8_t	ft_rl_yla(rl_input_t *input)
{
	int32_t			n;
	rl_fn_t			fn;
	uint8_t			direction;
	const t_list	*hist;

	if (!g_hist_cur || !g_hist_cur->next)
		return (1);
	n = INT32_MAX;
	direction = _HIST_BCK;
	hist = g_hist_cur->next;
	if (g_argument.set)
		n = ft_rl_getarg();
	ft_rl_setmark(_MARK_START);
	ft_rl_hist_yank_arg(input, hist->blk, n);
	ft_rl_setmark(_MARK_END);
	input->key = ft_rl_getkey();
	fn = ft_rl_getmap(input->key);
	g_status |= _YLA_RUNNING;
	while (fn == ft_rl_arg || fn == ft_rl_arg_n || fn == ft_rl_yla)
	{
		if (fn != ft_rl_yla)
			fn(input);
		if (g_argument.set && ft_rl_getarg() < 0)
			direction ^= _HIST_FWD | _HIST_BCK;
		switch (direction)
		{
			case _HIST_BCK:
				if (hist->next)
					hist = hist->next;
				break ;
			case _HIST_FWD:
				if (hist->prev && hist->prev != g_hist_cur)
					hist = hist->prev;
				break ;
		}
		ft_rl_kill_region(input);
		input->i = MIN(input->i, input->len);
		ft_rl_hist_yank_arg(input, hist->blk, n);
		ft_rl_setmark(_MARK_END);
		input->key = ft_rl_getkey();
		fn = ft_rl_getmap(input->key);
	}
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	g_status &= ~_YLA_RUNNING;
	if (fn)
		return (fn(input));
	return (1);
}
