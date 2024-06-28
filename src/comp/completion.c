/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:07:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/28 18:36:53 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline t_list	*_complete(const char *pattern);
static inline t_list	*_increment(t_list *completions, const char *pattern);
static inline char		*_uncase(char *fname);
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
		pattern = ft_push(ft_substr(input->line, g_mark_s.pos, g_mark_e.pos - g_mark_s.pos));
	}
	if (ft_rl_get(_CMP_CASE_HASH))
		pattern = _uncase((char *)pattern);
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
	const char	*pat;

	out = NULL;
	path = ft_strrchr(pattern, '/');
	if (path)
	{
		path = ft_substr(pattern, 0, path - pattern);
		if (ft_strequals(path, ""))
			path = ft_strdup("/");
		pat = ft_strrchr(pattern, '/') + 1;
		_match(pat, opendir(ft_push(path)), (const t_list **)&out);
	}
	else
		_match(pattern, opendir("."), (const t_list **)&out);
	_buildpath(path, out);
	ft_popblk(path);
	return (_increment(out, pattern));
}

static inline t_list	*_increment(t_list *completions, const char *pattern)
{
	char		*common;
	t_list		*start;
	uint64_t	maxlen;
	uint8_t		nocase;
	uint8_t		cur;
	size_t		i;

	if (!completions || !completions->next)
		return (completions);
	maxlen = ft_rl_comp_getlongest(completions);
	common = ft_calloc(maxlen + 1, sizeof(*common));
	if (!common)
		exit(ft_rl_perror());
	i = 0;
	nocase = (ft_rl_get(_CMP_CASE_HASH) != 0);
	if (nocase)
		nocase += (ft_rl_get(_CMP_MCASE_HASH) != 0);
	cur = ((uint8_t *)completions->blk)[i];
	switch (nocase)
	{
		case 2:
			if (cur == '-')
				cur = '_';
			__attribute__ ((fallthrough));
		case 1:
			cur = ft_tolower(cur);
	}
	start = completions;
	while (cur)
	{
		completions = completions->next;
		if (!completions)
		{
			common[i++] = cur;
			completions = start;
			cur = ((uint8_t *)completions->blk)[i];
			switch (nocase)
			{
				case 2:
					if (cur == '-')
						cur = '_';
					__attribute__ ((fallthrough));
				case 1:
					cur = ft_tolower(cur);
			}
			continue ;
		}
		switch (nocase)
		{
			case 2:
				if (cur == '_' && ((uint8_t *)completions->blk)[i] == '-')
					continue ;
				__attribute__ ((fallthrough));
			case 1:
				if (ft_tolower(((uint8_t *)completions->blk)[i]) == cur)
					continue ;
		}
		if (((uint8_t *)completions->blk)[i] == cur)
			continue ;
		break ;
	}
	if (!*common || ft_strequals(common, pattern))
		return (start);
	ft_popblk(completions->blk);
	completions->blk = ft_push(common);
	ft_lstpopall(completions->next);
	completions->next = NULL;
	return (completions);
}

static inline char	*_uncase(char *fname)
{
	size_t	i;

	fname = ft_strlower(fname);
	if (ft_rl_get(_CMP_MCASE_HASH))
	{
		i = 0;
		while (fname[i])
		{
			if (fname[i] == '-')
				fname[i] = '_';
			i++;
		}
	}
	return (fname);
}

static inline void	_match(const char *pattern, DIR *dir, const t_list **completions)
{
	struct dirent	*data;
	const uint64_t	plen = ft_strlen(pattern);
	const char		*fname;

	if (!dir)
		return ;
	data = readdir(dir);
	while (data)
	{
		if (ft_rl_get(_CMP_CASE_HASH))
			fname = _uncase(ft_strdup(data->d_name));
		else
			fname = ft_strdup(data->d_name);
		if (!ft_strequals(fname, ".")
			&& !ft_strequals(fname, "..")
			&& !ft_strncmp(fname, pattern, plen)
			&& (ft_rl_get(_CMP_HFILES_HASH) || *fname != '.'))
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
		if (ft_rl_isdir(completions->blk) && ft_rl_get(_CMP_MDIRS_HASH))
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

	subs[0] = ft_push(ft_substr(input->line, 0, g_mark_s.pos));
	subs[1] = ft_push(ft_substr(input->line, g_mark_e.pos, input->len - g_mark_e.pos));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	ft_popblk(input->line);
	input->line = ft_push(ft_strnjoin(3, subs[0], completion, subs[1]));
	if (!input->line)
		exit(ft_rl_perror());
	ft_popblks(2, subs[0], subs[1]);
	g_mark_e.pos = g_mark_s.pos + ft_strlen(completion);
	input->len = ft_strlen(input->line);
	input->i = g_mark_e.pos;
	ft_rl_redisplay(input, INPUT);
}

static inline void	_replace_mult(rl_input_t *input, const t_list *completions)
{
	const void	*prv;

	prv = NULL;
	if (!ft_rl_comp_display(input, ft_lstfirst(completions), NULL, prv))
		return ;
	input->key = ft_rl_getkey();
	while (ft_rl_getmap(input->key) == ft_rl_cmp)
	{
		_replace(input, completions->blk);
		if (ft_rl_get(_CMP_HLIGHT_HASH))
			ft_rl_comp_display(input, ft_lstfirst(completions), completions->blk, prv);
		prv = completions->blk;
		if (completions->next)
			completions = completions->next;
		else
			completions = ft_lstfirst(completions);
		input->key = ft_rl_getkey();
	}
	ft_rl_redisplay(input, CLEAR);
	ft_rl_clearblocks();
}
