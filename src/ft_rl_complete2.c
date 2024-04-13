/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_complete2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 12:37:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 11:59:54 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	matchfiles(const char *pattern, DIR *dir, t_list **cmp);
static inline void	checkdirs(const char *path, t_list *completions);

t_list	*ft_rl_complete_wc(const char *pattern)
{
	t_rl_wc	*wc;
	t_list	*completions;
	char	*str;

	wc = ft_rl_wildcard_expand(pattern);
	if (!wc || !wc->matches)
		return (NULL);
	str = NULL;
	completions = NULL;
	while (wc->matches)
	{
		str = ft_strsjoin(str, wc->matches->blk, ' ');
		wc->matches = wc->matches->next;
	}
	ft_lstpopall(ft_lstfirst(wc->matches));
	ft_lstadd_front(&completions, ft_lstnew(str));
	return (completions);
}

t_list	*ft_rl_complete_env(const char *pattern)
{
	char		*varname;
	extern char	**environ;
	t_list		*completions;
	size_t		plen;
	
	completions = NULL;
	plen = ft_strlen(pattern);
	while (*environ)
	{
		varname = ft_substr(*environ, 0, ft_strclen(*environ, '='));
		if (!ft_strncmp(varname, pattern, plen))
			ft_lstadd_back(&completions, ft_lstnew(ft_strjoin("$", varname)));
		environ++;
	}
	return (completions);
}

t_list	*ft_rl_complete_file(const char *pattern)
{
	t_list	*completions;
	char	*path;

	completions = NULL;
	path = ft_strrchr(pattern, '/');
	if (path)
	{
		path = ft_substr(pattern, 0, path - pattern);
		if (ft_strequals(path, ""))
			path = ft_strdup("/");
		pattern = ft_strrchr(pattern, '/') + 1;
		matchfiles(pattern, opendir(ft_push(path)), &completions);
	}
	else
		matchfiles(pattern, opendir("."), &completions);
	checkdirs(path, completions);
	ft_popblk(path);
	return (completions);
}

static inline void	matchfiles(const char *pattern, DIR *dir, t_list **cmp)
{
	struct dirent	*data;
	size_t			plen;

	if (!dir)
		return ;
	data = readdir(dir);
	plen = ft_strlen(pattern);
	while (data)
	{
		if (!ft_strequals(data->d_name, ".")
			&& !ft_strequals(data->d_name, "..")
			&& !ft_strncmp(data->d_name, pattern, plen))
			ft_lstadd_back(cmp, ft_lstnew(ft_strdup(data->d_name)));
		data = readdir(dir);
	}
	closedir(dir);
}

static inline void	checkdirs(const char *path, t_list *completions)
{
	t_list	*tmp;
	uint8_t	ptype;

	tmp = completions;
	ptype = ft_strequals(path, "/");
	while (tmp)
	{
		if (path)
		{
			ft_popblk(tmp->blk);
			if (ptype)
				tmp->blk = ft_push(ft_strjoin(path, tmp->blk));
			else
				tmp->blk = ft_push(ft_strsjoin(path, tmp->blk, '/'));
		}
		if (ft_rl_isdir(tmp->blk))
		{
			ft_popblk(tmp->blk);
			tmp->blk = ft_push(ft_strjoin(tmp->blk, "/"));
		}
		tmp = tmp->next;
	}
}
