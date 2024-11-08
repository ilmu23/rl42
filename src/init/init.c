/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:50:39 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/08 18:51:53 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define newbindmap(x)	(__mapadd(g_binds, x, __calloc(1, sizeof(rl_keytree_t))))

static inline void	_const_binds(void);
static inline void	_emacs_default_binds(void);
static inline void	_vi_ins_default_binds(void);
static inline void	_vi_cmd_default_binds(void);
static inline void	_hlcolor_default_binds(void);
static inline void	_ssi_binds(void);
static inline void	_defaultsettings(void);
static inline void	_init_escapes(void);
static inline void	_ft_rl_exit(void);

void	ft_rl_init(void)
{
	static uint8_t	init = 0;
	uint8_t			emode;

	if (init)
		return ;
	init = 1;
	_init_escapes();
	g_funcs = __mapnew();
	g_binds = __mapnew();
	if (!g_funcs || !g_binds)
		__exit(ft_rl_perror());
	tcgetattr(0, &g_oldsettings);
	g_newsettings = g_oldsettings;
	g_newsettings.c_iflag &= ~(ICRNL | IXON);
	g_newsettings.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(0, TCSANOW, &g_newsettings);
	if (!newbindmap("emacs") || !newbindmap("vi-ins") || !newbindmap("vi-cmd")
		|| !newbindmap("hlcolor") || !newbindmap("__ssi")  || atexit(_ft_rl_exit))
		__exit(ft_rl_perror());
	g_argument.set = 0;
	g_mark_s.set = 0;
	g_mark_e.set = 0;
	g_mark_u.set = 0;
	ft_ti_tputs(g_escapes.civis, 1, ft_rl_putc);
	ft_rl_updatetermsize();
	ft_rl_hist_load(_FT_RL_HFILE);
	ft_rl_initfuncs();
	_defaultsettings();
	_const_binds();
	ft_rl_read_initfile();
	emode = ft_rl_geteditmode();
	_emacs_default_binds();
	_vi_ins_default_binds();
	_vi_cmd_default_binds();
	_hlcolor_default_binds();
	_ssi_binds();
	ft_rl_sethlcolor_mode(FT_RL_HL_FG);
	ft_rl_sethlcolor_clr(2);
	ft_rl_seteditmode(emode);
	ft_rl_set_completion_fn(ft_rl_complete_path);
	tcsetattr(0, TCSANOW, &g_oldsettings);
}

void	ft_rl_init_input(const char *p, const uint64_t plen)
{
	memcpy(&g_input, &(rl_input_t){.line = NULL, .prompt = p, .keyseq = NULL,
			.exittype = E_ACL, .cursor = ft_rl_cursor_init(), .plen = plen,
			.len = 0, .i = 0}, sizeof(g_input));
	if (g_hist_cur)
		g_input.line = (char *)((rl_histnode_t *)g_hist_cur->blk)->line;
	else
		g_input.line = __push(__strdup(""));
	if (!g_input.cursor || !g_input.line)
		exit(ft_rl_perror());
}

static inline void	_const_binds(void)
{
	ft_rl_const_bind_emacs("\e[200~", "__paste");
	ft_rl_const_bind_vi_cmd("\e[200~", "__paste");
	ft_rl_const_bind_vi_ins("\e[200~", "__paste");
	ft_rl_const_bind_hlcolor("\e[200~", "__paste");
}

