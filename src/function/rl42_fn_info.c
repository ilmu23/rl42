// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_fn_info.c>>

#include "internal/_function.h"

#define __fninfo(fninfo)	(*(rl42_fn_info *)*fninfo)

static vector	*functions = NULL;

static inline u8	_compare_fn_name(const uintptr_t *fname, const uintptr_t *fninfo);
static inline u8	_compare_fn(const uintptr_t *f, const uintptr_t *fninfo);

rl42_fn_info	*get_fn_info_name(const char *f) {
	rl42_fn_info	*out;

	if (!functions)
		return NULL;
	out = (rl42_fn_info *)vector_get(functions, vector_find(functions, (uintptr_t)f, _compare_fn_name));
	return (out != (rl42_fn_info *)VECTOR_OUT_OF_BOUNDS) ? out : NULL;
}

rl42_fn_info	*get_fn_info(rl42_fn f) {
	rl42_fn_info	*out;

	if (!functions)
		return NULL;
	out = (rl42_fn_info *)vector_get(functions, vector_find(functions, (uintptr_t)f, _compare_fn));
	return (out != (rl42_fn_info *)VECTOR_OUT_OF_BOUNDS) ? out : NULL;
}

u8	rl42_register_function(rl42_fn f, const char *fname) {
	rl42_fn_info	*new;

	if (!functions) {
		functions = vector_new(FUNCTION_COUNT, ASCENDING);
		if (!functions)
			return error("rl42_register_function: unable to create function database\n");
	}
	new = get_fn_info(f);
	if (new)
		return error("rl42_register_function(%s): function already registered as '%s'\n", fname, new->fname);
	new = get_fn_info_name(fname);
	if (new)
		return error("rl42_register_function(%s): function name '%s' already taken\n", fname, new->fname);
	new = malloc(sizeof(*new));
	if (new) {
		*new = (rl42_fn_info) {
			.f = f,
			.fname = fname,
			.binds[0] = vector_new(1, ASCENDING),
			.binds[1] = vector_new(1, ASCENDING),
			.binds[2] = vector_new(1, ASCENDING)
		};
		if (!new->binds[0] || !new->binds[1] || !new->binds[2]) {
			vector_delete(new->binds[0], NULL);
			vector_delete(new->binds[1], NULL);
			vector_delete(new->binds[2], NULL);
			free(new);
			return error("rl42_register_function(%s): %s\n", fname, strerror(errno));
		}
	}
	return (vector_add(functions, (uintptr_t)new)) ? 1 : error("rl42_register_function(%s): %s\n", fname, strerror(errno));
}

static inline u8	_compare_fn_name(const uintptr_t *fname, const uintptr_t *fninfo) {
	if (*(const char **)fname == __fninfo(fninfo).fname)
		return 1;
	return (*(const char **)fname && __fninfo(fninfo).fname &&
			strcmp(*(const char **)fname, __fninfo(fninfo).fname) == 0) ? 1 : 0;
}

static inline u8	_compare_fn(const uintptr_t *f, const uintptr_t *fninfo) {
	return (*(rl42_fn *)f == __fninfo(fninfo).f) ? 1 : 0;
}
