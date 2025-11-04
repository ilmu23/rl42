// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<complete.c>>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ < 202311L
#include <bsd/string.h>
#endif

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_complete.h"

#define on_word(line)	((line->i < vector_size(line->line) && !isspace(*(u32 *)vector_get(line->line, line->i))) \
						|| (line->i > 0 && !isspace(*(u32 *)vector_get(line->line, line->i - 1))))

typedef struct {
	const char	*pattern;
	cvector		context;
}	_cmp_info;

extern rl42_completion_fn	cmp_fn;

static inline const char	*_substr(cvector s, const size_t start, size_t len);
static inline _cmp_info		_get_target(rl42_line *line);

#include <stdio.h>

rl42_fn(complete) {
	_cmp_info	target;
	cvector		completions;
	size_t		size;
	size_t		i;
	u8			rv;

	rv = 0;
	target = (vector_size(line->line) != 0) ? _get_target(line) : (_cmp_info){ .pattern = strdup("") };
	if (!target.pattern)
		goto _complete_ret_cleanup;
	completions = cmp_get_common(cmp_fn(target.pattern, target.context));
	if (completions) {
		rv = 1;
		fputc('\n', stderr);
		for (i = 0, size = vector_size(completions); i < size; i++)
			info("rl42: complete: completions[%zu]: '%s'\n", i, *(const char **)vector_get(completions, i));
	} else
		rv = 0;
_complete_ret_cleanup:
	state_flags &= ~STATE_KILL_DONT_UPDATE_RING;
	vector_delete((vector)target.context);
	free((void *)target.pattern);
	kill_start.set = 0;
	kill_end.set = 0;
	return rv;
}

static inline const char	*_substr(cvector s, const size_t start, size_t len) {
	utf8_cbuf	buf;
	size_t		asize;
	size_t		i;
	char		*out;

	asize = ((len * 4) + 1) * sizeof(*out);
	out = malloc(asize);
	if (out) for (i = start, *out = '\0'; len--; i++)
		strlcat(out, utf8_encode(*(u32 *)vector_get(s, i), buf), asize);
	return out;
}

static inline _cmp_info	_get_target(rl42_line *line) {
	const char	*_tmp;
	_cmp_info	target;
	cvector		tmp;

	tmp = NULL;
	_tmp = NULL;
	if (on_word(line)) {
		if (line->i > 0 && !isspace(*(u32 *)vector_get(line->line, line->i - 1)))
			move_to_start_of_word(line);
		add_mark(kill_start, line->i);
		move_to_end_of_word(line);
		add_mark(kill_end, line->i);
		target.pattern = _substr(line->line, kill_start.pos, kill_end.pos - kill_start.pos);
	} else {
		add_mark(kill_start, line->i);
		add_mark(kill_end, line->i);
		target.pattern = strdup("");
	}
	if (!target.pattern)
		goto __get_target_error;
	tmp = vector_copy_range(line->line, 0, kill_start.pos, NULL);
	if (!tmp)
		goto __get_target_error;
	if (vector_size(tmp) != 0) {
		_tmp = rl42str_to_cstr(tmp);
		if (!_tmp)
			goto __get_target_error;
		target.context = cstr_split(_tmp, ' ', "'\"");
	} else
		target.context = NULL;
	vector_delete((vector)tmp);
	free((void *)_tmp);
	return target;
__get_target_error:
	vector_delete((vector)target.context);
	vector_delete((vector)tmp);
	free((void *)target.pattern);
	free((void *)_tmp);
	target.pattern = NULL;
	return target;
}