static inline void	_emacs_default_binds(void)
{
	char	seq[2] = "!";
	uint8_t	c;

	ft_rl_seteditmode(_MD_EMACS);
	ft_rl_bind("<SPC>", "self-insert", QUIET);
	ft_rl_bind("\\<", "self-insert", QUIET);
	ft_rl_bind("\\\\", "self-insert", QUIET);
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		ft_rl_bind(seq, "self-insert", QUIET);
	ft_rl_bind("<DEL>", "remove-char", QUIET);
	ft_rl_bind("<BCK>", "backward-remove-char", QUIET);
	ft_rl_bind("<C-a>", "beginning-of-line", QUIET);
	ft_rl_bind("<HME>", "beginning-of-line", QUIET);
	ft_rl_bind("<C-e>", "end-of-line", QUIET);
	ft_rl_bind("<END>", "end-of-line", QUIET);
	ft_rl_bind("<RIGHT>", "forward-char", QUIET);
	ft_rl_bind("<C-f>", "forward-char", QUIET);
	ft_rl_bind("<LEFT>", "backward-char", QUIET);
	ft_rl_bind("<C-b>", "backward-char", QUIET);
	ft_rl_bind("<S-RIGHT>", "end-of-word", QUIET);
	ft_rl_bind("<S-LEFT>", "beginning-of-word", QUIET);
	ft_rl_bind("<M-f>", "forward-word", QUIET);
	ft_rl_bind("<M-b>", "backward-word", QUIET);
	ft_rl_bind("<C-l>", "clear-screen", QUIET);
	ft_rl_bind("<M-C-l>", "clear-display", QUIET);
	ft_rl_bind("<M-,>", "forward-find-character", QUIET);
	ft_rl_bind("<M-;>", "backward-find-character", QUIET);
	ft_rl_bind("<RET>", "accept-line", QUIET);
	ft_rl_bind("<C-d>", "end-of-file", QUIET);
	ft_rl_bind("<DOWN>", "next-history", QUIET);
	ft_rl_bind("<C-n>", "next-history", QUIET);
	ft_rl_bind("<UP>", "previous-history", QUIET);
	ft_rl_bind("<C-p>", "previous-history", QUIET);
	ft_rl_bind("<M-\\>>", "end-of-history", QUIET);
	ft_rl_bind("<M-<>", "beginning-of-history", QUIET);
	ft_rl_bind("<M-s>", "forward-search-history", QUIET);
	ft_rl_bind("<M-r>", "reverse-search-history", QUIET);
	ft_rl_bind("<C-h>", "inc-forward-search-history", QUIET);
	ft_rl_bind("<C-r>", "inc-reverse-search-history", QUIET);
	ft_rl_bind("<M-C-y>", "yank-nth-arg", QUIET);
	ft_rl_bind("<M-.>", "yank-last-arg", QUIET);
	ft_rl_bind("<M-u>", "upcase-word", QUIET);
	ft_rl_bind("<M-l>", "downcase-word", QUIET);
	ft_rl_bind("<M-c>", "capitalize-word", QUIET);
	ft_rl_bind("<C-k>", "forward-kill-line", QUIET);
	ft_rl_bind("<M-k>", "backward-kill-line", QUIET);
	ft_rl_bind("<M-K>", "kill-whole-line", QUIET);
	ft_rl_bind("<C-k>w", "forward-kill-word", QUIET);
	ft_rl_bind("<M-k>w", "backward-kill-word", QUIET);
	ft_rl_bind("<C-k>r", "kill-region", QUIET);
	ft_rl_bind("<M-\\\\>", "delete-horizontal-space", QUIET);
	ft_rl_bind("<M-C>", "copy-region-as-kill", QUIET);
	ft_rl_bind("<M-B>", "copy-backward-word", QUIET);
	ft_rl_bind("<M-F>", "copy-forward-word", QUIET);
	ft_rl_bind("<C-y>", "yank", QUIET);
	ft_rl_bind("<M-y>", "yank-pop", QUIET);
	ft_rl_bind("<C-t>", "transpose-characters", QUIET);
	ft_rl_bind("<M-t>", "transpose-words", QUIET);
	ft_rl_bind("<TAB>", "complete", QUIET);
	ft_rl_bind("<C-x><C-r>", "re-read-init-file", QUIET);
	ft_rl_bind("<C-c>", "discard-line", QUIET);
	ft_rl_bind("<M-0>", "digit-argument", QUIET);
	ft_rl_bind("<M-1>", "digit-argument", QUIET);
	ft_rl_bind("<M-2>", "digit-argument", QUIET);
	ft_rl_bind("<M-3>", "digit-argument", QUIET);
	ft_rl_bind("<M-4>", "digit-argument", QUIET);
	ft_rl_bind("<M-5>", "digit-argument", QUIET);
	ft_rl_bind("<M-6>", "digit-argument", QUIET);
	ft_rl_bind("<M-7>", "digit-argument", QUIET);
	ft_rl_bind("<M-8>", "digit-argument", QUIET);
	ft_rl_bind("<M-9>", "digit-argument", QUIET);
	ft_rl_bind("<M-->", "negative-digit-argument", QUIET);
	ft_rl_bind("<C-g>", "abort", QUIET);
	ft_rl_bind("<M-x>", "set-mark", QUIET);
	ft_rl_bind("<M-X>", "unset-mark", QUIET);
	ft_rl_bind("<M-C-x>", "exchange-point-and-mark", QUIET);
	ft_rl_bind("<M-C-e>", "vi-editing-mode", QUIET);
	ft_rl_bind("<M-h>", "set-highlight-color", QUIET);
}

