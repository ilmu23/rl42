/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:50:39 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/08 14:00:00 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_emacs_default_binds(void);
static inline void	_vi_ins_default_binds(void);
static inline void	_vi_cmd_default_binds(void);
static inline void	_defaultsettings(void);
static inline void	_ft_rl_exit(void);

void	ft_rl_init(void)
{
	static uint8_t	init = 0;
	uint8_t			emode;

	if (init)
		return ;
	g_keys = ft_mapnew();
	g_funcs = ft_mapnew();
	g_map_emacs = ft_mapnew();
	g_map_vi_ins = ft_mapnew();
	g_map_vi_cmd = ft_mapnew();
	tcgetattr(0, &g_oldsettings);
	g_newsettings = g_oldsettings;
	g_newsettings.c_iflag &= ~(ICRNL | IXON);
	g_newsettings.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(0, TCSANOW, &g_newsettings);
	if (!g_keys || !g_funcs || !g_map_emacs || !g_map_vi_ins || !g_map_vi_cmd
		|| atexit(_ft_rl_exit))
		ft_exit(ft_rl_perror());
	g_argument.set = 0;
	g_mark_s.set = 0;
	g_mark_e.set = 0;
	g_mark_u.set = 0;
	ft_putstr_fd(TERM_CUR_HIDE, 1);
	ft_rl_updatetermsize();
	ft_rl_hist_load(_FT_RL_HFILE);
	ft_rl_initkeys();
	ft_rl_initfuncs();
	_defaultsettings();
	ft_rl_read_initfile();
	emode = ft_rl_geteditmode();
	_emacs_default_binds();
	_vi_ins_default_binds();
	_vi_cmd_default_binds();
	g_hlcolor.mode = FT_RL_HL_FG;
	ft_rl_sethlcolor_sgr(SGR_FG4);
	ft_rl_sethlcolor_rgb(255, 23, 123);
	ft_rl_seteditmode(emode);
	tcsetattr(0, TCSANOW, &g_oldsettings);
	init = 1;
}

void	ft_rl_init_input(const char *p, const uint64_t plen)
{
	ft_memcpy(&g_input, &(rl_input_t){.line = NULL, .prompt = p, .keystr = NULL,
			.exittype = ACL, .cursor = ft_rl_cursor_init(), .plen = plen,
			.len = 0, .key = 0, .i = 0}, sizeof(g_input));
	if (g_hist_cur)
		g_input.line = (char *)((rl_histnode_t *)g_hist_cur->blk)->line;
	else
		g_input.line = ft_push(ft_strdup(""));
	if (!g_input.cursor || !g_input.line)
		exit(ft_rl_perror());
}

