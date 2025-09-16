// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42: rl42_bind.c>>

#include "internal/_keybinds.h"

static inline u8	_rebind(const char *seq, const char *f, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_bind_mode bmode, const rl42_editing_mode emode);
static inline u8	_bind(const char *seq, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_editing_mode emode);

u8 rl42_bind(const char *seq, const char *f, const rl42_bind_mode bmode, const rl42_editing_mode emode) {
	rl42_key_tree	*tmp;
	rl42_key_tree	*binds;
	rl42_fn_info	*fninfo;
	vector			expanded_seq;
	size_t			len;
	size_t			i;

	rl42_init();
	fninfo = get_fn_info_name(f);
	if (!fninfo)
		return warn("rl42: rl42_bind(%s, %s): function not found\n", seq, f);
	expanded_seq = expand_seq(seq);
	if (!expanded_seq || expanded_seq == EXPAND_INVALID_SEQ)
		return error("rl42: rl42_bind(%s, %s): %s\n", seq, f, (expanded_seq == NULL) ? strerror(errno) : "invalid key sequence");
	for (i = 0, len = vector_size(expanded_seq), binds = get_key_tree(emode); i < len; i++) {
		tmp = map_get(binds->next, *(u32 *)vector_get(expanded_seq, i));
		if (tmp == MAP_NOT_FOUND) {
			tmp = new_key_tree_node();
			if (!tmp || !map_set(binds->next, *(u32 *)vector_get(expanded_seq, i), tmp)) {
				free_key_tree_node(&tmp);
				vector_delete(expanded_seq);
				return error("rl42: rl42_bind(%s, %s): %s\n", seq, f, strerror(errno));
			}
			binds = tmp;
		} else
			binds = *(rl42_key_tree **)tmp;
	}
	vector_delete(expanded_seq);
	if (binds->c)
		return warn("rl42: rl42_bind(%s, %s): sequence already const bound\n", seq, f);
	return (!binds->f) ? _bind(seq, binds, fninfo, emode) : _rebind(seq, f, binds, fninfo, bmode, emode);
}

static inline u8	_rebind(const char *seq, const char *f, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_bind_mode bmode, const rl42_editing_mode emode) {
	rl42_fn_info	*old_fn;
	vector			vec;
	size_t			size;
	size_t			i;

	switch (bmode) {
		case WARN:
			warn("rl42: rl42_bind(%s, %s): key sequence is already bound\n", seq, f);
			[[fallthrough]];
		case QUIET:
			return 0;
		case REMAP:
			info("rl42: rl42_bind(%s, %s): remapping key sequence\n", seq, f);
			[[fallthrough]];
		case QREMAP:
			break ;
	}
	old_fn = get_fn_info_fn(node->f);
	vec = old_fn->binds[(emode != CURRENT) ? emode : get_editing_mode()];
	for (i = 0, size = vector_size(vec); i < size; i++)
		if (strcmp(seq,*(const char **)vector_get(vec, i)) == 0)
			break ;
	vector_erase(vec, i);
	return _bind(seq, node, fninfo, emode);
}

static inline u8	_bind(const char *seq, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_editing_mode emode) {
	node->f = fninfo->f;
//	TODO: Macro implementation
//	if (fninfo->f == __macro)
//		return _macro(seq, f);
	vector_push(fninfo->binds[(emode != CURRENT) ? emode : get_editing_mode()], seq);
	return 1;
}
