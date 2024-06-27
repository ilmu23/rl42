/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:23:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/27 13:58:19 by ivalimak         ###   ########.fr       */
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

void	ft_rl_hist_yank_arg(rl_input_t *input, const rl_histnode_t *node, int32_t n)
{
	char		*subs[2];
	char		**args;
	uint64_t	len;

	args = ft_push(ft_split(node->line, ' '));
	if (!args)
		exit(ft_rl_perror());
	if (n >= 0)
		n = ft_min(n, ft_getblksize(args) / sizeof(char *) - 2);
	else
		n = ft_max(0, ft_getblksize(args) / sizeof(char *) - 2 - -n);
	len = ft_strlen(ft_push(args[n]));
	subs[0] = ft_push(ft_substr(input->line, 0, input->i));
	subs[1] = ft_push(ft_substr(input->line, input->i, input->len - input->i));
	ft_popblk(input->line);
	input->line = ft_push(ft_strnjoin(3, subs[0], args[n], subs[1]));
	ft_popblks(4, subs[0], args[n], subs[1], args);
	input->len += len;
	input->i += len;
	ft_rl_redisplay(input, INPUT);
}

void	ft_rl_hist_recycle(void)
{
	t_list			*last;
	rl_histnode_t	*node;

	last = ft_lstlast(g_hist);
	node = (rl_histnode_t *)last->blk;
	ft_popblk(node->line);
	node->line = ft_push(ft_strdup(""));
	ft_lstrmnode(&g_hist, last);
	last->prev = NULL;
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
	int64_t	hsize;

	hsize = ft_rl_get(_HIST_SIZE_HASH);
	if (!g_hist || hsize < 0 || ft_min(*g_hist->size, INT64_MAX) <= hsize)
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