static inline void	_vi_ins_default_binds(void)
{
	char	seq[2] = "!";
	uint8_t	c;

	ft_rl_seteditmode(_MD_VI_INS);
	ft_rl_bind("<SPC>", "self-insert", QUIET);
	ft_rl_bind("\\<", "self-insert", QUIET);
	ft_rl_bind("\\\\", "self-insert", QUIET);
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		ft_rl_bind(seq, "self-insert", QUIET);
	ft_rl_bind("<C-c>", "discard-line", QUIET);
	ft_rl_bind("<C-d>", "end-of-file", QUIET);
	ft_rl_bind("<C-g>", "abort", QUIET);
	ft_rl_bind("<C-l>", "clear-screen", QUIET);
	ft_rl_bind("<M-C-l>", "clear-display", QUIET);
	ft_rl_bind("<C-r>", "inc-reverse-search-history", QUIET);
	ft_rl_bind("<C-s>", "inc-forward-search-history", QUIET);
	ft_rl_bind("<C-t>", "transpose-characters", QUIET);
	ft_rl_bind("<C-y>", "yank-last-arg", QUIET);
	ft_rl_bind("<UP>", "previous-history", QUIET);
	ft_rl_bind("<DOWN>", "next-history", QUIET);
	ft_rl_bind("<DEL>", "remove-char", QUIET);
	ft_rl_bind("<BCK>", "backward-remove-char", QUIET);
	ft_rl_bind("<ESC>", "vi-command-mode", QUIET);
	ft_rl_bind("<RET>", "accept-line", QUIET);
	ft_rl_bind("<TAB>", "complete", QUIET);
}

static inline void	_vi_cmd_default_binds(void)
{
	ft_rl_seteditmode(_MD_VI_CMD);
	ft_rl_bind("<C-c>", "discard-line", QUIET);
	ft_rl_bind("<C-d>", "end-of-file", QUIET);
	ft_rl_bind("<C-e>", "emacs-editing-mode", QUIET);
	ft_rl_bind("<C-g>", "abort", QUIET);
	ft_rl_bind("<C-l>", "clear-screen", QUIET);
	ft_rl_bind("<M-C-l>", "clear-display", QUIET);
	ft_rl_bind("<C-r>", "inc-reverse-search-history", QUIET);
	ft_rl_bind("<C-s>", "inc-forward-search-history", QUIET);
	ft_rl_bind("<C-t>", "transpose-characters", QUIET);
	ft_rl_bind("<SPC>", "forward-char", QUIET);
	ft_rl_bind("<M-r>", "re-read-init-file", QUIET);
	ft_rl_bind("$", "end-of-line", QUIET);
	ft_rl_bind(",", "exchange-point-and-mark", QUIET);
	ft_rl_bind("/", "inc-forward-search-history", QUIET);
	ft_rl_bind("0", "beginning-of-line", QUIET);
	ft_rl_bind("1", "digit-argument", QUIET);
	ft_rl_bind("2", "digit-argument", QUIET);
	ft_rl_bind("3", "digit-argument", QUIET);
	ft_rl_bind("4", "digit-argument", QUIET);
	ft_rl_bind("5", "digit-argument", QUIET);
	ft_rl_bind("6", "digit-argument", QUIET);
	ft_rl_bind("7", "digit-argument", QUIET);
	ft_rl_bind("8", "digit-argument", QUIET);
	ft_rl_bind("9", "digit-argument", QUIET);
	ft_rl_bind("?", "inc-reverse-search-history", QUIET);
	ft_rl_bind("A", "vi-insert-mode-A", QUIET);
	ft_rl_bind("F", "backward-find-character", QUIET);
	ft_rl_bind("I", "vi-insert-mode-I", QUIET);
	ft_rl_bind("M", "unset-mark", QUIET);
	ft_rl_bind("P", "yank-pop", QUIET);
	ft_rl_bind("S", "vi-subst-line", QUIET);
	ft_rl_bind("X", "backward-remove-char", QUIET);
	ft_rl_bind("Z", "discard-line", QUIET);
	ft_rl_bind("_", "yank-last-arg", QUIET);
	ft_rl_bind("a", "vi-insert-mode-a", QUIET);
	ft_rl_bind("b", "backward-word", QUIET);
	ft_rl_bind("c", "set-highlight-color", QUIET);
	ft_rl_bind("d", "vi-delete", QUIET);
	ft_rl_bind("f", "forward-find-character", QUIET);
	ft_rl_bind("h", "backward-char", QUIET);
	ft_rl_bind("i", "vi-insert-mode-i", QUIET);
	ft_rl_bind("j", "next-history", QUIET);
	ft_rl_bind("k", "previous-history", QUIET);
	ft_rl_bind("l", "forward-char", QUIET);
	ft_rl_bind("m", "set-mark", QUIET);
	ft_rl_bind("p", "yank", QUIET);
	ft_rl_bind("r", "vi-replace", QUIET);
	ft_rl_bind("s", "vi-subst", QUIET);
	ft_rl_bind("w", "forward-word", QUIET);
	ft_rl_bind("x", "remove-char", QUIET);
	ft_rl_bind("<RET>", "accept-line", QUIET);
	ft_rl_bind("<TAB>", "complete", QUIET);
	ft_rl_bind("<ESC>", "abort", QUIET);
}

