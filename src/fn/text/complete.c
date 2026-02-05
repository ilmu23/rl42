// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<complete.c>>

#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ < 202311L
# include <bsd/string.h>
#endif

#define __RL42_INTERNAL
#include "rl42.h"
#include "function.h"

#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_darray.h"
#include "internal/_complete.h"

#define on_word(line)	((line->i < darray_size(line->line) && !is_space(*(u32 *)darray_get(line->line, line->i))) \
						|| (line->i > 0 && !is_space(*(u32 *)darray_get(line->line, line->i - 1))))

typedef struct {
	const char	*pattern;
	cdarray		context;
}	_cmp_info;

extern rl42_completion_fn	cmp_fn;

static inline const char	*_substr(cdarray s, const size_t start, size_t len);
static inline _cmp_info		_get_target(rl42_line *line);

rl42_fn(complete) {
	_cmp_info	target;
	cdarray		completions;
	u8			rv;

	if (rl42_get(RL42_DISABLE_COMPLETION).u64 == rl42_conf_on)
		return 1;
	rv = 0;
	if (darray_size(line->line) == 0) {
		target.context = NULL;
		target.pattern = strdup("");
		add_mark(kill_start, 0);
	} else
		target = _get_target(line);
	if (!target.pattern)
		goto _complete_ret_cleanup;
	completions = cmp_get_common(cmp_fn(target.pattern, target.context), strlen(target.pattern));
	if (completions) {
		state_flags |= STATE_KILL_DONT_UPDATE_RING;
		switch (darray_size(completions)) {
			case 0:
				rv = 1;
				break ;
			case 1:
				rv = cmp_insert(line, *(const char **)darray_get(completions, 0));
				break ;
			default:
				rv = cmp_display(line, completions);
		}
	} else
		rv = 0;
	darray_delete((darray)completions);
_complete_ret_cleanup:
	state_flags &= ~STATE_KILL_DONT_UPDATE_RING;
	darray_delete((darray)target.context);
	free((void *)target.pattern);
	kill_start.set = 0;
	kill_end.set = 0;
	return rv;
}

static inline const char	*_substr(cdarray s, const size_t start, size_t len) {
	utf8_cbuf	buf;
	size_t		asize;
	size_t		i;
	char		*out;

	asize = ((len * 4) + 1) * sizeof(*out);
	out = malloc(asize);
	if (out) for (i = start, *out = '\0'; len--; i++)
		strlcat(out, utf8_encode(*(u32 *)darray_get(s, i), buf), asize);
	return out;
}

static inline _cmp_info	_get_target(rl42_line *line) {
	const char	*_tmp;
	_cmp_info	target;
	cdarray		tmp;

	tmp = NULL;
	_tmp = NULL;
	target.context = NULL;
	if (on_word(line)) {
		if (line->i > 0 && !is_space(*(u32 *)darray_get(line->line, line->i - 1)))
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
	if (rl42_get(RL42_EXPAND_TILDE).u64 == rl42_conf_on && *target.pattern == '~') {
		_tmp = rl42_getenv("HOME");
		if (_tmp) {
			_tmp = cstr_join(_tmp, &target.pattern[1]);
			if (!_tmp)
				goto __get_target_error;
			free((void *)target.pattern);
			target.pattern = _tmp;
		}
		_tmp = NULL;
	}
	tmp = darray_copy_range(line->line, 0, kill_start.pos, NULL);
	if (!tmp)
		goto __get_target_error;
	if (darray_size(tmp) != 0) {
		_tmp = rl42str_to_cstr(tmp);
		if (!_tmp)
			goto __get_target_error;
		target.context = cstr_split(_tmp, ' ', "'\"");
	}
	darray_delete((darray)tmp);
	free((void *)_tmp);
	return target;
__get_target_error:
	darray_delete((darray)target.context);
	darray_delete((darray)tmp);
	free((void *)target.pattern);
	free((void *)_tmp);
	target.pattern = NULL;
	return target;
}