static inline void	_emacs_default_binds(void)
{
	static char		key[2] = "!";
	static uint8_t	val = KEY_BANG;

	if (ft_rl_geteditmode() != _MD_EMACS)
		ft_rl_seteditmode(_MD_EMACS);
	ft_rl_map("<SPC>", "self-insert", QUIET);
	while (val <= KEY_TILDE)
	{
		ft_rl_map(key, "self-insert", QUIET);
		*key = ++val;
	}
	ft_rl_map("<DEL>", "remove-char", QUIET);
	ft_rl_map("<BCK>", "backward-remove-char", QUIET);
	ft_rl_map("<C-a>", "beginning-of-line", QUIET);
	ft_rl_map("<C-e>", "end-of-line", QUIET);
	ft_rl_map("<HME>", "beginning-of-line", QUIET);
	ft_rl_map("<END>", "end-of-line", QUIET);
	ft_rl_map("<left>", "backward-char", QUIET);
	ft_rl_map("<right>", "forward-char", QUIET);
	ft_rl_map("<U-left>", "beginning-of-word", QUIET);
	ft_rl_map("<U-right>", "end-of-word", QUIET);
	ft_rl_map("<C-b>", "backward-char", QUIET);
	ft_rl_map("<C-f>", "forward-char", QUIET);
	ft_rl_map("<M-b>", "backward-word", QUIET);
	ft_rl_map("<M-f>", "forward-word", QUIET);
	ft_rl_map("<C-l>", "clear-screen", QUIET);
	ft_rl_map("<M-,>", "forward-find-character", QUIET);
	ft_rl_map("<M-;>", "backward-find-character", QUIET);
	ft_rl_map("<RET>", "accept-line", QUIET);
	ft_rl_map("<C-d>", "end-of-file", QUIET);
	ft_rl_map("<C-p>", "previous-history", QUIET);
	ft_rl_map("<C-n>", "next-history", QUIET);
	ft_rl_map("<up>", "previous-history", QUIET);
	ft_rl_map("<down>", "next-history", QUIET);
	ft_rl_map("<M-<>", "beginning-of-history", QUIET);
	ft_rl_map("<M->>", "end-of-history", QUIET);
	ft_rl_map("<M-r>", "reverse-search-history", QUIET);
	ft_rl_map("<M-s>", "forward-search-history", QUIET);
	ft_rl_map("<C-r>", "inc-reverse-search-history", QUIET);
	ft_rl_map("<C-h>", "inc-forward-search-history", QUIET);
	ft_rl_map("<M-C-y>", "yank-nth-arg", QUIET);
	ft_rl_map("<M-.>", "yank-last-arg", QUIET);
	ft_rl_map("<M-u>", "upcase-word", QUIET);
	ft_rl_map("<M-l>", "downcase-word", QUIET);
	ft_rl_map("<M-c>", "capitalize-word", QUIET);
	ft_rl_map("<C-k>", "forward-kill-line", QUIET);
	ft_rl_map("<M-k>", "backward-kill-line", QUIET);
	ft_rl_map("<M-K>", "kill-whole-line", QUIET);
	ft_rl_map("<M-DEL>", "forward-kill-word", QUIET);
	ft_rl_map("<M-BCK>", "backward-kill-word", QUIET);
	ft_rl_map("<M-U-DEL>", "kill-region", QUIET);
	ft_rl_map("<M-\\>", "delete-horizontal-space", QUIET);
	ft_rl_map("<C-t>", "transpose-characters", QUIET);
	ft_rl_map("<M-t>", "transpose-words", QUIET);
	ft_rl_map("<TAB>", "complete", QUIET);
	ft_rl_map("<ESC>", "prefix-meta", QUIET);
	ft_rl_map("<C-c>", "discard-line", QUIET);
	ft_rl_map("<M-0>", "digit-argument", QUIET);
	ft_rl_map("<M-1>", "digit-argument", QUIET);
	ft_rl_map("<M-2>", "digit-argument", QUIET);
	ft_rl_map("<M-3>", "digit-argument", QUIET);
	ft_rl_map("<M-4>", "digit-argument", QUIET);
	ft_rl_map("<M-5>", "digit-argument", QUIET);
	ft_rl_map("<M-6>", "digit-argument", QUIET);
	ft_rl_map("<M-7>", "digit-argument", QUIET);
	ft_rl_map("<M-8>", "digit-argument", QUIET);
	ft_rl_map("<M-9>", "digit-argument", QUIET);
	ft_rl_map("<M-->", "negative-digit-argument", QUIET);
	ft_rl_map("<C-g>", "abort", QUIET);
	ft_rl_map("<M-SPC>", "set-mark", QUIET);
	ft_rl_map("<C-@>", "set-mark", QUIET);
	ft_rl_map("<M-C-@>", "unset-mark", QUIET);
	ft_rl_map("<M-C-x>", "exchange-point-and-mark", QUIET);
	ft_rl_map("<M-C-e>", "vi-editing-mode", QUIET);
	ft_rl_map("<M-h>", "set-highlight-color", QUIET);
	ft_rl_map("<M-H>", "toggle-highlight-mode", QUIET);
}

static inline void	_vi_ins_default_binds(void)
{
	static char		key[2] = "!";
	static uint8_t	val = KEY_BANG;

	ft_rl_seteditmode(_MD_VI_INS);
	ft_rl_map("<SPC>", "self-insert", QUIET);
	while (val <= KEY_TILDE)
	{
		ft_rl_map(key, "self-insert", QUIET);
		*key = ++val;
	}
	ft_rl_map("<C-c>", "discard-line", QUIET);
	ft_rl_map("<C-d>", "end-of-file", QUIET);
	ft_rl_map("<C-l>", "clear-screen", QUIET);
	ft_rl_map("<C-r>", "inc-reverse-search-history", QUIET);
	ft_rl_map("<C-s>", "inc-forward-search-history", QUIET);
	ft_rl_map("<C-t>", "transpose-characters", QUIET);
	ft_rl_map("<DEL>", "remove-char", QUIET);
	ft_rl_map("<BCK>", "backward-remove-char", QUIET);
	ft_rl_map("<ESC>", "vi-command-mode", QUIET);
	ft_rl_map("<RET>", "accept-line", QUIET);
	ft_rl_map("<TAB>", "complete", QUIET);
}

