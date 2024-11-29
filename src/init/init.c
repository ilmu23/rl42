/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:50:39 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 16:56:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_fnames.h"
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
	g_macrodata[0] = __mapnew();
	g_macrodata[1] = __mapnew();
	g_macrodata[2] = __mapnew();
	if (!g_funcs || !g_binds || !g_macrodata[0] || !g_macrodata[1] || !g_macrodata[2])
		__exit(ft_rl_perror());
	tcgetattr(0, &g_oldsettings);
	g_newsettings = g_oldsettings;
	g_newsettings.c_iflag &= ~(ICRNL | IXON);
	g_newsettings.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tcsetattr(0, TCSANOW, &g_newsettings);
	if (!newbindmap("emacs") || !newbindmap("vi-ins") || !newbindmap("vi-cmd")
		|| !newbindmap("hlcolor") || !newbindmap("__ssi")  || atexit(_ft_rl_exit))
		__exit(ft_rl_perror());
	g_macro = (rl_fninfo_t){.f = ft_rl_mcr, .name = "__macro", .binds[0] = NULL, .binds[1] = NULL, .binds[2] = NULL};
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
	ft_rl_bind("<SPC>", self_insert, QUIET);
	ft_rl_bind("\\<", self_insert, QUIET);
	ft_rl_bind("\\\\", self_insert, QUIET);
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		ft_rl_bind(seq, self_insert, QUIET);
	ft_rl_bind("<DEL>", remove_char, QUIET);
	ft_rl_bind("<BCK>", backward_remove_char, QUIET);
	ft_rl_bind("<C-a>", beginning_of_line, QUIET);
	ft_rl_bind("<HME>", beginning_of_line, QUIET);
	ft_rl_bind("<C-e>", end_of_line, QUIET);
	ft_rl_bind("<END>", end_of_line, QUIET);
	ft_rl_bind("<RIGHT>", forward_char, QUIET);
	ft_rl_bind("<C-f>", forward_char, QUIET);
	ft_rl_bind("<LEFT>", backward_char, QUIET);
	ft_rl_bind("<C-b>", backward_char, QUIET);
	ft_rl_bind("<S-RIGHT>", end_of_word, QUIET);
	ft_rl_bind("<S-LEFT>", beginning_of_word, QUIET);
	ft_rl_bind("<M-f>", forward_word, QUIET);
	ft_rl_bind("<M-b>", backward_word, QUIET);
	ft_rl_bind("<C-l>", clear_screen, QUIET);
	ft_rl_bind("<M-C-l>", clear_display, QUIET);
	ft_rl_bind("<M-,>", forward_find_character, QUIET);
	ft_rl_bind("<M-;>", backward_find_character, QUIET);
	ft_rl_bind("<RET>", accept_line, QUIET);
	ft_rl_bind("<C-o>", operate_and_get_next, QUIET);
	ft_rl_bind("<C-d>", end_of_file, QUIET);
	ft_rl_bind("<DOWN>", next_history, QUIET);
	ft_rl_bind("<C-n>", next_history, QUIET);
	ft_rl_bind("<UP>", previous_history, QUIET);
	ft_rl_bind("<C-p>", previous_history, QUIET);
	ft_rl_bind("<M-\\>>", end_of_history, QUIET);
	ft_rl_bind("<M-<>", beginning_of_history, QUIET);
	ft_rl_bind("<M-s>", forward_search_history, QUIET);
	ft_rl_bind("<M-r>", reverse_search_history, QUIET);
	ft_rl_bind("<C-h>", inc_forward_search_history, QUIET);
	ft_rl_bind("<C-r>", inc_reverse_search_history, QUIET);
	ft_rl_bind("<M-C-y>", yank_nth_arg, QUIET);
	ft_rl_bind("<M-.>", yank_last_arg, QUIET);
	ft_rl_bind("<M-u>", upcase_word, QUIET);
	ft_rl_bind("<M-l>", downcase_word, QUIET);
	ft_rl_bind("<M-c>", capitalize_word, QUIET);
	ft_rl_bind("<C-k>", forward_kill_line, QUIET);
	ft_rl_bind("<M-k>", backward_kill_line, QUIET);
	ft_rl_bind("<M-K>", kill_whole_line, QUIET);
	ft_rl_bind("<C-k>w", forward_kill_word, QUIET);
	ft_rl_bind("<M-k>w", backward_kill_word, QUIET);
	ft_rl_bind("<C-k>r", kill_region, QUIET);
	ft_rl_bind("<M-\\\\>", delete_horizontal_space, QUIET);
	ft_rl_bind("<M-C>", copy_region_as_kill, QUIET);
	ft_rl_bind("<M-F>", copy_forward_word, QUIET);
	ft_rl_bind("<M-B>", copy_backward_word, QUIET);
	ft_rl_bind("<C-y>", yank, QUIET);
	ft_rl_bind("<M-y>", yank_pop, QUIET);
	ft_rl_bind("<C-t>", transpose_characters, QUIET);
	ft_rl_bind("<M-t>", transpose_words, QUIET);
	ft_rl_bind("<TAB>", complete, QUIET);
	ft_rl_bind("<C-x><C-r>", re_read_init_file, QUIET);
	ft_rl_bind("<C-c>", discard_line, QUIET);
	ft_rl_bind("<M-0>", digit_argument, QUIET);
	ft_rl_bind("<M-1>", digit_argument, QUIET);
	ft_rl_bind("<M-2>", digit_argument, QUIET);
	ft_rl_bind("<M-3>", digit_argument, QUIET);
	ft_rl_bind("<M-4>", digit_argument, QUIET);
	ft_rl_bind("<M-5>", digit_argument, QUIET);
	ft_rl_bind("<M-6>", digit_argument, QUIET);
	ft_rl_bind("<M-7>", digit_argument, QUIET);
	ft_rl_bind("<M-8>", digit_argument, QUIET);
	ft_rl_bind("<M-9>", digit_argument, QUIET);
	ft_rl_bind("<M-->", negative_digit_argument, QUIET);
	ft_rl_bind("<C-g>", rl_abort, QUIET);
	ft_rl_bind("<M-x>", set_mark, QUIET);
	ft_rl_bind("<M-X>", unset_mark, QUIET);
	ft_rl_bind("<M-C-x>", exchange_point_and_mark, QUIET);
	ft_rl_bind("<M-C-e>", vi_editing_mode, QUIET);
	ft_rl_bind("<M-h>", set_highlight_color, QUIET);
}

