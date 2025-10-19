// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<tilde_expand.c>>

#include <ctype.h>
#include <stdlib.h>

#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

#include "internal/fn/move.h"

rl42_fn(tilde_expand) {
	const char	*_home;
	cvector		home;
	size_t		len;
	size_t		i;
	size_t		j;
	u32			c;
	u8			rv;

	len = vector_size(line->line);
	if (!len)
		return 1;
	rv = 1;
	c = '\0';
	i = line->i;
	home = NULL;
	if (line->i == len || (line->i != 0 && isspace(*(u32 *)vector_get(line->line, line->i))))
		line->i--;
	if (isspace(*(u32 *)vector_get(line->line, line->i)))
		goto _tilde_expand_err_reset_i;
	if (line->i != 0) for (c = *(u32 *)vector_get(line->line, line->i - 1); !isspace(c) && c != '~'; c = *(u32 *)vector_get(line->line, line->i))
		line->i--;
	c = *(u32 *)vector_get(line->line, line->i);
	if (c != '~')
		goto _tilde_expand_err_reset_i;
	_home = getenv("HOME");
	if (!_home)
		goto _tilde_expand_err_reset_i;
	rv = 0;
	home = cstr_to_rl42str(_home);
	if (!home)
		goto _tilde_expand_err_reset_i;
	vector_erase(line->line, line->i);
	for (j = 0, len = vector_size(home); j < len; j++, line->i++)
		if (!__vec_ins(line->line, line->i, vector_get(home, j)))
			goto _tilde_expand_err;
	if (!term_display_line(line, 0))
		goto _tilde_expand_err;
	vector_delete((vector)home);
	return forward_word(line);
_tilde_expand_err_reset_i:
	line->i = i;
_tilde_expand_err:
	vector_delete((vector)home);
	return rv;
}
