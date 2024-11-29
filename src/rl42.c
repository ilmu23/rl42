/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 17:12:30 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

/** globals **/
int16_t	g_rows;
int16_t	g_cols;

uint64_t	g_maxlen;
uint64_t	g_status;

rl_hlc_t	g_hlcolor;

rl_arg_t	g_argument;

__t_hmap	*g_funcs;
__t_hmap	*g_binds;
__t_hmap	*g_macrodata[3];

rl_mark_t	g_mark_s;
rl_mark_t	g_mark_e;
rl_mark_t	g_mark_u;

rl_input_t	g_input;

rl_fninfo_t	g_macro;

rl_cmp_fn_t	g_cmp_fn;

rl_escapes_t	g_escapes;

const __t_list	*g_hist;
const __t_list	*g_hist_cur;
const __t_list	*g_hist_init;
const __t_list	*g_kill_ring;
const __t_list	*g_blocks;

rl_settings_t	g_settings;

struct termios	g_oldsettings;
struct termios	g_newsettings;

const rl_keytree_t	g_emptynode;

const char	*g_fn_names[_FNCOUNT] = { "self-insert", "remove-char", "backward-remove-char",
	"beginning-of-line", "end-of-line", "forward-char", "backward-char", "end-of-word", "beginning-of-word",
	"forward-word", "backward-word", "clear-screen", "clear-display", "forward-find-character", "backward-find-character",
	"accept-line", "end-of-file", "next-history", "previous-history", "end-of-history", "beginning-of-history",
	"forward-search-history", "reverse-search-history", "inc-forward-search-history", "inc-reverse-search-history",
	"yank-nth-arg", "yank-last-arg", "upcase-word", "downcase-word", "capitalize-word", "forward-kill-line",
	"backward-kill-line", "kill-whole-line", "forward-kill-word", "backward-kill-word", "kill-region",
	"delete-horizontal-space", "copy-region-as-kill", "copy-forward-word", "copy-backward-word", "yank",
	"yank-pop", "transpose-characters", "transpose-words", "complete", "re-read-init-file", "discard-line",
	"digit-argument", "negative-digit-argument", "abort", "set-mark", "unset-mark", "exchange-point-and-mark",
	"emacs-editing-mode", "vi-editing-mode", "vi-command-mode", "vi-insert-mode-a", "vi-insert-mode-A",
	"vi-insert-mode-i", "vi-insert-mode-I", "vi-subst", "vi-subst-line", "vi-delete", "vi-replace",
	"set-highlight-color", "hlcolor-toggle-bold", "hlcolor-toggle-underline", "hlcolor-toggle-fg/bg",
	"hlcolor-set-sgr", "hlcolor-set-color", "hlcolor-set-rgb", "hlcolor-accept", "dump-functions",
	"dump-variables", "dump-macros", "operate-and-get-next"};
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
	ft_ti_tputs(g_escapes.smkx, 1, ft_rl_putc);
	ft_ti_tputs(TERM_BPM_ON, 1, ft_rl_putc);
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
	ft_ti_tputs(TERM_BPM_OFF, 1, ft_rl_putc);
	ft_ti_tputs(g_escapes.rmkx, 1, ft_rl_putc);
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
	rl_fn_t	f;

	g_argument.set = 0;
	f = ft_rl_getinput(&g_input.keyseq);
	return (f) ? f(&g_input) : 1;
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
	if (g_hist_init) {
		g_input.line = __push(__strdup(((rl_histnode_t *)g_hist_init->blk)->line));
		if (!g_input.line)
			exit(ft_rl_perror());
		g_input.i = g_input.len = strlen(g_input.line);
		ft_rl_redisplay(&g_input, INPUT);
		ft_rl_cursor_reset(&g_input);
		g_hist_cur = g_hist_init;
		g_hist_init = NULL;
	}
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
