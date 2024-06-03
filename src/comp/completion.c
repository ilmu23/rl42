/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:07:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/03 15:27:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline t_list	*_complete(const char *pattern);
static inline void		_match(const char *pattern, DIR *dir, const t_list **completions);
static inline void		_buildpath(const char *path, t_list *completions);
static inline void		_replace(rl_input_t *input, const char *completion);
static inline void		_replace_mult(rl_input_t *input, const t_list *completions);

void	ft_rl_complete(rl_input_t *input)
{
	const char		*pattern;
	const t_list	*completions;

	if (ft_isspace(input->line[input->i]))
	{
		ft_rl_setmark(_MARK_START | _MARK_END);
		pattern = ft_push(ft_strdup(""));
	}
	else
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		pattern = ft_push(ft_substr(input->line, g_mark_s, g_mark_e - g_mark_s));
	}
	completions = _complete(pattern);
	if (!completions)
		ft_rl_cursor_reset(input);
	else if (!completions->next)
		_replace(input, completions->blk);
	else
		_replace_mult(input, completions);
	ft_rl_unsetmark(_MARK_START | _MARK_END);
}

static inline t_list	*_complete(const char *pattern)
{
	t_list		*out;
	const char	*path;

	out = NULL;
	path = ft_strrchr(pattern, '/');
	if (path)
	{
		path = ft_substr(pattern, 0, path - pattern);
		if (ft_strequals(path, ""))
			path = ft_strdup("/");
		pattern = ft_strrchr(pattern, '/') + 1;
		_match(pattern, opendir(ft_push(path)), (const t_list **)&out);
	}
	else
		_match(pattern, opendir("."), (const t_list **)&out);
	_buildpath(path, out);
	ft_popblk(path);
	return (out);
}

static inline void	_match(const char *pattern, DIR *dir, const t_list **completions)
{
	struct dirent	*data;
	const uint64_t	plen = ft_strlen(pattern);

	if (!dir)
		return ;
	data = readdir(dir);
	while (data)
	{
		if (!ft_strequals(data->d_name, ".")
			&& !ft_strequals(data->d_name, "..")
			&& !ft_strncmp(data->d_name, pattern, plen))
			ft_lstadd_back(completions, ft_lstnew(ft_strdup(data->d_name)));
		data = readdir(dir);
	}
	closedir(dir);
}

static inline void	_buildpath(const char *path, t_list *completions)
{
	const uint8_t	ptype = ft_strequals(path, "/");

	while (completions)
	{
		if (path)
		{
			ft_popblk(completions->blk);
			if (ptype)
				completions->blk = ft_push(ft_strjoin(path, completions->blk));
			else
				completions->blk = ft_push(ft_strsjoin(path, completions->blk, '/'));
		}
		if (ft_rl_isdir(completions->blk))
		{
			ft_popblk(completions->blk);
			completions->blk = ft_push(ft_strjoin(completions->blk, "/"));
		}
		completions = completions->next;
	}
}

static inline void	_replace(rl_input_t *input, const char *completion)
{
	const char	*subs[2];

	subs[0] = ft_push(ft_substr(input->line, 0, g_mark_s));
	subs[1] = ft_push(ft_substr(input->line, g_mark_e, input->len - g_mark_e));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	ft_popblk(input->line);
	input->line = ft_push(ft_strnjoin(3, subs[0], completion, subs[1]));
	if (!input->line)
		exit(ft_rl_perror());
	ft_popblks(2, subs[0], subs[1]);
	g_mark_e = g_mark_s + ft_strlen(completion);
	input->len = ft_strlen(input->line);
	input->i = g_mark_e;
	ft_rl_redisplay(input, ALL);
}

static inline void	_replace_mult(rl_input_t *input, const t_list *completions)
{
	ft_rl_comp_display(input, ft_lstfirst(completions), NULL);
	input->key = ft_rl_getkey();
	while (ft_rl_getmap(input->key) == ft_rl_cmp)
	{
		_replace(input, completions->blk);
		ft_rl_comp_display(input, ft_lstfirst(completions), completions->blk);
		if (completions->next)
			completions = completions->next;
		else
			completions = ft_lstfirst(completions);
		input->key = ft_rl_getkey();
	}
	ft_rl_redisplay(input, ALL);
}
