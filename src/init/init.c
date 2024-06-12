/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:50:39 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 23:12:52 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_ft_rl_defaultbinds(void);
static inline void	_rl_exit(void);

void	ft_rl_init(void)
{
	static uint8_t	init = 0;

	if (init)
		return ;
	g_keys = ft_mapnew();
	g_funcs = ft_mapnew();
	g_maps = ft_mapnew();
	tcgetattr(0, &g_oldsettings);
	g_newsettings = g_oldsettings;
	g_newsettings.c_iflag &= ~(ICRNL | IXON);
	g_newsettings.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(0, TCSANOW, &g_newsettings);
	if (!g_keys || !g_funcs || !g_maps)
		ft_exit(ft_rl_perror());
	if (atexit(_rl_exit))
		ft_exit(ft_rl_perror());
	g_argument.set = 0;
	ft_putstr_fd(TERM_CUR_HIDE, 1);
	ft_rl_updatetermsize();
	ft_rl_hist_load(_FT_RL_HFILE);
	ft_rl_initkeys();
	ft_rl_initfuncs();
	_ft_rl_defaultbinds();
	g_hlcolor.mode = FT_RL_HL_FG;
	ft_rl_sethlcolor_sgr(SGR_FG4);
	ft_rl_sethlcolor_rgb(255, 23, 123);
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

static inline void	_ft_rl_defaultbinds(void)
{
	static char		key[2] = "!";
	static uint8_t	val = KEY_BANG;

	ft_rl_map("<SPC>", "self-insert", QREMAP);
	while (val <= KEY_TILDE)
	{
		ft_rl_map(key, "self-insert", QREMAP);
		*key = ++val;
	}
	ft_rl_map("<DEL>", "remove-char", QREMAP);
	ft_rl_map("<BCK>", "backward-remove-char", QREMAP);
	ft_rl_map("<C-a>", "beginning-of-line", QREMAP);
	ft_rl_map("<C-e>", "end-of-line", QREMAP);
	ft_rl_map("<HME>", "beginning-of-line", QREMAP);
	ft_rl_map("<END>", "end-of-line", QREMAP);
	ft_rl_map("<left>", "backward-char", QREMAP);
	ft_rl_map("<right>", "forward-char", QREMAP);
	ft_rl_map("<U-left>", "beginning-of-word", QREMAP);
	ft_rl_map("<U-right>", "end-of-word", QREMAP);
	ft_rl_map("<C-b>", "backward-char", QREMAP);
	ft_rl_map("<C-f>", "forward-char", QREMAP);
	ft_rl_map("<M-b>", "backward-word", QREMAP);
	ft_rl_map("<M-f>", "forward-word", QREMAP);
	ft_rl_map("<C-l>", "clear-screen", QREMAP);
	ft_rl_map("<M-.>", "forward-find-character", QREMAP);
	ft_rl_map("<M-,>", "backward-find-character", QREMAP);
	ft_rl_map("<RET>", "accept-line", QREMAP);
	ft_rl_map("<C-d>", "end-of-file", QREMAP);
	ft_rl_map("<C-p>", "previous-history", QREMAP);
	ft_rl_map("<C-n>", "next-history", QREMAP);
	ft_rl_map("<up>", "previous-history", QREMAP);
	ft_rl_map("<down>", "next-history", QREMAP);
	ft_rl_map("<M-<>", "beginning-of-history", QREMAP);
	ft_rl_map("<M->>", "end-of-history", QREMAP);
	ft_rl_map("<M-r>", "reverse-search-history", QREMAP);
	ft_rl_map("<M-s>", "forward-search-history", QREMAP);
	ft_rl_map("<C-r>", "inc-reverse-search-history", QREMAP);
	ft_rl_map("<C-h>", "inc-forward-search-history", QREMAP);
	ft_rl_map("<M-u>", "upcase-word", QREMAP);
	ft_rl_map("<M-l>", "downcase-word", QREMAP);
	ft_rl_map("<M-c>", "capitalize-word", QREMAP);
	ft_rl_map("<C-k>", "forward-kill-line", QREMAP);
	ft_rl_map("<M-k>", "backward-kill-line", QREMAP);
	ft_rl_map("<M-K>", "kill-whole-line", QREMAP);
	ft_rl_map("<M-DEL>", "forward-kill-word", QREMAP);
	ft_rl_map("<M-BCK>", "backward-kill-word", QREMAP);
	ft_rl_map("<C-t>", "transpose-characters", QREMAP);
	ft_rl_map("<M-t>", "transpose-words", QREMAP);
	ft_rl_map("<TAB>", "complete", QREMAP);
	ft_rl_map("<ESC>", "prefix-meta", QREMAP);
	ft_rl_map("<C-c>", "discard-line", QREMAP);
	ft_rl_map("<M-0>", "digit-argument", QREMAP);
	ft_rl_map("<M-1>", "digit-argument", QREMAP);
	ft_rl_map("<M-2>", "digit-argument", QREMAP);
	ft_rl_map("<M-3>", "digit-argument", QREMAP);
	ft_rl_map("<M-4>", "digit-argument", QREMAP);
	ft_rl_map("<M-5>", "digit-argument", QREMAP);
	ft_rl_map("<M-6>", "digit-argument", QREMAP);
	ft_rl_map("<M-7>", "digit-argument", QREMAP);
	ft_rl_map("<M-8>", "digit-argument", QREMAP);
	ft_rl_map("<M-9>", "digit-argument", QREMAP);
	ft_rl_map("<M-->", "negative-digit-argument", QREMAP);
	ft_rl_map("<M-h>", "set-highlight-color", QREMAP);
	ft_rl_map("<M-H>", "toggle-highlight-mode", QREMAP);
}

static inline void	_rl_exit(void)
{
	if (g_hist)
		ft_rl_hist_save(_FT_RL_HFILE);
	tcsetattr(0, TCSANOW, &g_oldsettings);
	ft_putstr_fd(TERM_CUR_SHOW, 1);
	ft_return(42);
}
