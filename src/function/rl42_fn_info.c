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

#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_function.h"

static vector	functions;

static void	_clean_fn_info(rl42_fn_info *f);

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

void	clean_fns(void) {
	vector_delete(functions);
}

u8	rl42_register_function(rl42_fn f, const char *fname) {
	const rl42_fn_info	*tmp;
	rl42_fn_info		new;

	rl42_init();
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
		.fname = fname,
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
}