static inline void	_hlcolor_default_binds(void)
{
	ft_rl_seteditmode(_MD_HLCOLOR);
	ft_rl_bind("b", "hlcolor-toggle-bold", QUIET);
	ft_rl_bind("u", "hlcolor-toggle-underline", QUIET);
	ft_rl_bind("f", "hlcolor-toggle-fg/bg", QUIET);
	ft_rl_bind("s", "hlcolor-set-sgr", QUIET);
	ft_rl_bind("c", "hlcolor-set-color", QUIET);
	ft_rl_bind("r", "hlcolor-set-rgb", QUIET);
	ft_rl_bind("<RET>", "hlcolor-accept", QUIET);
	ft_rl_bind("<BCK>", "backward-remove-char", QUIET);
	ft_rl_bind("0", "self-insert", QUIET);
	ft_rl_bind("1", "self-insert", QUIET);
	ft_rl_bind("2", "self-insert", QUIET);
	ft_rl_bind("3", "self-insert", QUIET);
	ft_rl_bind("4", "self-insert", QUIET);
	ft_rl_bind("5", "self-insert", QUIET);
	ft_rl_bind("6", "self-insert", QUIET);
	ft_rl_bind("7", "self-insert", QUIET);
	ft_rl_bind("8", "self-insert", QUIET);
	ft_rl_bind("9", "self-insert", QUIET);
}

static inline void	_ssi_binds(void)
{
	char	seq[2] = "!";
	uint8_t	c;

	ft_rl_seteditmode(_MD_SSI);
	ft_rl_const_bind("<SPC>", "self-insert");
	ft_rl_const_bind("\\<", "self-insert");
	ft_rl_const_bind("\\\\", "self-insert");
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		ft_rl_const_bind(seq, "self-insert");
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
	ft_rl_set("keyseq-timeout", 500);
}

