// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_key_tree.c>>

#include "internal/_keybinds.h"

static rl42_editing_mode	current_mode;
static rl42_key_tree		*trees[3];

rl42_editing_mode	get_editing_mode(void) {
	return current_mode;
}

void	set_editing_mode(const rl42_editing_mode mode) {
	switch (mode) {
		case EMACS:
		case VI_CMD:
		case VI_INS:
			current_mode = mode;
		default:
			break ;
	}
}

rl42_key_tree	*get_key_tree(const rl42_editing_mode mode) {
	if (mode == CURRENT)
		return get_key_tree(current_mode);
	return trees[mode];
}

rl42_key_tree	*new_key_tree_node(void) {
	rl42_key_tree	*out;

	out = malloc(sizeof(*out));
	if (out) {
		*out = (rl42_key_tree){
			.f = NULL,
			.next = map(rl42_key_tree *, KEY_TREE_DEFAULT_MAP_SIZE, INTEGER, (void (*)(void *))free_key_tree_node),
			.c = 0
		};
		if (!out->next) {
			free(out);
			out = NULL;
		}
	}
	return out;
}

void	free_key_tree_node(rl42_key_tree **node) {
	map_delete((*node)->next);
	free(*node);
}

void	clean_key_trees(void) {
	free_key_tree_node(&trees[EMACS]);
	free_key_tree_node(&trees[VI_CMD]);
	free_key_tree_node(&trees[VI_INS]);
}

u8	init_key_trees(void) {
	trees[EMACS] = new_key_tree_node();
	trees[VI_CMD] = new_key_tree_node();
	trees[VI_INS] = new_key_tree_node();
	return (trees[EMACS] && trees[VI_CMD] && trees[VI_INS]) ? 1 : 0;
}
