// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42.c>>

#include "internal/_kb.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_keybinds.h"

#define _AMBIGUOUS_TIMEOUT 0

typedef struct {
	rl42_key_tree	*fn;
	u8				run;
}	__fn;

#define __fn(f, r)	((__fn){.fn = f, .run = r})

static inline __fn	_match_seq(rl42_line *input, rl42_key_tree *current, const rl42_kb_event *event);

char	*ft_readline(const char *prompt) {
	rl42_line	input;
	__fn		match;
	char		*out;
	u8			rv;

	rl42_init();
	input = (rl42_line){
		.prompt.prompt = cstr_to_rl42str(prompt),
		.keyseq = vector(u32, 64, NULL),
		.line = vector(u32, 64, NULL),
		.i = 0
	};
	rv = 1;
	match.fn = NULL;
	term_apply_settings(TERM_SETTINGS_RL42);
	do {
		match = _match_seq(&input, match.fn, kb_listen((match.fn && match.fn->f) ? _AMBIGUOUS_TIMEOUT : -1));
		if (match.fn && match.run) {
			rv = match.fn->f(&input);
			vector_clear(input.keyseq);
			match.fn = NULL;
		}
	} while (rv);
	term_apply_settings(TERM_SETTINGS_DEFAULT);
	out = rl42str_to_cstr(input.line);
	vector_delete(input.prompt.prompt);
	vector_delete(input.keyseq);
	vector_delete(input.line);
	return out;
}

static inline __fn	_match_seq(rl42_line *input, rl42_key_tree *current, const rl42_kb_event *event) {
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
		vector_push(input->keyseq, (u32){'\x1b'});
	vector_push(input->keyseq, ucp);
	for (i = 0, len = vector_size(input->keyseq), binds = get_key_tree(CURRENT); i < len && binds->next; i++) {
		tmp = map_get(binds->next, *(u32 *)vector_get(input->keyseq, i));
		if (tmp == MAP_NOT_FOUND)
			break ;
		binds = *tmp;
	}
	if (i < len || tmp == MAP_NOT_FOUND) {
		vector_clear(input->keyseq);
		return __fn(NULL, 0);
	}
	return __fn(binds, (binds->f && (!binds->next || map_empty(binds->next))) ? 1 : 0);
}
