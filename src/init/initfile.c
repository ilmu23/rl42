/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 15:03:15 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/12 18:21:25 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define __E_INIT "rl42: init:"
#define __E_CBIND "rl42: init: const bind:"
#define __E_BIND "rl42: init: bind:"
#define __E_SET "rl42: init: set:"

static inline uint64_t	_getval(const char *var, const char *val);
static inline char		*_expand(const char *path);
static inline void		_readfile(const char *path);
static inline void		_parse(char *line);
static inline void		_cbind(const char **args);
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
			path = _SYSTEM_INITFILE;
		if (access(path, F_OK) != 0)
			path = NULL;
	}
	_readfile(path);
	g_status ^= _READ_INITFILE;
}

static inline char	*_expand(const char *path)
{
	if (path && *path == '~')
		return (__strjoin(getenv("HOME"), &path[1]));
	return ((char *)path);
}

static inline uint64_t	_getval(const char *var, const char *val)
{
	switch (__strhash(var, 347, UINT64_MAX))
	{
		case _BSTYLE_HASH:
			if (__strequals(val, "none"))
				return (BELL_NONE);
			if (__strequals(val, "visible"))
				return (BELL_VISIBLE);
			if (__strequals(val, "audible"))
				return (BELL_AUDIBLE);
#ifndef RL42NOCOMPLAIN
			__dprintf(2, "%s unrecognized value: '%s'\n", __E_SET, val);
#endif
			return (BELL_NONE);
		case _CMP_DWIDTH_HASH:
			if (__isint(val))
				return (atol(val));
#ifndef RL42NOCOMPLAIN
			__dprintf(2, "%s unrecognized value: '%s'\n", __E_SET, val);
#endif
			return (-1);
		case _CMP_QITEMS_HASH:
			if (__isint(val))
				return (atol(val));
#ifndef RL42NOCOMPLAIN
			__dprintf(2, "%s unrecognized value: '%s'\n", __E_SET, val);
#endif
			return (100);
		case _HIST_SIZE_HASH:
			if (__isint(val))
				return (atol(val));
			return (500);
		case _KEY_TMEOUT_HASH:
			if (__isint(val))
				return (atol(val));
			return (0);
		case _CMP_CASE_HASH:
		case _CMP_MCASE_HASH:
		case _CMP_ENABLE_HASH:
		case _CMP_MDIRS_HASH:
		case _CMP_MLDIRS_HASH:
		case _CMP_HFILES_HASH:
		case _CMP_HLIGHT_HASH:
			if (__strequals(__strlower(__strdup(val)), "on"))
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
	line = __push(__strtrim(__getline(fd), "\t\n\v\f\r"));
	while (line)
	{
		if (*line && *line != '#')
			_parse(line);
		__popblk(line);
		line = __push(__strtrim(__getline(fd), "\t\n\v\f\r"));
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
		if (strchr("\t\n\v\f\r", line[i]))
			line[i] = ' ';
		i++;
	}
	args = __pusharr(__qsplit(line, ' '));
	if (!args)
		exit(ft_rl_perror());
	if (__strequals(args[0], "const") && __strequals(args[1], "bind"))
		_cbind(&args[1]);
	else if (__strequals(args[0], "bind"))
		_bind(args);
	else if (__strequals(args[0], "set"))
		_set(args);
#ifndef RL42NOCOMPLAIN
	else
		__dprintf(2, "%s unrecognized keyword: '%s'\n", __E_INIT, args[0]);
#endif
	i = 0;
	while (args[i])
		__popblk(args[i++]);
	__popblk(args);
}

static inline void	_cbind(const char **args)
{
	if (!args[1]) {
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "%s missing keyseq\n", __E_CBIND);
#endif
		return ;
	}
	if (!args[2]) {
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "%s missing function\n", __E_CBIND);
#endif
		return ;
	}
	if (args[3]) {
		if (__strequals(args[3], "vi-ins"))
			ft_rl_const_bind_vi_ins(args[1], args[2]);
		else if (__strequals(args[3], "vi-cmd"))
			ft_rl_const_bind_vi_cmd(args[1], args[2]);
		else if (__strequals(args[3], "emacs"))
			ft_rl_const_bind_emacs(args[1], args[2]);
		else if (__strequals(args[3], "hlcolor"))
			ft_rl_const_bind_hlcolor(args[1], args[2]);
#ifndef RL42NOCOMPLAIN
		else
			__dprintf(2, "%s unrecognized mode: '%s'\n", __E_CBIND, args[3]);
#endif
		return ;
	}
	ft_rl_const_bind(args[1], args[2]);
}

static inline void	_bind(const char **args)
{
	if (!args[1])
	{
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "%s missing keyseq\n", __E_BIND);
#endif
		return ;
	}
	if (!args[2])
	{
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "%s missing function\n", __E_BIND);
#endif
		return ;
	}
	if (args[3])
	{
		if (__strequals(args[3], "vi-ins"))
			ft_rl_bind_vi_ins(args[1], args[2], QREMAP);
		else if (__strequals(args[3], "vi-cmd"))
			ft_rl_bind_vi_cmd(args[1], args[2], QREMAP);
		else if (__strequals(args[3], "emacs"))
			ft_rl_bind_emacs(args[1], args[2], QREMAP);
		else if (__strequals(args[3], "hlcolor"))
			ft_rl_bind_hlcolor(args[1], args[2], QREMAP);
#ifndef RL42NOCOMPLAIN
		else
			__dprintf(2, "%s unrecognized mode: '%s'\n", __E_BIND, args[3]);
#endif
		return ;
	}
	ft_rl_bind(args[1], args[2], QREMAP);
}

static inline void	_set(const char **args)
{
	if (!args[1])
	{
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "%s missing variable name\n", __E_SET);
#endif
		return ;
	}
	if (!args[2])
	{
#ifndef RL42NOCOMPLAIN
		__dprintf(2, "%s missing value\n", __E_SET);
#endif
		return ;
	}
	if (__strhash(args[1], 347, UINT64_MAX) == _EMODE_HASH)
		_set_emode(args[2]);
	else
		ft_rl_set(args[1], _getval(args[1], (args[2])));
}

static inline void	_set_emode(const char *emode)
{
	if (__strequals(emode, "vi"))
		ft_rl_seteditmode(_MD_VI_INS);
	else if (__strequals(emode, "emacs"))
		ft_rl_seteditmode(_MD_EMACS);
#ifndef RL42NOCOMPLAIN
	else
		__dprintf(2, "%s editing-mode: unrecognized mode: '%s'\n", __E_SET, emode);
#endif
}
