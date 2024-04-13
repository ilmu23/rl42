/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 15:03:14 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 21:47:49 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline t_rl_wc	*expand_path(char **patterns, size_t dpth);
static inline uint8_t	checkpattern(char *pat, char *name);
static inline void		matchpatterns(t_rl_wc *cur, t_rl_wc *prv, size_t dpth);
static inline void		match(char *pat, char *path, t_list **mtc, uint8_t any);

t_rl_wc	*ft_rl_wildcard_expand(const char *pattern)
{
	t_rl_wc		*matches;
	size_t		depth;
	const char	*tmp;

	depth = 0;
	tmp = ft_strchr(pattern, '/');
	while (tmp)
	{
		tmp++;
		if (*tmp && tmp - 1 != pattern)
			depth++;
		tmp = ft_strchr(tmp, '/');
	}
	matches = expand_path(ft_pusharr(ft_split(pattern, '/')), depth);
	ft_rl_wc_rmdot(matches->matches);
	return (matches);
}

static inline t_rl_wc	*expand_path(char **patterns, size_t dpth)
{
	size_t	i;
	t_rl_wc	*prv;
	t_rl_wc	*cur;

	prv = NULL;
	cur = ft_push(ft_calloc(1, sizeof(*cur)));
	if (!ft_rl_wc_checkalloc(cur, prv, patterns))
		return (NULL);
	i = 0;
	cur->pattern = patterns[i++];
	match(cur->pattern, ".", &cur->matches, !dpth);
	while (--dpth + 1)
	{
		prv = cur;
		cur = ft_push(ft_calloc(1, sizeof(*cur)));
		if (!ft_rl_wc_checkalloc(cur, prv, patterns))
			return (NULL);
		cur->pattern = patterns[i++];
		matchpatterns(cur, prv, dpth);
		ft_popblks(2, prv, prv->pattern);
		ft_lstpopall(ft_lstfirst(prv->matches));
	}
	ft_popblk(patterns);
	return (cur);
}

static inline uint8_t	checkpattern(char *pat, char *name)
{
	char	**sstrings;

	if ((*name == '.' && * pat!= '.')
		|| (*pat != '*'
			&& ft_strncmp(name, pat, ft_strclen(pat, '*'))))
		return (0);
	sstrings = ft_split(pat, '*');
	if (!sstrings)
		return (0);
	name += (*pat == '*');
	while (*sstrings)
	{
		name = ft_strnstr(name, *sstrings, ft_strlen(name));
		if (!name)
			return (0);
		name += ft_strlen(*sstrings++);
	}
	if (*name && pat[ft_strlen(pat) - 1] != '*')
		return (0);
	return (1);
}

static inline void	matchpatterns(t_rl_wc *cur, t_rl_wc *prv, size_t dpth)
{
	while (prv->matches)
	{
		match(cur->pattern, prv->matches->blk,
			&cur->matches, !dpth);
		prv->matches = prv->matches->next;
	}
}

static inline void	match(char *pat, char *path, t_list **mtc, uint8_t any)
{
	DIR				*dir;
	struct dirent	*data;

	dir = opendir(path);
	if (!dir)
		return ;
	data = readdir(dir);
	while (data)
	{
		if ((!ft_strequals(data->d_name, ".")
				&& !ft_strequals(data->d_name, ".."))
			|| ft_strequals(data->d_name, pat))
		{
			if (checkpattern(pat, data->d_name) && (any
					|| ft_rl_isdir(ft_strsjoin(path, data->d_name, '/'))))
				ft_lstadd_back(mtc, ft_lstnew(ft_strsjoin(path,
							data->d_name, '/')));
		}
		data = readdir(dir);
	}
	closedir(dir);
}
