/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 19:59:09 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 04:26:56 by ivalimak         ###   ########.fr       */
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
	ft_popblk(node->edit);
	node->edit = ft_push(ft_strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	g_hist_cur = g_hist_cur->prev;
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(input->line);
	if (node->edit)
		input->line = ft_push(ft_strdup(node->edit));
	else
		input->line = ft_push(ft_strdup(node->line));
	if (!input->line)
		exit(ft_rl_perror());
	input->len = ft_strlen(input->line);
	return (1);
}

uint8_t	ft_rl_hist_get_prev(rl_input_t *input)
{
	rl_histnode_t	*node;

	if (!g_hist_cur || !g_hist_cur->next)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(node->edit);
	node->edit = ft_push(ft_strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	g_hist_cur = g_hist_cur->next;
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(input->line);
	if (node->edit)
		input->line = ft_push(ft_strdup(node->edit));
	else
		input->line = ft_push(ft_strdup(node->line));
	if (!input->line)
		exit(ft_rl_perror());
	input->len = ft_strlen(input->line);
	return (1);
}

uint8_t	ft_rl_hist_get_last(rl_input_t *input)
{
	rl_histnode_t	*node;
	
	if (!g_hist_cur || !g_hist_cur->next)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(node->edit);
	node->edit = ft_push(ft_strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	while (g_hist_cur->next)
		g_hist_cur = g_hist_cur->next;
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(input->line);
	if (node->edit)
		input->line = ft_push(ft_strdup(node->edit));
	else
		input->line = ft_push(ft_strdup(node->line));
	if (!input->line)
		exit(ft_rl_perror());
	input->len = ft_strlen(input->line);
	return (1);
}

uint8_t	ft_rl_hist_get_first(rl_input_t *input)
{
	rl_histnode_t	*node;

	if (!g_hist_cur || !g_hist_cur->prev)
		return (0);
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(node->edit);
	node->edit = ft_push(ft_strdup(input->line));
	if (!node->edit)
		exit(ft_rl_perror());
	while (g_hist_cur->prev)
		g_hist_cur = g_hist_cur->prev;
	node = (rl_histnode_t *)g_hist_cur->blk;
	ft_popblk(input->line);
	if (node->edit)
		input->line = ft_push(ft_strdup(node->edit));
	else
		input->line = ft_push(ft_strdup(node->line));
	if (!input->line)
		exit(ft_rl_perror());
	input->len = ft_strlen(input->line);
	return (1);
}