static inline void	_init_escapes(void)
{
	ft_ti_loadinfo(getenv("TERM"));
	g_escapes.cup = ft_ti_getstr("cup");
	g_escapes.bel = ft_ti_getstr("bel");
	g_escapes.flash = ft_ti_getstr("flash");
	g_escapes.civis = ft_ti_getstr("civis");
	g_escapes.cnorm = ft_ti_getstr("cnorm");
	g_escapes.smkx = ft_ti_getstr("smkx");
	g_escapes.rmkx = ft_ti_getstr("rmkx");
	g_escapes.el1 = ft_ti_getstr("el1");
	g_escapes.el = ft_ti_getstr("el");
	g_escapes.ed1 = TERM_CLEAR_START;
	g_escapes.ed = ft_ti_getstr("ed");
	g_escapes.indn = ft_ti_getstr("indn");
	g_escapes.rin = ft_ti_getstr("rin");
	g_escapes.sgr0 = ft_ti_getstr("sgr0");
	g_escapes.bold = ft_ti_getstr("bold");
	g_escapes.smul = ft_ti_getstr("smul");
	g_escapes.rmul = ft_ti_getstr("rmul");
	g_escapes.setaf = ft_ti_getstr("setaf");
	g_escapes.setab = ft_ti_getstr("setab");
	g_escapes.kf1 = ft_ti_getstr("kf1");
	g_escapes.kf2 = ft_ti_getstr("kf2");
	g_escapes.kf3 = ft_ti_getstr("kf3");
	g_escapes.kf4 = ft_ti_getstr("kf4");
	g_escapes.kf5 = ft_ti_getstr("kf5");
	g_escapes.kf6 = ft_ti_getstr("kf6");
	g_escapes.kf7 = ft_ti_getstr("kf7");
	g_escapes.kf8 = ft_ti_getstr("kf8");
	g_escapes.kf9 = ft_ti_getstr("kf9");
	g_escapes.kf10 = ft_ti_getstr("kf10");
	g_escapes.kf11 = ft_ti_getstr("kf11");
	g_escapes.kf12 = ft_ti_getstr("kf12");
	g_escapes.kcuu1 = ft_ti_getstr("kcuu1");
	g_escapes.kcud1 = ft_ti_getstr("kcud1");
	g_escapes.kcub1 = ft_ti_getstr("kcub1");
	g_escapes.kcuf1 = ft_ti_getstr("kcuf1");
	g_escapes.kLFT = ft_ti_getstr("kLFT");
	g_escapes.kRIT = ft_ti_getstr("kRIT");
	g_escapes.ht = ft_ti_getstr("ht");
	g_escapes.kent = ft_ti_getstr("kent");
	g_escapes.cr = ft_ti_getstr("cr");
	g_escapes.kich1 = ft_ti_getstr("kich1");
	g_escapes.khome = ft_ti_getstr("khome");
	g_escapes.knp = ft_ti_getstr("knp");
	g_escapes.kdch1 = ft_ti_getstr("kdch1");
	g_escapes.kend = ft_ti_getstr("kend");
	g_escapes.kpp = ft_ti_getstr("kpp");
	g_escapes.kHOM = ft_ti_getstr("kHOM");
	g_escapes.kDC = ft_ti_getstr("kDC");
	g_escapes.kEND = ft_ti_getstr("kEND");
	g_escapes.bel = (!g_escapes.bel) ? TERM_RING_BELL : g_escapes.bel;
	g_escapes.flash = (!g_escapes.flash) ? TERM_FLASH_SCREEN : g_escapes.flash;
	g_escapes.civis = (!g_escapes.civis) ? TERM_CUR_HIDE : g_escapes.civis;
	g_escapes.cnorm = (!g_escapes.cnorm) ? TERM_CUR_SHOW : g_escapes.cnorm;
	g_escapes.el1 = (!g_escapes.el1) ? TERM_CLEAR_LINE_START : g_escapes.el1;
	g_escapes.el = (!g_escapes.el) ? TERM_CLEAR_LINE_END : g_escapes.el;
	g_escapes.ed = (!g_escapes.ed) ? TERM_CLEAR_END : g_escapes.ed;
	g_escapes.indn = (!g_escapes.indn) ? TERM_SCROLL_UP_N : g_escapes.indn;
	g_escapes.rin = (!g_escapes.rin) ? TERM_SCROLL_DOWN_N : g_escapes.rin;
	g_escapes.sgr0 = (!g_escapes.sgr0) ? SGR_RESET : g_escapes.sgr0;
	g_escapes.bold = (!g_escapes.bold) ? SGR_BOLDON : g_escapes.bold;
	g_escapes.smul = (!g_escapes.smul) ? SGR_ULINEON : g_escapes.smul;
	g_escapes.rmul = (!g_escapes.rmul) ? SGR_ULINEOFF : g_escapes.rmul;
}

static inline void	_ft_rl_exit(void)
{
	if (g_hist)
		ft_rl_hist_save(_FT_RL_HFILE);
	tcsetattr(0, TCSANOW, &g_oldsettings);
	ft_ti_tputs(TERM_BPM_OFF, 1, ft_rl_putc);
	ft_ti_tputs(g_escapes.rmkx, 1, ft_rl_putc);
	ft_ti_tputs(g_escapes.cnorm, 1, ft_rl_putc);
	__return(42);
}
