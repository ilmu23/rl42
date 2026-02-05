// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42.c>>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_kb.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_darray.h"
#include "internal/_display.h"
#include "internal/_history.h"
#include "internal/_keybinds.h"

#ifndef __RL42_USE_EXTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

#include "internal/fn/misc.h"

char	*(*rl42_getenv)(const char *) = getenv;

rl42_hist_node	*current;

rl42_state	state_flags;

rl42_mark	user;

rl42_fn	prev_fn;

static inline void			_commit_hist(const char *line);

char	*ft_readline(const char *prompt) {
	rl42_fn_match	match;
	rl42_line		line;
	char			*out;
	u8				rv;

	if (!rl42_init()) {
		error("rl42: unable to initialize: %s", (errno) ? strerror(errno) : "unknown error");
		return NULL;
	}
	if (rl42_get(RL42_ENABLE_BRACKETED_PASTE).u64 == rl42_conf_on && ! term_set_bpm(BPM_ENABLED)) {
		error("rl42: unable to enable bracketed paste mode: %s", (errno) ? strerror(errno) : "unknown error");
		return NULL;
	}
	line = (rl42_line){
		.prompt.prompt = cstr_to_rl42str(prompt),
		.keyseq = darray(u32, 8, NULL),
		.rows = 1,
	};
	if (!line.prompt.prompt || !line.keyseq)
		goto _rl42_malloc_fail;
	user.set = 0;
	prev_fn = NULL;
	match.fn = NULL;
	term_apply_settings(TERM_SETTINGS_RL42);
	line.prompt.root = term_cursor_new_anchor();
	if (!line.prompt.root)
		goto _rl42_malloc_fail;
	term_display_line(&line, DISPLAY_PROMPT_ONLY);
	line.root = term_cursor_new_anchor();
	if (!line.root)
		goto _rl42_malloc_fail;
	hist_add_line(strdup(""));
	if (!current) {
		current = hist_get_first_node();
		line.line = darray(u32, 64, NULL);
	} else
		line.line = cstr_to_rl42str(current->line);
	if (!line.line)
		goto _rl42_malloc_fail;
	line.i = darray_size(line.line);
	term_display_line(&line, 0);
	rv = 1;
	if (get_editing_mode() == VI_CMD)
		set_editing_mode(VI_INS);
	do {
		match = kb_match_seq(&line, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
		if (match.fn && match.run) {
			rv = match.fn->f(&line);
			state_flags &= ~STATE_ABORT;
			if (~state_flags & STATE_DONT_CLEAR_KEYSEQ)
				darray_clear(line.keyseq);
			else
				state_flags &= ~STATE_DONT_CLEAR_KEYSEQ;
			if (n_arg.set && match.fn->f != numeric_argument) {
				darray_delete(line.prompt.sprompt);
				line.prompt.sprompt = NULL;
				if (line.line)
					term_display_line(&line, 0);
				n_arg.set = 0;
			}
			if (!term_flush_outbuf())
				error("rl42: unable to flush output buffer: %m");
			prev_fn = match.fn->f;
			match.fn = NULL;
		}
	} while (rv);
	ti42_tputs("\n", 1, term_putchar_unbuffered);
	term_apply_settings(TERM_SETTINGS_DEFAULT);
	if (rl42_get(RL42_ENABLE_BRACKETED_PASTE).u64 == rl42_conf_on && ! term_set_bpm(BPM_ENABLED))
		error("rl42: unable to disable bracketed paste mode: %s", (errno) ? strerror(errno) : "unknown error");
	out = (line.line) ? rl42str_to_cstr(line.line) : NULL;
	term_cursor_delete_anchor(line.prompt.root);
	term_cursor_delete_anchor(line.root);
	darray_delete(line.prompt.sprompt);
	darray_delete(line.prompt.prompt);
	darray_delete(line.keyseq);
	darray_delete(line.line);
	_commit_hist(out);
	return out;
_rl42_malloc_fail:
	error("rl42: unable to allocate memory: %s", (errno) ? strerror(errno) : "unknown error");
	term_cursor_delete_anchor(line.prompt.root);
	term_cursor_delete_anchor(line.root);
	darray_delete(line.prompt.prompt);
	darray_delete(line.keyseq);
	return NULL;
}

static inline void	_commit_hist(const char *line) {
	rl42_hist_node	*node;
	rl42_hist_node	*prev;

	node = hist_get_first_node();
	if (line && *line) {
		free((void *)node->line);
		node->line = strdup(line);
	} else
		hist_remove_node(node);
	for (node = hist_get_first_node(), prev = NULL; node != prev; prev = node, node = hist_get_next_node(node, BACKWARD)) {
		if (node->edit) {
			free((void *)node->edit);
			node->edit = NULL;
		}
	}
	if (~state_flags & STATE_SAVE_HIST_POSITION)
		current = NULL;
	state_flags &= ~STATE_SAVE_HIST_POSITION;
}