static inline void	_vi_ins_default_binds(void)
{
	char	seq[2] = "!";
	uint8_t	c;

	ft_rl_seteditmode(_MD_VI_INS);
	ft_rl_bind("<SPC>", self_insert, QUIET);
	ft_rl_bind("\\<", self_insert, QUIET);
	ft_rl_bind("\\\\", self_insert, QUIET);
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		ft_rl_bind(seq, self_insert, QUIET);
	ft_rl_bind("<C-c>", discard_line, QUIET);
	ft_rl_bind("<C-d>", end_of_file, QUIET);
	ft_rl_bind("<C-g>", rl_abort, QUIET);
	ft_rl_bind("<C-l>", clear_screen, QUIET);
	ft_rl_bind("<M-C-l>", clear_display, QUIET);
	ft_rl_bind("<C-r>", inc_reverse_search_history, QUIET);
	ft_rl_bind("<C-s>", inc_forward_search_history, QUIET);
	ft_rl_bind("<C-t>", transpose_characters, QUIET);
	ft_rl_bind("<C-y>", yank_last_arg, QUIET);
	ft_rl_bind("<UP>", previous_history, QUIET);
	ft_rl_bind("<DOWN>", next_history, QUIET);
	ft_rl_bind("<DEL>", remove_char, QUIET);
	ft_rl_bind("<BCK>", backward_remove_char, QUIET);
	ft_rl_bind("<ESC>", vi_command_mode, QUIET);
	ft_rl_bind("<RET>", accept_line, QUIET);
	ft_rl_bind("<TAB>", complete, QUIET);
}

