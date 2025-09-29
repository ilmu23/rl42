// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<init.c>>

#include <stdlib.h>

#ifdef __TEST_BUILD
#include <unistd.h>
#endif

#define __RL42_INTERNAL
#include "rl42.h"
#include "function.h"

#include "internal/_kb.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_history.h"
#include "internal/_function.h"
#include "internal/_keybinds.h"
#include "internal/_terminfo.h"

#include "internal/fn/move.h"
#include "internal/fn/text.h"
#include "internal/fn/history.h"

#if !defined __DEBUG_BUILD || defined __DEBUG_BIND_WARNINGS
# define _BIND_MODE WARN
#else
# define _BIND_MODE QUIET
#endif

#define bind_emacs(seq, f)	(rl42_bind(seq, f, _BIND_MODE, EMACS))
#define bind_vi_cmd(seq, f)	(rl42_bind(seq, f, _BIND_MODE, VI_CMD))
#define bind_vi_ins(seq, f)	(rl42_bind(seq, f, _BIND_MODE, VI_INS))
#define bind_insert(seq, f)	(bind_emacs(seq, f), bind_vi_ins(seq, f))
#define bind_all(seq, f)	(bind_emacs(seq, f), bind_vi_cmd(seq, f), bind_vi_ins(seq, f))

static inline void	_init_binds(void);
static inline void	_rl42_exit(void);
static inline u8	_init_fns(void);

static u8	init = 0;

u8	rl42_init(void) {
	u8			rv;

	rv = 1;
	if (!init && ~state_flags & STATE_INIT_IN_PROGRESS) {
		state_flags ^= STATE_INIT_IN_PROGRESS;
		if (!init_key_trees())
			rv = 0;
		// MAYBE: init macro data
		if (atexit(_rl42_exit) != 0)
			rv = 0;
#ifdef __TEST_BUILD
		if (isatty(0) && !term_init())
#else
		if (!term_init())
#endif
			rv = 0;
		if (!_init_fns())
			rv = 0;
		_init_binds();
		hist_load(getenv("RL42_HISTORY"));
		// TODO: read initfile
		// MAYBE: init highlight color
		state_flags ^= STATE_INIT_IN_PROGRESS;
		init = rv;
	}
	return rv;
}

