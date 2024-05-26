/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/27 02:18:42 by ivalimak         ###   ########.fr       */
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

rl_input_t	g_input;

const char	*g_hlcolor;

const t_list	*g_hist;
const t_list	*g_hist_cur;
/** globals **/

static inline uint64_t	_plen(const char *p);
static inline uint8_t	_getinput(void);
static inline char	*_getline(const char *p);

#include "ft_stdio/ft_printf.h"

char	*ft_readline(const char *p, uint8_t opts)
{
	struct termios	old;
	struct termios	new;
	char			*out;

	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(0, TCSANOW, &new);
	ft_rl_init();
	ft_printf("rows: %d\ncols: %d\n", g_rows, g_cols);
	g_hist_cur = NULL;
	if (!(opts & FT_RL_HIST_OFF))
		g_hist_cur = g_hist;
	out = _getline(p);
	tcsetattr(0, TCSANOW, &old);
	return (out);
	(void)opts;
	(void)p;
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
	g_input.key = 0;
	if (read(0, &g_input.key, sizeof(g_input.key)) == -1)
		exit(ft_rl_perror());
	g_input.keystr = ft_rl_keystr(g_input.key);
	return (ft_rl_execmap(&g_input));
}

static inline char	*_getline(const char *p)
{
	ft_memcpy(&g_input, &(rl_input_t){.line = NULL, .prompt = p, .keystr = NULL,
			.exittype = ACL, .cursor = ft_rl_cursor_init(), .plen = _plen(p),
			.len = 0, .key = 0}, sizeof(g_input));
	if (g_hist_cur)
		g_input.line = (char *)((rl_histnode_t *)g_hist_cur->blk)->line;
	else
		g_input.line = ft_push(ft_strdup(""));
	ft_putstr_fd(p, 1);
	ft_rl_cursor_getpos(&g_input.cursor->row, &g_input.cursor->col);
	while (_getinput())
		;
	ft_popblk(g_input.line);
	if (g_input.exittype == EOF)
		return (NULL);
	return (ft_strdup(g_input.line));
}
