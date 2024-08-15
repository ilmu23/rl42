/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 19:59:09 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:34:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_data.h"
#include "ft_rl_internal.h"

uint8_t	ft_rl_hist_get_next(rl_input_t *input)
{
	rl_histnode_t	*node;

	if (!g_hist_cur || !g_hist_cur->prev)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	__popblk(node->edit);
	node->edit = __push(__strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	g_hist_cur = g_hist_cur->prev;
	__popblk(input->line);
	input->line = ft_rl_hist_get_line(g_hist_cur);
	if (!input->line)
		exit(ft_rl_perror());
	input->len = strlen(input->line);
	return (1);
}

uint8_t	ft_rl_hist_get_prev(rl_input_t *input)
{
	rl_histnode_t	*node;

	if (!g_hist_cur || !g_hist_cur->next)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	__popblk(node->edit);
	node->edit = __push(__strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	g_hist_cur = g_hist_cur->next;
	__popblk(input->line);
	input->line = ft_rl_hist_get_line(g_hist_cur);
	if (!input->line)
		exit(ft_rl_perror());
	input->len = strlen(input->line);
	return (1);
}

uint8_t	ft_rl_hist_get_last(rl_input_t *input)
{
	rl_histnode_t	*node;
	
	if (!g_hist_cur || !g_hist_cur->next)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	__popblk(node->edit);
	node->edit = __push(__strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	while (g_hist_cur->next)
		g_hist_cur = g_hist_cur->next;
	__popblk(input->line);
	input->line = ft_rl_hist_get_line(g_hist_cur);
	if (!input->line)
		exit(ft_rl_perror());
	input->len = strlen(input->line);
	return (1);
}

uint8_t	ft_rl_hist_get_first(rl_input_t *input)
{
	rl_histnode_t	*node;

	if (!g_hist_cur || !g_hist_cur->prev)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	__popblk(node->edit);
	node->edit = __push(__strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	g_hist_cur = g_hist;
	__popblk(input->line);
	input->line = ft_rl_hist_get_line(g_hist_cur);
	if (!input->line)
		exit(ft_rl_perror());
	input->len = strlen(input->line);
	return (1);
}
