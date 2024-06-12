/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 22:38:08 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

/** globals **/
t_hmap	*g_keys;
t_hmap	*g_funcs;
t_hmap	*g_maps;

int16_t	g_rows;
int16_t	g_cols;

uint64_t	g_maxlen;
uint64_t	g_mark_s;
uint64_t	g_mark_e;

rl_hlc_t	g_hlcolor;

rl_arg_t	g_argument;

rl_input_t	g_input;

rl_keybuf_t	g_keybuf;

const t_list	*g_hist;
const t_list	*g_hist_cur;
const t_list	*g_blocks;

struct termios	g_oldsettings;
struct termios	g_newsettings;
/** globals **/

static inline uint64_t	_plen(const char *p);
static inline uint8_t	_getinput(void);
static inline char	*_getline(const char *p);
static inline void	_histcommit(const char *line, const uint8_t opts);

char	*ft_readline(const char *p, const uint8_t opts)
{
	char			*out;

	ft_push(p);
	ft_rl_init();
	g_hist_cur = NULL;
	tcsetattr(0, TCSANOW, &g_newsettings);
	ft_putstr_fd(TERM_CUR_HIDE, 1);
	if (!(opts & FT_RL_HIST_OFF))
	{
		ft_rl_hist_newnode();
		g_hist_cur = g_hist;
	}
	out = _getline(p);
	if (!(opts & FT_RL_HIST_OFF))
		_histcommit(out, opts);
	tcsetattr(0, TCSANOW, &g_oldsettings);
	ft_putstr_fd(TERM_CUR_SHOW, 1);
	ft_popblk(p);
	return (out);
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
	g_input.key = ft_rl_getkey();
	g_input.keystr = ft_rl_keystr(g_input.key);
	return (ft_rl_execmap(&g_input));
}

static inline char	*_getline(const char *p)
{
	ft_rl_init_input(p, _plen(p));
	ft_putstr_fd(p, 1);
	ft_rl_cursor_getpos(&g_input.cursor->row, &g_input.cursor->col);
	g_input.cursor->i_row = g_input.cursor->row;
	g_input.cursor->i_col = g_input.cursor->col;
	while (_getinput())
		;
	ft_popblk(g_input.line);
	if (g_input.exittype == EOF)
		return (NULL);
	return (ft_strdup(g_input.line));
}

static inline void	_histcommit(const char *line, const uint8_t opts)
{
	rl_histnode_t	*node;

	if (opts & FT_RL_HIST_NOUP || g_input.exittype == EOF || !*line)
		ft_rl_hist_rmnode();
	else
	{
		node = (rl_histnode_t *)g_hist->blk;
		if (node)
		{
			ft_popblk(node->line);
			node->line = ft_push(ft_strdup(line));
			if (!node->line)
				exit(ft_rl_perror());
		}
	}
	ft_rl_hist_restore();
}
