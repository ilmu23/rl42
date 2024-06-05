/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:23:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 11:46:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

rl_histnode_t	*ft_rl_hist_mknode(const char *line)
{
	rl_histnode_t	*out;

	out = ft_push(ft_alloc(sizeof(*out)));
	if (!out)
		exit(ft_rl_perror());
	out->line = ft_push(ft_strdup(line));
	out->edit = NULL;
	return (out);
}

char	*ft_rl_hist_get_line(const t_list *node)
{
	const rl_histnode_t	*histnode;

	histnode = node->blk;
	if (histnode->edit)
		return (ft_push(ft_strdup(histnode->edit)));
	return (ft_push(ft_strdup(histnode->line)));
}

void	ft_rl_hist_recycle(void)
{
	const t_list	*last;
	rl_histnode_t	*node;

	last = ft_lstlast(g_hist);
	node = (rl_histnode_t *)last->blk;
	ft_popblk(node->line);
	node->line = ft_push(ft_strdup(""));
	ft_lstrmnode(&g_hist, last);
	ft_lstadd_front(&g_hist, (t_list *)last);
}

void	ft_rl_hist_restore(void)
{
	rl_histnode_t	*node;

	g_hist_cur = g_hist;
	while (g_hist_cur)
	{
		node = (rl_histnode_t *)g_hist_cur->blk;
		ft_popblk(node->edit);
		node->edit = NULL;
		g_hist_cur = g_hist_cur->next;
	}
}

void	ft_rl_hist_newnode(void)
{
	if (*g_hist->size < _FT_RL_HSIZE)
		ft_lstadd_front(&g_hist, ft_lstnew(ft_rl_hist_mknode("")));
	else
		ft_rl_hist_recycle();
}

void	ft_rl_hist_rmnode(void)
{
	const rl_histnode_t	*node;

	if (!g_hist)
		return ;
	node = g_hist->blk;
	if (node)
		ft_popblks(2, node->line, node->edit);
	ft_lstrmnode(&g_hist, g_hist);
}
