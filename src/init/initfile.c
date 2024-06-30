/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 15:03:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/30 16:49:25 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

static inline uint64_t	_getval(const char *var, const char *val);
static inline char		*_expand(const char *path);
static inline void		_readfile(const char *path);
static inline void		_parse(char *line);
static inline void		_bind(const char **args);
static inline void		_set(const char **args);

uint64_t	g_lnr;

void	ft_rl_read_initfile(void)
{
	const char	*path;

	g_status ^= _READ_INITFILE;
	path = _expand(getenv("RL42RC"));
	if (!path)
	{
		path = _expand(_USER_INITFILE);
		if (access(path, F_OK) != 0)
			path = _expand(_USER_INITFILE);
		if (access(path, F_OK) != 0)
			path = NULL;
	}
	_readfile(path);
	g_status ^= _READ_INITFILE;
}

static inline char	*_expand(const char *path)
{
	if (path && *path == '~')
		return (ft_strsjoin(getenv("HOME"), &path[1], '/'));
	return ((char *)path);
}

static inline uint64_t	_getval(const char *var, const char *val)
{
	switch (ft_strhash(var, 347, UINT64_MAX))
	{
		case _BSTYLE_HASH:
			if (ft_strequals(val, "none"))
				return (BELL_NONE);
			if (ft_strequals(val, "visible"))
				return (BELL_VISIBLE);
			if (ft_strequals(val, "audible"))
				return (BELL_AUDIBLE);
			ft_dprintf(2, "rl42: init: set: unrecognized value: '%s'\n", val, g_lnr);
			return (BELL_NONE);
		case _CMP_DWIDTH_HASH:
			if (ft_isint(val, sizeof(int64_t)))
				return (ft_atoi64(val));
			ft_dprintf(2, "rl42: init: set: unrecognized value: '%s'\n", val, g_lnr);
			return (-1);
		case _CMP_QITEMS_HASH:
			if (ft_isint(val, sizeof(int64_t)))
				return (ft_atoi64(val));
			ft_dprintf(2, "rl42: init: set: unrecognized value: '%s'\n", val, g_lnr);
			return (100);
		case _HIST_SIZE_HASH:
			if (ft_isint(val, sizeof(int64_t)))
				return (ft_atoi64(val));
			return (500);
		case _CMP_CASE_HASH:
		case _CMP_MCASE_HASH:
		case _CMP_ENABLE_HASH:
		case _CMP_MDIRS_HASH:
		case _CMP_MLDIRS_HASH:
		case _CMP_HFILES_HASH:
		case _CMP_HLIGHT_HASH:
			if (ft_strequals(ft_strlower(ft_strdup(val)), "on"))
				return (SET_ON);
			return (SET_OFF);
	}
	return (SET_OFF);
}

static inline void	_readfile(const char *path)
{
	int32_t	fd;
	char	*line;

	if (!path)
		return ;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		goto _err;
	g_lnr = 1;
	line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r"));
	while (line)
	{
		if (*line && *line != '#')
			_parse(line);
		ft_popblk(line);
		g_lnr++;
		line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r"));
	}
	close(fd);
	return ;
	_err:
		ft_rl_perror();
}

static inline void	_parse(char *line)
{
	const char	**args;
	size_t		i;

	i = 0;
	while (line[i])
	{
		if (ft_strchr("\t\n\v\f\r", line[i]))
			line[i] = ' ';
		i++;
	}
	args = ft_pusharr(ft_split(line, ' '));
	if (!args)
		exit(ft_rl_perror());
	if (ft_strequals(args[0], "bind"))
		_bind(args);
	else if (ft_strequals(args[0], "set"))
		_set(args);
	else
		ft_dprintf(2, "rl42: init: unrecognized keyword: '%s'\n", args[0], g_lnr);
	i = 0;
	while (args[i])
		ft_popblk(args[i++]);
	ft_popblk(args);
}

static inline void	_bind(const char **args)
{
	if (!args[1])
	{
		ft_dprintf(2, "rl42: init: bind: missing keycode\n", g_lnr);
		return ;
	}
	if (!args[2])
	{
		ft_dprintf(2, "rl42: init: bind: missing function\n", g_lnr);
		return ;
	}
	ft_rl_map(args[1], args[2], REMAP);
}

static inline void	_set(const char **args)
{
	if (!args[1])
	{
		ft_dprintf(2, "rl42: init: set: missing variable name\n", g_lnr);
		return ;
	}
	if (!args[2])
	{
		ft_dprintf(2, "rl42: init: set: missing value\n", g_lnr);
		return ;
	}
	ft_rl_set(args[1], _getval(args[1], (args[2])));
}