static inline void	_vi_cmd_default_binds(void)
{
	ft_rl_seteditmode(_MD_VI_CMD);
	ft_rl_bind("<C-c>", discard_line, QUIET);
	ft_rl_bind("<C-d>", end_of_file, QUIET);
	ft_rl_bind("<C-e>", emacs_editing_mode, QUIET);
	ft_rl_bind("<C-f>", dump_functions, QUIET);
	ft_rl_bind("<C-g>", rl_abort, QUIET);
	ft_rl_bind("<C-l>", clear_screen, QUIET);
	ft_rl_bind("<M-C-l>", clear_display, QUIET);
	ft_rl_bind("<C-r>", inc_reverse_search_history, QUIET);
	ft_rl_bind("<C-s>", inc_forward_search_history, QUIET);
	ft_rl_bind("<C-t>", transpose_characters, QUIET);
	ft_rl_bind("<C-v>", dump_variables, QUIET);
	ft_rl_bind("<SPC>", forward_char, QUIET);
	ft_rl_bind("<M-r>", re_read_init_file, QUIET);
	ft_rl_bind("$", end_of_line, QUIET);
	ft_rl_bind(",", exchange_point_and_mark, QUIET);
	ft_rl_bind("/", inc_forward_search_history, QUIET);
	ft_rl_bind("0", beginning_of_line, QUIET);
	ft_rl_bind("1", digit_argument, QUIET);
	ft_rl_bind("2", digit_argument, QUIET);
	ft_rl_bind("3", digit_argument, QUIET);
	ft_rl_bind("4", digit_argument, QUIET);
	ft_rl_bind("5", digit_argument, QUIET);
	ft_rl_bind("6", digit_argument, QUIET);
	ft_rl_bind("7", digit_argument, QUIET);
	ft_rl_bind("8", digit_argument, QUIET);
	ft_rl_bind("9", digit_argument, QUIET);
	ft_rl_bind("?", inc_reverse_search_history, QUIET);
	ft_rl_bind("A", vi_insert_mode_A, QUIET);
	ft_rl_bind("F", backward_find_character, QUIET);
	ft_rl_bind("I", vi_insert_mode_I, QUIET);
	ft_rl_bind("M", unset_mark, QUIET);
	ft_rl_bind("P", yank_pop, QUIET);
	ft_rl_bind("S", vi_subst_line, QUIET);
	ft_rl_bind("X", backward_remove_char, QUIET);
	ft_rl_bind("Z", discard_line, QUIET);
	ft_rl_bind("_", yank_last_arg, QUIET);
	ft_rl_bind("a", vi_insert_mode_a, QUIET);
	ft_rl_bind("b", backward_word, QUIET);
	ft_rl_bind("c", set_highlight_color, QUIET);
	ft_rl_bind("d", vi_delete, QUIET);
	ft_rl_bind("f", forward_find_character, QUIET);
	ft_rl_bind("h", backward_char, QUIET);
	ft_rl_bind("i", vi_insert_mode_i, QUIET);
	ft_rl_bind("j", next_history, QUIET);
	ft_rl_bind("k", previous_history, QUIET);
	ft_rl_bind("l", forward_char, QUIET);
	ft_rl_bind("m", set_mark, QUIET);
	ft_rl_bind("p", yank, QUIET);
	ft_rl_bind("r", vi_replace, QUIET);
	ft_rl_bind("s", vi_subst, QUIET);
	ft_rl_bind("w", forward_word, QUIET);
	ft_rl_bind("x", remove_char, QUIET);
	ft_rl_bind("<RET>", accept_line, QUIET);
	ft_rl_bind("<TAB>", complete, QUIET);
	ft_rl_bind("<ESC>", rl_abort, QUIET);
}

static inline void	_hlcolor_default_binds(void)
{
	ft_rl_seteditmode(_MD_HLCOLOR);
	ft_rl_bind("b", hlcolor_toggle_bold, QUIET);
	ft_rl_bind("u", hlcolor_toggle_underline, QUIET);
	ft_rl_bind("f", hlcolor_toggle_fg_bg, QUIET);
	ft_rl_bind("s", hlcolor_set_sgr, QUIET);
	ft_rl_bind("c", hlcolor_set_color, QUIET);
	ft_rl_bind("r", hlcolor_set_rgb, QUIET);
	ft_rl_bind("<RET>", hlcolor_accept, QUIET);
	ft_rl_bind("<BCK>", backward_remove_char, QUIET);
	ft_rl_bind("0", self_insert, QUIET);
	ft_rl_bind("1", self_insert, QUIET);
	ft_rl_bind("2", self_insert, QUIET);
	ft_rl_bind("3", self_insert, QUIET);
	ft_rl_bind("4", self_insert, QUIET);
	ft_rl_bind("5", self_insert, QUIET);
	ft_rl_bind("6", self_insert, QUIET);
	ft_rl_bind("7", self_insert, QUIET);
	ft_rl_bind("8", self_insert, QUIET);
	ft_rl_bind("9", self_insert, QUIET);
}

static inline void	_ssi_binds(void)
{
	char	seq[2] = "!";
	uint8_t	c;

	ft_rl_seteditmode(_MD_SSI);
	ft_rl_const_bind("<SPC>", self_insert);
	ft_rl_const_bind("\\<", self_insert);
	ft_rl_const_bind("\\\\", self_insert);
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		ft_rl_const_bind(seq, self_insert);
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
