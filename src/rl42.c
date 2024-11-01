/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

/** globals **/
__t_hmap	*g_keys;
__t_hmap	*g_funcs;
__t_hmap	*g_map_emacs;
__t_hmap	*g_map_vi_ins;
__t_hmap	*g_map_vi_cmd;
__t_hmap	*g_map_hlcolor;

int16_t	g_rows;
int16_t	g_cols;

uint64_t	g_maxlen;
uint64_t	g_status;

rl_hlc_t	g_hlcolor;

rl_arg_t	g_argument;

rl_mark_t	g_mark_s;
rl_mark_t	g_mark_e;
rl_mark_t	g_mark_u;

rl_input_t	g_input;

rl_keybuf_t	g_keybuf;

rl_cmp_fn_t	g_cmp_fn;

rl_escapes_t	g_escapes;

const __t_list	*g_hist;
const __t_list	*g_hist_cur;
const __t_list	*g_kill_ring;
const __t_list	*g_blocks;

rl_settings_t	g_settings;

struct termios	g_oldsettings;
struct termios	g_newsettings;
/** globals **/

static inline uint64_t	_plen(const char *p);
static inline uint8_t	_getinput(void);
static inline char	*_strdup(const char *s);
static inline char	*_getline(const char *p);
static inline void	_histcommit(const char *line, const uint8_t opts);
static inline void	_rl_signal(const uint8_t mode);
static inline void	_rl_sigaction(const int signal);

char	*ft_readline(const char *p, const uint8_t opts)
{
	char			*out;

	__push(p);
	ft_rl_init();
	g_hist_cur = NULL;
	tcsetattr(0, TCSANOW, &g_newsettings);
	ft_ti_tputs(g_escapes.civis, 1, ft_rl_putc);
	if (!(opts & FT_RL_HIST_OFF) && ft_rl_get(_HIST_SIZE_HASH) > 0)
	{
		ft_rl_hist_newnode();
		g_hist_cur = g_hist;
	}
	_rl_signal(SET_ON);
	out = _getline(p);
	_rl_signal(SET_OFF);
	if (!(opts & FT_RL_HIST_OFF) && ft_rl_get(_HIST_SIZE_HASH) > 0)
		_histcommit(out, opts);
	tcsetattr(0, TCSANOW, &g_oldsettings);
	ft_ti_tputs(g_escapes.cnorm, 1, ft_rl_putc);
	__popblk(p);
	return (_strdup(out));
}

static inline uint64_t	_plen(const char *p)
{
	uint64_t	len;

	len = 0;
	while (*p)
	{
		if (*p == '\e')
		{
			while (*p && *p != 'm')
				p++;
			p++;
		}
		if (*p == '\e')
			continue ;
		if (!*p)
			break ;
		len++;
		p++;
	}
	return (len);
}

static inline uint8_t	_getinput(void)
{
	g_argument.set = 0;
	g_input.key = ft_rl_getkey();
	return (ft_rl_execmap(&g_input));
}

static inline char	*_strdup(const char *s)
{
	char	*out;

	if (!s)
		return (NULL);
	out = malloc(__getblksize(s));
	if (!out)
		return (NULL);
	__strlcpy(out, s, __getblksize(s));
	return (out);
}

static inline char	*_getline(const char *p)
{
	ft_rl_init_input(p, _plen(p));
	ft_ti_tputs(p, 1, ft_rl_putc);
	ft_rl_cursor_getpos(&g_input.cursor->row, &g_input.cursor->col);
	g_input.cursor->i_row = g_input.cursor->row;
	g_input.cursor->i_col = g_input.cursor->col;
	while (_getinput())
		;
	__popblk(g_input.line);
	if (g_input.exittype == E_EOF)
		return (NULL);
	return (__strdup(g_input.line));
}

static inline void	_histcommit(const char *line, const uint8_t opts)
{
	rl_histnode_t	*node;

	if (opts & FT_RL_HIST_NOUP || g_input.exittype == E_EOF || !*line)
		ft_rl_hist_rmnode();
	else
	{
		node = (rl_histnode_t *)g_hist->blk;
		if (node)
		{
			__popblk(node->line);
			node->line = __push(__strdup(line));
			if (!node->line)
				exit(ft_rl_perror());
		}
	}
	ft_rl_hist_restore();
}

static inline void	_rl_signal(const uint8_t mode)
{
	static struct sigaction	act;
	static struct sigaction	orig;
	static uint8_t			init = 0;

	if (!init)
	{
		act.sa_handler = _rl_sigaction;
		act.sa_flags = 0;
	}
	switch (mode)
	{
		case SET_ON:
			sigaction(SIGWINCH, &act, &orig);
			break ;
		case SET_OFF:
			sigaction(SIGWINCH, &orig, NULL);
	}
}

static inline void	_rl_sigaction(const int signal)
{
	switch (signal)
	{
		case SIGWINCH:
			ft_rl_updatetermsize();
			ft_rl_redisplay(&g_input, CLEAR);
			ft_rl_redisplay(&g_input, PROMPT);
	}
}
