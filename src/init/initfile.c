/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 15:03:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/08 09:10:51 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

#define __E_INIT "rl42: init:"
#define __E_BIND "rl42: init: bind:"
#define __E_SET "rl42: init: set:"

static inline uint64_t	_getval(const char *var, const char *val);
static inline char		*_expand(const char *path);
static inline void		_readfile(const char *path);
static inline void		_parse(char *line);
static inline void		_bind(const char **args);
static inline void		_set(const char **args);
static inline void		_set_emode(const char *emode);

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
		return (ft_strjoin(getenv("HOME"), &path[1]));
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
			ft_dprintf(2, "%s unrecognized value: '%s'\n", __E_SET, val);
			return (BELL_NONE);
		case _CMP_DWIDTH_HASH:
			if (ft_isint(val, sizeof(int64_t)))
				return (ft_atoi64(val));
			ft_dprintf(2, "%s unrecognized value: '%s'\n", __E_SET, val);
			return (-1);
		case _CMP_QITEMS_HASH:
			if (ft_isint(val, sizeof(int64_t)))
				return (ft_atoi64(val));
			ft_dprintf(2, "%s unrecognized value: '%s'\n", __E_SET, val);
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
	line = ft_push(ft_strtrim(get_next_line(fd), "\t\n\v\f\r"));
	while (line)
	{
		if (*line && *line != '#')
			_parse(line);
		ft_popblk(line);
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
		ft_dprintf(2, "%s unrecognized keyword: '%s'\n", __E_INIT, args[0]);
	i = 0;
	while (args[i])
		ft_popblk(args[i++]);
	ft_popblk(args);
}

static inline void	_bind(const char **args)
{
	if (!args[1])
	{
		ft_dprintf(2, "%s missing keycode\n", __E_BIND);
		return ;
	}
	if (!args[2])
	{
		ft_dprintf(2, "%s missing function\n", __E_BIND);
		return ;
	}
	if (args[3])
	{
		if (ft_strequals(args[3], "vi-ins"))
			ft_rl_map_vi_ins(args[1], args[2], REMAP);
		else if (ft_strequals(args[3], "vi-cmd"))
			ft_rl_map_vi_cmd(args[1], args[2], REMAP);
		else if (ft_strequals(args[3], "emacs"))
			ft_rl_map_emacs(args[1], args[2], REMAP);
		else
			ft_dprintf(2, "%s unrecognized mode: '%s'\n", __E_BIND, args[3]);
		return ;
	}
	ft_rl_map(args[1], args[2], REMAP);
}

static inline void	_set(const char **args)
{
	if (!args[1])
	{
		ft_dprintf(2, "%s missing variable name\n", __E_SET);
		return ;
	}
	if (!args[2])
	{
		ft_dprintf(2, "%s missing value\n", __E_SET);
		return ;
	}
	if (ft_strhash(args[1], 347, UINT64_MAX) == _EMODE_HASH)
		_set_emode(args[2]);
	else
		ft_rl_set(args[1], _getval(args[1], (args[2])));
}

static inline void	_set_emode(const char *emode)
{
	if (ft_strequals(emode, "vi"))
		ft_rl_seteditmode(_MD_VI_INS);
	else if (ft_strequals(emode, "emacs"))
		ft_rl_seteditmode(_MD_EMACS);
	else
		ft_dprintf(2, "%s editing-mode: unrecognized mode: '%s'\n", __E_SET, emode);
}
