// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42.c>>

#include "internal/_kb.h"
#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_keybinds.h"

#define _AMBIGUOUS_TIMEOUT 0

typedef struct {
	rl42_key_tree	*fn;
	u8				run;
}	__fn;

#define __fn(f, r)	((__fn){.fn = f, .run = r})

static inline __fn	_match_seq(rl42_line *line, rl42_key_tree *current, const rl42_kb_event *event);

char	*ft_readline(const char *prompt) {
	rl42_line	line;
	__fn		match;
	char		*out;
	u8			rv;

	rl42_init();
	line = (rl42_line){
		.prompt.prompt = cstr_to_rl42str(prompt),
		.keyseq = vector(u32, 64, NULL),
		.line = vector(u32, 64, NULL),
		.i = 0
	};
	rv = 1;
	match.fn = NULL;
	term_apply_settings(TERM_SETTINGS_RL42);
	term_cursor_get_pos(&line.cursor.prompt_row, &line.cursor.prompt_col);
	term_display_line(&line, DISPLAY_PROMPT_ONLY);
	term_cursor_get_pos(&line.cursor.input_row, &line.cursor.input_col);
	line.cursor.row = line.cursor.input_row;
	line.cursor.col = line.cursor.input_col;
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
	return out;
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
	if (event->mods & KB_MOD_CTRL)
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
