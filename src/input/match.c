// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<match.c>>

#include "internal/_kb.h"
#include "internal/_map.h"
#include "internal/_vector.h"
#include "internal/_keybinds.h"

#define __fn_match(f, r)	((rl42_fn_match){.fn = f, .run = r})

rl42_fn_match	kb_match_seq(rl42_line *line, rl42_key_tree *current, const rl42_kb_event *event) {
	rl42_key_tree	**tmp;
	rl42_key_tree	*binds;
	size_t			len;
	size_t			i;
	u32				ucp;

	if (!event)
		return __fn_match(current, 1);
	ucp = kb_event_to_ucp(event);
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
		return __fn_match(NULL, 0);
	}
	return __fn_match(binds, (binds->f && (!binds->next || map_empty(binds->next))) ? 1 : 0);
}
