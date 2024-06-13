/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historyfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 19:13:30 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/13 18:20:35 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_hist_load(const char *path)
{
	int32_t		fd;
	const char	*line;

	g_hist = NULL;
	if (path && *path == '~')
	{
		path = ft_strsjoin(getenv("HOME"), ft_substr(path, 1, ft_strlen(path)), '/');
		if (!path)
			exit(ft_rl_perror());
	}
	if (access(path, F_OK) == -1)
		return ;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		goto _err;
	line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r"));
	while (line)
	{
		ft_lstadd_front(&g_hist, ft_lstnew(ft_rl_hist_mknode(line)));
		ft_popblk(line);
		line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r"));
	}
	close(fd);
	return ;
	_err:
		ft_rl_perror();
}

void	ft_rl_hist_save(const char *path)
{
	int32_t				fd;
	const t_list		*node;
	const rl_histnode_t	*hnode;

	if (path && *path == '~')
	{
		path = ft_strsjoin(getenv("HOME"), ft_substr(path, 1, ft_strlen(path)), '/');
		if (!path)
		{
			ft_rl_perror();
			return ;
		}
	}
	fd = open(path, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		goto _err;
	node = ft_lstlast(g_hist);
	while (node)
	{
		hnode = node->blk;
		ft_putendl_fd(hnode->line, fd);
		node = node->prev;
	}
	close(fd);
	return ;
	_err:
		ft_rl_perror();
}
