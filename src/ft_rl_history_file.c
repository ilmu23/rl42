/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_history_file.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:10:48 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 12:02:43 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_hist_load(const char *fname)
{
	int32_t	fd;
	t_list	**hist;
	char	*line;

	fd = open(ft_strsjoin(getenv("HOME"), fname, '/'), O_RDONLY);
	if (fd < 0)
		return ;
	hist = ft_rl_hist_gethead();
	line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r "));
	while (line)
	{
		ft_lstadd_back(hist, ft_lstnew(ft_rl_strinput(line)));
		ft_popblk(line);
		line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r "));
	}
	close(fd);
}

void	ft_rl_hist_save(const char *fname)
{
	int32_t	fd;
	t_list	*node;

	fd = open(ft_strsjoin(getenv("HOME"), fname, '/'), O_WRONLY | O_CREAT,
			0644);
	if (fd < 0)
		return ;
	node = *ft_rl_hist_gethead();
	while (node)
	{
		ft_putendl_fd(ft_rl_inputstr(node->blk, 0), fd);
		node = node->next;
	}
	close(fd);
}
