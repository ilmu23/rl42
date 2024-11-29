/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historyfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 19:13:30 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 16:37:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_hist_load(const char *path)
{
	int32_t		fd;
	const char	*line;

	g_hist = NULL;
	g_hist_init = NULL;
	if (path && *path == '~')
	{
		path = __strsjoin(getenv("HOME"), __substr(path, 1, strlen(path)), '/');
		if (!path)
			exit(ft_rl_perror());
	}
	if (access(path, F_OK) == -1)
		return ;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		goto _err;
	line = __push(__strtrim(__getline(fd), "\t\n\v\f\r"));
	while (line)
	{
		__lstadd_front(&g_hist, __lstnew(ft_rl_hist_mknode(line)));
		__popblk(line);
		line = __push(__strtrim(__getline(fd), "\t\n\v\f\r"));
	}
	close(fd);
	return ;
	_err:
		ft_rl_perror();
}

void	ft_rl_hist_save(const char *path)
{
	int32_t				fd;
	const __t_list		*node;
	const rl_histnode_t	*hnode;

	if (path && *path == '~')
	{
		path = __strsjoin(getenv("HOME"), __substr(path, 1, strlen(path)), '/');
		if (!path)
		{
			ft_rl_perror();
			return ;
		}
	}
	fd = open(path, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		goto _err;
	node = __lstlast(g_hist);
	while (node)
	{
		hnode = node->blk;
		__putendl_fd(hnode->line, fd);
		node = node->prev;
	}
	close(fd);
	return ;
	_err:
		ft_rl_perror();
}
