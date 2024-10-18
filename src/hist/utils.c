/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:23:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

rl_histnode_t	*ft_rl_hist_mknode(const char *line)
{
	rl_histnode_t	*out;

	out = __push(__alloc(sizeof(*out)));
	if (!out)
		exit(ft_rl_perror());
	out->line = __push(__strdup(line));
	out->edit = NULL;
	return (out);
}

char	*ft_rl_hist_get_line(const __t_list *node)
{
	const rl_histnode_t	*histnode;

	histnode = node->blk;
	if (histnode->edit)
		return (__push(__strdup(histnode->edit)));
	return (__push(__strdup(histnode->line)));
}

void	ft_rl_hist_yank_arg(rl_input_t *input, const rl_histnode_t *node, int32_t n)
{
	char		*subs[2];
	char		**args;
	uint64_t	len;

	args = __push(__split(node->line, ' '));
	if (!args)
		exit(ft_rl_perror());
	if (n >= 0)
		n = MIN(n, (int32_t)(__getblksize(args) / sizeof(char *) - 2));
	else
		n = MAX(0, __getblksize(args) / sizeof(char *) - 2 - -n);
	len = strlen(__push(args[n]));
	subs[0] = __push(__substr(input->line, 0, input->i));
	subs[1] = __push(__substr(input->line, input->i, input->len - input->i));
	__popblk(input->line);
	input->line = __push(__strnjoin(3, subs[0], args[n], subs[1]));
	__popblks(4, subs[0], args[n], subs[1], args);
	input->len += len;
	input->i += len;
	ft_rl_redisplay(input, INPUT);
}

void	ft_rl_hist_recycle(void)
{
	__t_list			*last;
	rl_histnode_t	*node;

	last = __lstlast(g_hist);
	node = (rl_histnode_t *)last->blk;
	__popblk(node->line);
	node->line = __push(__strdup(""));
	__lstrmnode(&g_hist, last);
	last->prev = NULL;
	__lstadd_front(&g_hist, (__t_list *)last);
}

void	ft_rl_hist_restore(void)
{
	rl_histnode_t	*node;

	g_hist_cur = g_hist;
	while (g_hist_cur)
	{
		node = (rl_histnode_t *)g_hist_cur->blk;
		__popblk(node->edit);
		node->edit = NULL;
		g_hist_cur = g_hist_cur->next;
	}
}

void	ft_rl_hist_newnode(void)
{
	int64_t	hsize;

	hsize = ft_rl_get(_HIST_SIZE_HASH);
	if (!g_hist || hsize < 0 || (int64_t)MIN(*g_hist->size, INT64_MAX) <= hsize)
		__lstadd_front(&g_hist, __lstnew(ft_rl_hist_mknode("")));
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
		__popblks(2, node->line, node->edit);
	__lstrmnode(&g_hist, g_hist);
}
