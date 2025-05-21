// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42: rl42_bind.c>>

#include "internal/_keybinds.h"

static inline i32	_rl42c_to_i32(const char rl42c[4]);
static inline u8	_rebind(const char *seq, const char *f, rl42_key_tree *node, const rl42_fn_info *fninfo, const rl42_bind_mode bmode, const rl42_editing_mode emode);
static inline u8	_bind(const char *seq, [[maybe_unused]] const char *f, rl42_key_tree *node, const rl42_fn_info *fninfo, const rl42_editing_mode emode);

u8 rl42_bind(const char *seq, const char *f, const rl42_bind_mode bmode, const rl42_editing_mode emode) {
	const rl42_string	*expanded_seq;
	rl42_key_tree		*tmp;
	rl42_key_tree		*binds;
	rl42_fn_info		*fninfo;
	size_t				i;

	rl42_init();
	fninfo = get_fn_info_name(f);
	if (!fninfo)
		return warn("rl42: rl42_bind(%s, %s): function not found\n", seq, f);
	expanded_seq = expand_seq(seq);
	if (!expanded_seq || expanded_seq == EXPAND_INVALID_SEQ)
		return error("rl42: rl42_bind(%s, %s): %s\n", seq, f, (expanded_seq == NULL) ? strerror(errno) : "invalid key sequence");
	for (i = 0, binds = get_key_tree(emode); i < expanded_seq->len; i++) {
		tmp = map_find(binds->next, (u32)*expanded_seq->str[i].cbuf);
		if (tmp == MAP_NOT_FOUND) {
			tmp = new_key_tree_node();
			if (!tmp || !map_add(binds->next, _rl42c_to_i32(expanded_seq->str[i].cbuf), tmp)) {
				free_key_tree_node(tmp);
				free_rlstring(expanded_seq);
				return error("rl42: rl42_bind(%s, %s): %s\n", seq, f, strerror(errno));
			}
		}
		binds = tmp;
	}
	free_rlstring(expanded_seq);
	if (binds->c)
		return warn("rl42: rl42_bind(%s, %s): sequence already const bound\n", seq, f);
	return (!binds->f) ? _bind(seq, f, binds, fninfo, emode) : _rebind(seq, f, binds, fninfo, bmode, emode);
}

static inline i32	_rl42c_to_i32(const char rl42c[4]) {
	i32	out;
	u8	i;

	for (i = out = 0; i < 4; i++)
		out |= rl42c[i] << (i * 8);
	return out;
}

static inline u8	_rebind(const char *seq, const char *f, rl42_key_tree *node, const rl42_fn_info *fninfo, const rl42_bind_mode bmode, const rl42_editing_mode emode) {
	rl42_fn_info	*old_fn;

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
	old_fn = get_fn_info(node->f);
	vector_remove(old_fn->binds[(emode != CURRENT) ? emode : get_editing_mode()], vector_find(old_fn->binds[(emode != CURRENT) ? emode : get_editing_mode()], (uintptr_t)seq, NULL), NULL);
	return _bind(seq, f, node, fninfo, emode);
}

static inline u8	_bind(const char *seq, [[maybe_unused]] const char *f, rl42_key_tree *node, const rl42_fn_info *fninfo, const rl42_editing_mode emode) {
	node->f = fninfo->f;
//	TODO: Macro implementation
//	if (fninfo->f == __macro)
//		return _macro(seq, f);
	vector_add(fninfo->binds[(emode != CURRENT) ? emode : get_editing_mode()], (uintptr_t)seq);
	return 1;
}