static inline void	_init_binds(void) {
	char	seq[2] = "!";
	i8		c;

	bind_all("<C-l>", "clear-screen");
	bind_all("<M-C-l>", "clear-display");
	bind_all("<RET>", "accept-line");
	bind_all("<C-c>", "discard-line");
	bind_all("<C-d>", "end-of-file");
	bind_all("<DOWN>", "forward-history");
	bind_all("<UP>", "backward-history");
	bind_all("<C-g>", "abort");
	bind_all("<C-v>", "dump-variables");
	bind_insert("<SPC>", "self-insert");
	bind_insert("\\<", "self-insert");
	bind_insert("\\\\", "self-insert");
	for (c = *seq; c++ <= '~'; *seq = (*seq == ';' || *seq == '[') ? ++c : c)
		bind_insert(seq, "self-insert");
	bind_insert("<DEL>", "delete-char");
	bind_insert("<BCK>", "backward-delete-char");
	bind_insert("<END>", "end-of-line");
	bind_insert("<HME>", "beginning-of-line");
	bind_insert("<RIGHT>", "forward-char");
	bind_insert("<LEFT>", "backward-char");
	bind_insert("<TAB>", "complete");
	bind_emacs("<C-a>", "beginning-of-line");
	bind_emacs("<C-e>", "end-of-line");
	bind_emacs("<C-f>", "forward-char");
	bind_emacs("<C-b>", "backward-char");
	bind_emacs("<S-RIGHT>", "end-of-word");
	bind_emacs("<S-LEFT>", "start-of-word");
	bind_emacs("<M-f>", "forward-word");
	bind_emacs("<M-b>", "backward-word");
	bind_emacs("<M-]>", "forward-find-character");
	bind_emacs("<M-[>", "backward-find-character");
	bind_emacs("<C-o>", "operate-and-get-next");
	bind_emacs("<C-n>", "forward-history");
	bind_emacs("<C-p>", "backward-history");
	bind_emacs("<M-\\>>", "end-of-history");
	bind_emacs("<M-<>", "beginning-of-history");
	bind_emacs("<M-h>", "forward-search-history");
	bind_emacs("<M-r>", "backward-search-history");
	bind_emacs("<C-h>", "inc-forward-search-history");
	bind_emacs("<C-r>", "inc-backward-search-history");
	bind_emacs("<M-C-y>", "yank-nth-arg");
	bind_emacs("<M-.>", "yank-last-arg");
	bind_emacs("<M-u>", "upcase-word");
	bind_emacs("<M-l>", "downcase-word");
	bind_emacs("<M-c>", "capitalize-word");
	bind_emacs("<C-k>", "forward-kill-line");
	bind_emacs("<M-k>", "backward-kill-line");
	bind_emacs("<M-K>", "kill-whole-line");
	bind_emacs("<C-k>w", "forward-kill-word");
	bind_emacs("<M-k>w", "backward-kill-word");
	bind_emacs("<C-k>r", "kill-region");
	bind_emacs("<M-\\\\>", "delete-horizontal-space");
	bind_emacs("<M-C>", "copy-region-as-kill");
	bind_emacs("<M-F>", "copy-forward-word");
	bind_emacs("<M-B>", "copy-backward-word");
	bind_emacs("<C-y>", "yank");
	bind_emacs("<M-y>", "yank-pop");
	bind_emacs("<C-t>", "transpose-characters");
	bind_emacs("<M-t>", "transpose-words");
	bind_emacs("<C-x><C-r>", "re-read-init-file");
	bind_emacs("<M-0>", "digit-argument");
	bind_emacs("<M-1>", "digit-argument");
	bind_emacs("<M-2>", "digit-argument");
	bind_emacs("<M-3>", "digit-argument");
	bind_emacs("<M-4>", "digit-argument");
	bind_emacs("<M-5>", "digit-argument");
	bind_emacs("<M-6>", "digit-argument");
	bind_emacs("<M-7>", "digit-argument");
	bind_emacs("<M-8>", "digit-argument");
	bind_emacs("<M-9>", "digit-argument");
	bind_emacs("<M-->", "negative-digit-argument");
	bind_emacs("<M-x>", "set-mark");
	bind_emacs("<M-X>", "unset-mark");
	bind_emacs("<M-C-x>", "exchange-point-and-mark");
	bind_emacs("<M-C-e>", "vi-editing-mode");
	bind_vi_cmd("<C-e>", "emacs-editing-mode");
	bind_vi_cmd("<C-t>", "tranpose-characters");
	bind_vi_cmd("<SPC>", "forward-char");
	bind_vi_cmd("<M-r>", "re-read-init-file");
	bind_vi_cmd("$", "end-of-line");
	bind_vi_cmd("0", "beginning-of-line");
	bind_vi_cmd("1", "digit-argument");
	bind_vi_cmd("2", "digit-argument");
	bind_vi_cmd("3", "digit-argument");
	bind_vi_cmd("4", "digit-argument");
	bind_vi_cmd("5", "digit-argument");
	bind_vi_cmd("6", "digit-argument");
	bind_vi_cmd("7", "digit-argument");
	bind_vi_cmd("8", "digit-argument");
	bind_vi_cmd("9", "digit-argument");
	bind_vi_cmd(",", "exchange-point-and-mark");
	bind_vi_cmd("/", "inc-forward-search-history");
	bind_vi_cmd("?", "inc-reverse-search-history");
	bind_vi_cmd("a", "vi-append");
	bind_vi_cmd("A", "vi-append-eol");
	bind_vi_cmd("b", "backward-word");
	bind_vi_cmd("d", "vi-delete");
	bind_vi_cmd("f", "forward-find-character");
	bind_vi_cmd("F", "backward-find-character");
	bind_vi_cmd("h", "backward-char");
	bind_vi_cmd("i", "vi-insert");
	bind_vi_cmd("I", "vi-insert-sol");
	bind_vi_cmd("j", "forward-history");
	bind_vi_cmd("k", "backward-history");
	bind_vi_cmd("l", "forward-char");
	bind_vi_cmd("m", "set-mark");
	bind_vi_cmd("M", "unset-mark");
	bind_vi_cmd("p", "yank");
	bind_vi_cmd("P", "yank-pop");
	bind_vi_cmd("r", "vi-replace");
	bind_vi_cmd("s", "vi-substitute");
	bind_vi_cmd("S", "vi-substitute-line");
	bind_vi_cmd("w", "forward-word");
	bind_vi_cmd("W", "backward-word");
	bind_vi_cmd("x", "delete-char");
	bind_vi_cmd("X", "backward-delete-char");
	bind_vi_cmd("Z", "discard-line");
	bind_vi_cmd("<ESC>", "abort");
	bind_vi_ins("<C-r>", "inc-reverse-search-history");
	bind_vi_ins("<C-s>", "inc-forward-search-history");
	bind_vi_ins("<C-y>", "yank-last-arg");
	bind_vi_ins("<ESC>", "vi-command-mode");
	// MAYBE: highlight color editing
}

static inline void	_rl42_exit(void) {
	if (init) {
		term_apply_settings(TERM_SETTINGS_DEFAULT);
		clean_kb_listener();
		clean_key_trees();
		hist_clean();
		clean_fns();
		ti_unload();
	}
}

#define __rl42_fn(f, n)	{.address = f, .name = n}

static const struct {
	const rl42_fn	address;
	const char		*name;
}	functions[] = {
	__rl42_fn(accept_line, "accept-line"),
	__rl42_fn(backward_char, "backward-char"),
	__rl42_fn(backward_delete_char, "backward-delete-char"),
	__rl42_fn(backward_history, "backward-history"),
	__rl42_fn(backward_word, "backward-word"),
	__rl42_fn(beginning_of_history, "beginning-of-history"),
	__rl42_fn(beginning_of_line, "beginning-of-line"),
	__rl42_fn(capitalize_word, "capitalize-word"),
	__rl42_fn(clear_display, "clear-display"),
	__rl42_fn(clear_screen, "clear-screen"),
	__rl42_fn(delete_char, "delete-char"),
	__rl42_fn(downcase_word, "downcase-word"),
	__rl42_fn(end_of_file, "end-of-file"),
	__rl42_fn(end_of_history, "end-of-history"),
	__rl42_fn(end_of_line, "end-of-line"),
	__rl42_fn(forward_char, "forward-char"),
	__rl42_fn(forward_history, "forward-history"),
	__rl42_fn(forward_word, "forward-word"),
	__rl42_fn(operate_and_get_next, "operate-and-get-next"),
	__rl42_fn(self_insert, "self-insert"),
	__rl42_fn(upcase_word, "upcase-word"),
};

static inline u8	_init_fns(void) {
	size_t	fn_count;
	size_t	i;

	for (i = 0, fn_count = sizeof(functions) / sizeof(*functions); i < fn_count; i++)
		if (!rl42_register_function(functions[i].address, functions[i].name))
			break ;
	return (i == fn_count) ? 1 : 0;
}