static inline void	_vi_cmd_default_binds(void)
{
	ft_rl_seteditmode(_MD_VI_CMD);
	ft_rl_map("<C-c>", "discard-line", QUIET);
	ft_rl_map("<C-d>", "end-of-file", QUIET);
	ft_rl_map("<C-e>", "emacs-editing-mode", QUIET);
	ft_rl_map("<C-g>", "abort", QUIET);
	ft_rl_map("<C-l>", "clear-screen", QUIET);
	ft_rl_map("<C-r>", "inc-reverse-search-history", QUIET);
	ft_rl_map("<C-s>", "inc-forward-search-history", QUIET);
	ft_rl_map("<C-t>", "transpose-characters", QUIET);
	ft_rl_map("<SPC>", "forward-char", QUIET);
	ft_rl_map("$", "end-of-line", QUIET);
	ft_rl_map(",", "exchange-point-and-mark", QUIET);
	ft_rl_map("/", "inc-forward-search-history", QUIET);
	ft_rl_map("0", "beginning-of-line", QUIET);
	ft_rl_map("1", "digit-argument", QUIET);
	ft_rl_map("2", "digit-argument", QUIET);
	ft_rl_map("3", "digit-argument", QUIET);
	ft_rl_map("4", "digit-argument", QUIET);
	ft_rl_map("5", "digit-argument", QUIET);
	ft_rl_map("6", "digit-argument", QUIET);
	ft_rl_map("7", "digit-argument", QUIET);
	ft_rl_map("8", "digit-argument", QUIET);
	ft_rl_map("9", "digit-argument", QUIET);
	ft_rl_map("?", "inc-reverse-search-history", QUIET);
	ft_rl_map("A", "vi-insert-mode-A", QUIET);
	ft_rl_map("F", "backward-find-character", QUIET);
	ft_rl_map("I", "vi-insert-mode-I", QUIET);
	ft_rl_map("M", "set-mark", QUIET);
	ft_rl_map("S", "vi-subst-line", QUIET);
	ft_rl_map("X", "backward-remove-char", QUIET);
	ft_rl_map("Z", "discard-line", QUIET);
	ft_rl_map("_", "yank-last-arg", QUIET);
	ft_rl_map("a", "vi-insert-mode-a", QUIET);
	ft_rl_map("b", "backward-word", QUIET);
	ft_rl_map("d", "vi-delete", QUIET);
	ft_rl_map("f", "forward-find-character", QUIET);
	ft_rl_map("h", "backward-char", QUIET);
	ft_rl_map("i", "vi-insert-mode-i", QUIET);
	ft_rl_map("j", "next-history", QUIET);
	ft_rl_map("k", "previous-history", QUIET);
	ft_rl_map("l", "forward-char", QUIET);
	ft_rl_map("m", "set-mark", QUIET);
	ft_rl_map("r", "vi-replace", QUIET);
	ft_rl_map("s", "vi-subst", QUIET);
	ft_rl_map("w", "forward-word", QUIET);
	ft_rl_map("x", "remove-char", QUIET);
	ft_rl_map("<RET>", "accept-line", QUIET);
	ft_rl_map("<TAB>", "complete", QUIET);
	ft_rl_map("<ESC>", "abort", QUIET);
}

static inline void	_defaultsettings(void)
{
	ft_rl_seteditmode(_MD_EMACS);
	ft_rl_set("bell-style", BELL_NONE);
	ft_rl_set("completion-display-width", -1);
	ft_rl_set("completion-ignore-case", SET_OFF);
	ft_rl_set("completion-map-case", SET_OFF);
	ft_rl_set("completion-query-items", 100);
	ft_rl_set("enable-completion", SET_ON);
	ft_rl_set("history-size", 500);
	ft_rl_set("mark-directories", SET_ON);
	ft_rl_set("mark-symlinked-directories", SET_OFF);
	ft_rl_set("match-hidden-files", SET_ON);
	ft_rl_set("highlight-current-completion", SET_ON);
}

static inline void	_ft_rl_exit(void)
{
	if (g_hist)
		ft_rl_hist_save(_FT_RL_HFILE);
	tcsetattr(0, TCSANOW, &g_oldsettings);
	ft_putstr_fd(TERM_CUR_SHOW, 1);
	ft_return(42);
}
