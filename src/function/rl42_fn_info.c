// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_fn_info.c>>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_function.h"
#include "internal/_keybinds.h"

static vector	functions;

static void	_clean_fn_info(rl42_fn_info *f);

rl42_fn_info	*get_fn_info_keyseq(cvector expanded_seq, const rl42_editing_mode emode) {
	rl42_key_tree	*binds;
	size_t			len;
	size_t			i;

	for (i = 0, len = vector_size(expanded_seq), binds = get_key_tree(emode); i < len; i++) {
		binds = map_get(binds->next, *(u32 *)vector_get(expanded_seq, i));
		if (binds == MAP_NOT_FOUND)
			return NULL;
		binds = *(rl42_key_tree **)binds;
	}
	return get_fn_info_fn(binds->f);
}

rl42_fn_info	*get_fn_info_name(const char *f) {
	const rl42_fn_info	*fns;
	size_t				size;
	size_t				i;

	if (!functions || !f)
		return NULL;
	for (fns = vector_get(functions, 0), i = 0, size = vector_size(functions); i < size; i++)
		if (fns[i].fname && strcmp(fns[i].fname, f) == 0)
			return (rl42_fn_info *)&fns[i];
	return NULL;
}

rl42_fn_info	*get_fn_info_fn(rl42_fn f) {
	const rl42_fn_info	*fns;
	size_t				size;
	size_t				i;

	if (!functions || !f)
		return NULL;
	for (fns = vector_get(functions, 0), i = 0, size = vector_size(functions); i < size; i++)
		if (fns[i].f == f)
			return (rl42_fn_info *)&fns[i];
	return NULL;
}

cvector	get_fn_list(void) {
	return functions;
}

void	clean_fns(void) {
	delete_macros();
	vector_delete(functions);
}

u8	rl42_register_function(rl42_fn f, const char *fname) {
	const rl42_fn_info	*tmp;
	rl42_fn_info		new;

	if (!rl42_init()) {
		error("rl42: unable to initialize: %s", (errno) ? strerror(errno) : "unknown error");
		return 0;
	}
	if (!functions) {
		functions = vector(rl42_fn_info, FUNCTION_COUNT, (void (*)(void *))_clean_fn_info);
		if (!functions)
			return error("rl42_register_function: unable to create function database\n");
	}
	tmp = get_fn_info_fn(f);
	if (tmp)
		return error("rl42_register_function(%s): function already registered as '%s'\n", fname, tmp->fname);
	tmp = get_fn_info_name(fname);
	if (tmp)
		return error("rl42_register_function(%s): function name '%s' already taken\n", fname, tmp->fname);
	new = (rl42_fn_info){
		.f = f,
		.fname = strdup(fname),
		.macro = (state_flags & STATE_REGISTER_MACRO) ? 1 : 0,
		.binds[0] = vector(char *, 1, free),
		.binds[1] = vector(char *, 1, free),
		.binds[2] = vector(char *, 1, free)
	};
	if (!new.binds[0] || !new.binds[1] || !new.binds[2]) {
		vector_delete(new.binds[0]);
		vector_delete(new.binds[1]);
		vector_delete(new.binds[2]);
		return error("rl42_register_function(%s): %s\n", fname, strerror(errno));
	}
	return (vector_push(functions, new)) ? 1 : error("rl42_register_function(%s): %s\n", fname, strerror(errno));
}

static void	_clean_fn_info(rl42_fn_info *f) {
	vector_delete(f->binds[0]);
	vector_delete(f->binds[1]);
	vector_delete(f->binds[2]);
	free((void *)f->fname);
}
