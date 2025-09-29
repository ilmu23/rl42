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
#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"
#include "internal/_keybinds.h"

#define _AMBIGUOUS_TIMEOUT 0

typedef struct {
	rl42_key_tree	*fn;
	u8				run;
}	__fn;

#define __fn(f, r)	((__fn){.fn = f, .run = r})

extern u32	kcbs;

rl42_state	state_flags;

rl42_hist_node	*current;

static inline __fn	_match_seq(rl42_line *line, rl42_key_tree *current, const rl42_kb_event *event);
static inline void	_commit_hist(const char *line);

char	*ft_readline(const char *prompt) {
	rl42_line	line;
	__fn		match;
	char		*out;
	u8			rv;

	if (!rl42_init()) {
		error("rl42: unable to initialize: %s", (errno) ? strerror(errno) : "unknown error");
		return NULL;
	}
	line = (rl42_line){
		.prompt.prompt = cstr_to_rl42str(prompt),
		.keyseq = vector(u32, 8, NULL),
	};
	if (!line.prompt.prompt || !line.keyseq)
		goto _rl42_malloc_fail;
	match.fn = NULL;
	term_apply_settings(TERM_SETTINGS_RL42);
	term_cursor_get_pos(&line.prompt.root.row, &line.prompt.root.col);
	term_display_line(&line, DISPLAY_PROMPT_ONLY);
	term_cursor_get_pos(&line.root.row, &line.root.col);
	hist_add_line(strdup(""));
	if (!current) {
		current = hist_get_first_node();
		line.line = vector(u32, 64, NULL);
	} else
		line.line = cstr_to_rl42str(current->line);
	if (!line.line)
		goto _rl42_malloc_fail;
	line.i = vector_size(line.line);
	term_display_line(&line, 0);
	rv = 1;
	do {
		match = _match_seq(&line, match.fn, kb_listen((match.fn && match.fn->f) ? _AMBIGUOUS_TIMEOUT : -1));
		if (match.fn && match.run) {
			rv = match.fn->f(&line);
			vector_clear(line.keyseq);
			match.fn = NULL;
		}
	} while (rv);
	term_apply_settings(TERM_SETTINGS_DEFAULT);
	out = (line.line) ? rl42str_to_cstr(line.line) : NULL;
	vector_delete(line.prompt.prompt);
	vector_delete(line.keyseq);
	vector_delete(line.line);
	_commit_hist(out);
	return out;
_rl42_malloc_fail:
	error("rl42: unable to allocate memory: %s", (errno) ? strerror(errno) : "unknown error");
	return NULL;
}

static inline __fn	_match_seq(rl42_line *line, rl42_key_tree *current, const rl42_kb_event *event) {
	rl42_key_tree	**tmp;
	rl42_key_tree	*binds;
	size_t			len;
	size_t			i;
	u32				ucp;

	if (!event)
		return __fn(current, 1);
	ucp = event->code;
	// TODO: proper modifier parsing
	if (event->mods & KB_MOD_SHIFT)
		ucp &= ~0x20;
	if (event->mods & KB_MOD_CTRL && event->code != kcbs)
		ucp &= ~0x60;
	if (event->mods & KB_MOD_ALT)
		vector_push(line->keyseq, (u32){'\x1b'});
	vector_push(line->keyseq, ucp);
	for (i = 0, tmp = NULL, len = vector_size(line->keyseq), binds = get_key_tree(CURRENT); i < len && binds->next; i++) {
		tmp = map_get(binds->next, *(u32 *)vector_get(line->keyseq, i));
		if (tmp == MAP_NOT_FOUND)
			break ;
		binds = *tmp;
	}
	if (i < len || tmp == MAP_NOT_FOUND) {
		vector_clear(line->keyseq);
		return __fn(NULL, 0);
	}
	return __fn(binds, (binds->f && (!binds->next || map_empty(binds->next))) ? 1 : 0);
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
