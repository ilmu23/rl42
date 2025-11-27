// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<tilde_expand.c>>

#include <stdlib.h>

#include "internal/_defs.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

#include "internal/fn/move.h"

rl42_fn(tilde_expand) {
	const char	*_home;
	cvector		home;
	size_t		len;
	size_t		i;
	u32			c;
	u8			rv;

	len = vector_size(line->line);
	if (!len)
		return 1;
	rv = 1;
	c = '\0';
	i = line->i;
	home = NULL;
	if (line->i == len || is_space(*(u32 *)vector_get(line->line, line->i)))
		line->i -= (line->i != 0) ? 1 : 0;
	if (is_space(*(u32 *)vector_get(line->line, line->i)))
		goto _tilde_expand_err_reset_i;
	if (line->i != 0) for (c = *(u32 *)vector_get(line->line, line->i - 1); !is_space(c) && c != '~'; c = *(u32 *)vector_get(line->line, line->i))
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
	len = vector_size(home);
	if (!vector_insert_n(line->line, line->i, len, vector_start(home)))
		goto _tilde_expand_err;
	line->i += len;
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
