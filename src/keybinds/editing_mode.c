// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_key_tree.c>>

#include <stdlib.h>

#include "internal/_keybinds.h"

#define _KEY_TREE_ALLOC_OK	(\
		trees[RL42_EM_EMACS] &&\
		trees[RL42_EM_VI_CMD] &&\
		trees[RL42_EM_VI_INS] &&\
		trees[RL42_EM_CUSTOM_1] &&\
		trees[RL42_EM_CUSTOM_2] &&\
		trees[RL42_EM_CUSTOM_3] &&\
		trees[RL42_EM_CUSTOM_4] &&\
		trees[RL42_EM_CUSTOM_5]\
)

static rl42_editing_mode	current_mode = RL42_EM_EMACS;
static rl42_key_tree		*trees[RL42_EM_CUSTOM_5 + 1];

rl42_editing_mode	get_editing_mode(void) {
	return current_mode;
}

void	set_editing_mode(const rl42_editing_mode mode) {
	switch (mode) {
		case RL42_EM_EMACS:
		case RL42_EM_VI_CMD:
		case RL42_EM_VI_INS:
		case RL42_EM_CUSTOM_1:
		case RL42_EM_CUSTOM_2:
		case RL42_EM_CUSTOM_3:
		case RL42_EM_CUSTOM_4:
		case RL42_EM_CUSTOM_5:
			current_mode = mode;
		default:
			break ;
	}
}

rl42_key_tree	*get_key_tree(const rl42_editing_mode mode) {
	if (mode == RL42_EM_CURRENT)
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
	free_key_tree_node(&trees[RL42_EM_EMACS]);
	free_key_tree_node(&trees[RL42_EM_VI_CMD]);
	free_key_tree_node(&trees[RL42_EM_VI_INS]);
	free_key_tree_node(&trees[RL42_EM_CUSTOM_1]);
	free_key_tree_node(&trees[RL42_EM_CUSTOM_2]);
	free_key_tree_node(&trees[RL42_EM_CUSTOM_3]);
	free_key_tree_node(&trees[RL42_EM_CUSTOM_4]);
	free_key_tree_node(&trees[RL42_EM_CUSTOM_5]);
}

u8	init_key_trees(void) {
	trees[RL42_EM_EMACS] = new_key_tree_node();
	trees[RL42_EM_VI_CMD] = new_key_tree_node();
	trees[RL42_EM_VI_INS] = new_key_tree_node();
	trees[RL42_EM_CUSTOM_1] = new_key_tree_node();
	trees[RL42_EM_CUSTOM_2] = new_key_tree_node();
	trees[RL42_EM_CUSTOM_3] = new_key_tree_node();
	trees[RL42_EM_CUSTOM_4] = new_key_tree_node();
	trees[RL42_EM_CUSTOM_5] = new_key_tree_node();
	return (_KEY_TREE_ALLOC_OK) ? 1 : 0;
}
