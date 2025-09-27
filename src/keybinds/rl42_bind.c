// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42: rl42_bind.c>>

#include <errno.h>
#include <string.h>

#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_function.h"
#include "internal/_keybinds.h"

static inline u8	_unbind(const char *seq, rl42_key_tree *node, const rl42_editing_mode emode);
static inline u8	_rebind(const char *seq, const char *f, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_bind_mode bmode, const rl42_editing_mode emode);
static inline u8	_bind(const char *seq, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_editing_mode emode);

u8 rl42_bind(const char *seq, const char *f, const rl42_bind_mode bmode, const rl42_editing_mode emode) {
	rl42_key_tree	*tmp;
	rl42_key_tree	*binds;
	rl42_fn_info	*fninfo;
	vector			expanded_seq;
	size_t			len;
	size_t			i;

	if (!rl42_init()) {
		error("rl42: unable to initialize: %s", (errno) ? strerror(errno) : "unknown error");
		return 0;
	}
	fninfo = get_fn_info_name(f);
	if (!fninfo)
		return (!(bmode & 1)) ? warn("rl42: rl42_bind(%s, %s): function not found\n", seq, f) : 0;
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
		return (!(bmode & 1)) ? warn("rl42: rl42_bind(%s, %s): sequence already const bound\n", seq, f) : 0;
	return (!binds->f) ? _bind(seq, binds, fninfo, emode) : _rebind(seq, f, binds, fninfo, bmode, emode);
}

u8	rl42_unbind(const char *seq, const rl42_editing_mode emode) {
	rl42_key_tree	**tmp;
	rl42_key_tree	*binds;
	vector			expanded_seq;
	size_t			len;
	size_t			i;

	errno = 0;
	if (!rl42_init()) {
		error("rl42: unable to initialize: %s", (errno) ? strerror(errno) : "unknown error");
		return 0;
	}
	expanded_seq = expand_seq(seq);
	if (!expanded_seq || expanded_seq == EXPAND_INVALID_SEQ)
		return error("rl42: rl42_unbind(%s): %s\n", seq, (expanded_seq == NULL) ? strerror(errno) : "invalid key sequence");
	for (i = 0, len = vector_size(expanded_seq), binds = get_key_tree(emode); i < len; i++) {
		tmp = map_get(binds->next, *(u32 *)vector_get(expanded_seq, i));
		if (tmp == MAP_NOT_FOUND) {
			vector_delete(expanded_seq);
			return error("rl42: rl42_unbind(%s): sequence not bound\n", seq);
		}
		binds = *tmp;
	}
	vector_delete(expanded_seq);
	if (binds->c)
		return error("rl42: rl42_unbind(%s): sequence is const bound\n", seq);
	if (!_unbind(seq, binds, emode))
		return error("rl42: rl42_unbind(%s): unable to unbind: %s\n", seq, (errno) ? strerror(errno) : "unknown error");
	binds->f = NULL;
	return 1;
}

static inline u8	_unbind(const char *seq, rl42_key_tree *node, const rl42_editing_mode emode) {
	rl42_fn_info	*fn;
	vector			vec;
	size_t			size;
	size_t			i;

	fn = get_fn_info_fn(node->f);
	vec = fn->binds[(emode != CURRENT) ? emode : get_editing_mode()];
	for (i = 0, size = vector_size(vec); i < size; i++)
		if (strcmp(seq, *(const char **)vector_get(vec, i)) == 0)
			break ;
	return vector_erase(vec, i);
}

static inline u8	_rebind(const char *seq, const char *f, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_bind_mode bmode, const rl42_editing_mode emode) {
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
	return (_unbind(seq, node, emode)) ? _bind(seq, node, fninfo, emode) : 0;
}

static inline u8	_bind(const char *seq, rl42_key_tree *node, rl42_fn_info *fninfo, const rl42_editing_mode emode) {
	const char	*_seq;

	node->f = fninfo->f;
//	TODO: Macro implementation
//	if (fninfo->f == __macro)
//		return _macro(seq, f);
	_seq = strdup(seq);
	vector_push(fninfo->binds[(emode != CURRENT) ? emode : get_editing_mode()], _seq);
	return 1;
}
