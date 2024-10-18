/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:07:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:07:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline t_list	*_increment(t_list *completions, const char *pattern);
static inline char		*_uncase(char *fname);
static inline void		_match(const char *pattern, DIR *dir, const t_list **completions);
static inline void		_buildpath(const char *path, t_list *completions);
static inline void		_replace(rl_input_t *input, const char *completion);
static inline void		_replace_mult(rl_input_t *input, const t_list *completions);

t_list	*ft_rl_complete_path(const char *pattern, __UNUSED const rl_input_t *context)
{
	t_list		*out;
	const char	*path;
	const char	*pat;

	out = NULL;
	path = strrchr(pattern, '/');
	if (path)
	{
		path = __substr(pattern, 0, path - pattern);
		if (__strequals(path, ""))
			path = __strdup("/");
		pat = strrchr(pattern, '/') + 1;
		_match(pat, opendir(__push(path)), (const t_list **)&out);
	}
	else
		_match(pattern, opendir("."), (const t_list **)&out);
	_buildpath(path, out);
	__popblk(path);
	return (_increment(out, pattern));
}

void	ft_rl_complete(rl_input_t *input)
{
	const char		*pattern;
	const t_list	*completions;

	if (isspace(input->line[input->i])
		&& isspace(input->line[MAX(input->i - 1, 0)]))
		ft_rl_setmark(_MARK_START | _MARK_END);
	else
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		input->i = MIN(input->i + 1, input->len);
		ft_rl_setmark(_MARK_END);
	}
	pattern = __push(__substr(input->line, g_mark_s.pos, g_mark_e.pos - g_mark_s.pos));
	if (ft_rl_get(_CMP_CASE_HASH))
		pattern = _uncase((char *)pattern);
	completions = ft_rl_get_completion_fn()(pattern, input);
	if (!completions)
		ft_rl_cursor_reset(input);
	else if (!completions->next)
		_replace(input, completions->blk);
	else
		_replace_mult(input, completions);
	ft_rl_unsetmark(_MARK_START | _MARK_END);
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
	common = __calloc(maxlen + 1, sizeof(*common));
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
			cur = tolower(cur);
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
					cur = tolower(cur);
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
				if (tolower(((uint8_t *)completions->blk)[i]) == cur)
					continue ;
		}
		if (((uint8_t *)completions->blk)[i] == cur)
			continue ;
		break ;
	}
	if (!*common || __strequals(common, pattern))
		return (start);
	__popblk(completions->blk);
	completions->blk = __push(common);
	__lstpopall(completions->next);
	completions->next = NULL;
	return (completions);
}

static inline char	*_uncase(char *fname)
{
	size_t	i;

	fname = __strlower(fname);
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
	const uint64_t	plen = strlen(pattern);
	const char		*fname;

	if (!dir)
		return ;
	data = readdir(dir);
	while (data)
	{
		if (ft_rl_get(_CMP_CASE_HASH))
			fname = _uncase(__strdup(data->d_name));
		else
			fname = __strdup(data->d_name);
		if (!__strequals(fname, ".")
			&& !__strequals(fname, "..")
			&& !strncmp(fname, pattern, plen)
			&& (ft_rl_get(_CMP_HFILES_HASH) || *fname != '.'))
			__lstadd_back(completions, __lstnew(__strdup(data->d_name)));
		data = readdir(dir);
	}
	closedir(dir);
}

static inline void	_buildpath(const char *path, t_list *completions)
{
	const uint8_t	ptype = __strequals(path, "/");

	while (completions)
	{
		if (path)
		{
			__popblk(completions->blk);
			if (ptype)
				completions->blk = __push(__strjoin(path, completions->blk));
			else
				completions->blk = __push(__strsjoin(path, completions->blk, '/'));
		}
		if (ft_rl_isdir(completions->blk) && ft_rl_get(_CMP_MDIRS_HASH))
		{
			__popblk(completions->blk);
			completions->blk = __push(__strjoin(completions->blk, "/"));
		}
		completions = completions->next;
	}
}

static inline void	_replace(rl_input_t *input, const char *completion)
{
	const char	*subs[2];

	subs[0] = __push(__substr(input->line, 0, g_mark_s.pos));
	subs[1] = __push(__substr(input->line, g_mark_e.pos, input->len - g_mark_e.pos));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	__popblk(input->line);
	input->line = __push(__strnjoin(3, subs[0], completion, subs[1]));
	if (!input->line)
		exit(ft_rl_perror());
	__popblks(2, subs[0], subs[1]);
	g_mark_e.pos = g_mark_s.pos + strlen(completion);
	input->len = strlen(input->line);
	input->i = g_mark_e.pos;
	ft_rl_redisplay(input, INPUT);
}

static inline void	_replace_mult(rl_input_t *input, const t_list *completions)
{
	const void	*prv;

	prv = NULL;
	if (!ft_rl_comp_display(input, __lstfirst(completions), NULL, prv))
		return ;
	input->key = ft_rl_getkey();
	while (ft_rl_getmap(input->key) == ft_rl_cmp)
	{
		_replace(input, completions->blk);
		if (ft_rl_get(_CMP_HLIGHT_HASH))
			ft_rl_comp_display(input, __lstfirst(completions), completions->blk, prv);
		prv = completions->blk;
		if (completions->next)
			completions = completions->next;
		else
			completions = __lstfirst(completions);
		input->key = ft_rl_getkey();
	}
	ft_rl_redisplay(input, CLEAR);
	ft_rl_clearblocks();
}
