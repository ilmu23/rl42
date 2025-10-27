// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<dump_functions.c>>

#include "internal/_defs.h"
#include "internal/_rl42.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_function.h"
#include "internal/_keybinds.h"
#include "internal/_terminfo_caps.h"

#define __DUMP_FN
#include "internal/fn/misc.h"

#define get_1_bind(bv)	*(const char **)vector_get(bv, 0)
#define get_2_binds(bv)	get_1_bind(bv), *(const char **)vector_get(bv, 1)
#define get_3_binds(bv)	get_2_binds(bv), *(const char **)vector_get(bv, 2)
#define get_4_binds(bv)	get_3_binds(bv), *(const char **)vector_get(bv, 3)
#define get_5_binds(bv)	get_4_binds(bv), *(const char **)vector_get(bv, 4)

static inline u8	_dump_config(rl42_line *line);
static inline u8	_dump_human(rl42_line *line);

rl42_fn(dump_functions) {
	return (get_numeric_arg(line, 1) == NUMERIC_ARG_NOT_SET) ? _dump_human(line) : _dump_config(line);
}

static inline u8	_dump_config(rl42_line *line) {
	const rl42_fn_info	*info;
	rl42_editing_mode	emode;
	static const char	*emode_strs[3] = { "emacs", "vi-cmd", "vi-ins" };
	const char			*tmp;
	cvector				funcs;
	size_t				binds;
	size_t				size;
	size_t				i;
	size_t				j;
	i16					row_diff;
	i16					col_diff;

	funcs = get_fn_list();
	tmp = term_get_seq(ti_ed);
	if (add_to_dump_buf("\n%s", (tmp) ? tmp : "") == -1)
		return 0;
	if (~state_flags & STATE_DUMP_MACROS) for (emode = EMACS; emode < CURRENT; emode++) {
		for (i = 0, size = vector_size(funcs); i < size; i++) {
			info = vector_get(funcs, i);
			if (info->macro)
				continue ;
			for (j = 0, binds = vector_size(info->binds[emode]); j < binds; j++)
				if (add_to_dump_buf("bind\t%s\t%s\t%s\n", *(const char **)vector_get(info->binds[emode], j), info->fname, emode_strs[emode]) == -1)
					return 0;
		}
	} else for (emode = EMACS; emode < CURRENT; emode++) {
		for (i = 0, size = vector_size(funcs); i < size; i++) {
			info = vector_get(funcs, i);
			if (!info->macro)
				continue ;
			for (j = 0, binds = vector_size(info->binds[emode]); j < binds; j++)
				if (add_to_dump_buf("bind\t%s\t\"%s\"\t%s\n", *(const char **)vector_get(info->binds[emode], j), get_macro_content(info->f), emode_strs[emode]) == -1)
					return 0;
		}
	}
	if (flush_dump_buf() == -1)
		return 0;
	row_diff = line->root.row - line->prompt.root.row;
	col_diff = line->root.col - line->prompt.root.col;
	term_cursor_get_pos(&line->prompt.root.row, &line->prompt.root.col);
	line->root.row = line->prompt.root.row + row_diff;
	line->root.col = line->prompt.root.col + col_diff;
	return term_display_line(line, 0);
}

static inline u8	_dump_human(rl42_line *line) {
	const rl42_fn_info	*info;
	rl42_editing_mode	emode;
	const char			*tmp;
	cvector				funcs;
	size_t				size;
	size_t				i;
	i16					row_diff;
	i16					col_diff;

	funcs = get_fn_list();
	tmp = term_get_seq(ti_ed);
	emode = get_editing_mode();
	if (add_to_dump_buf("\n%s", (tmp) ? tmp : "") == -1)
		return 0;
	if (~state_flags & STATE_DUMP_MACROS) for (i = 0, size = vector_size(funcs); i < size; i++) {
		info = vector_get(funcs, i);
		if (info->macro)
			continue ;
		switch (vector_size(info->binds[emode])) {
			case 0:
				if (add_to_dump_buf("%s is not bound\n", info->fname) == -1)
					return 0;
				break ;
			case 1:
				if (add_to_dump_buf("%s is bound to %s\n", info->fname, get_1_bind(info->binds[emode])) == -1)
					return 0;
				break ;
			case 2:
				if (add_to_dump_buf("%s is bound to %s, %s\n", info->fname, get_2_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			case 3:
				if (add_to_dump_buf("%s is bound to %s, %s, %s\n", info->fname, get_3_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			case 4:
				if (add_to_dump_buf("%s is bound to %s, %s, %s, %s\n", info->fname, get_4_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			case 5:
				if (add_to_dump_buf("%s is bound to %s, %s, %s, %s, %s\n", info->fname, get_5_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			default:
				if (add_to_dump_buf("%s is bound to %s, %s, %s, %s, %s...\n", info->fname, get_5_binds(info->binds[emode])) == -1)
					return 0;
				break ;
		}
	} else for (i = 0, size = vector_size(funcs); i < size; i++) {
		info = vector_get(funcs, i);
		if (!info->macro)
			continue ;
		switch (vector_size(info->binds[emode])) {
			case 0:
				if (add_to_dump_buf("%s ('%s') is not bound\n", info->fname, get_macro_content(info->f)) == -1)
					return 0;
				break ;
			case 1:
				if (add_to_dump_buf("%s ('%s') is bound to %s\n", info->fname, get_macro_content(info->f), get_1_bind(info->binds[emode])) == -1)
					return 0;
				break ;
			case 2:
				if (add_to_dump_buf("%s ('%s') is bound to %s, %s\n", info->fname, get_macro_content(info->f), get_2_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			case 3:
				if (add_to_dump_buf("%s ('%s') is bound to %s, %s, %s\n", info->fname, get_macro_content(info->f), get_3_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			case 4:
				if (add_to_dump_buf("%s ('%s') is bound to %s, %s, %s, %s\n", info->fname, get_macro_content(info->f), get_4_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			case 5:
				if (add_to_dump_buf("%s ('%s') is bound to %s, %s, %s, %s, %s\n", info->fname, get_macro_content(info->f), get_5_binds(info->binds[emode])) == -1)
					return 0;
				break ;
			default:
				if (add_to_dump_buf("%s ('%s') is bound to %s, %s, %s, %s, %s...\n", info->fname, get_macro_content(info->f), get_5_binds(info->binds[emode])) == -1)
					return 0;
				break ;
		}
	}
	if (flush_dump_buf() == -1)
		return 0;
	row_diff = line->root.row - line->prompt.root.row;
	col_diff = line->root.col - line->prompt.root.col;
	term_cursor_get_pos(&line->prompt.root.row, &line->prompt.root.col);
	line->root.row = line->prompt.root.row + row_diff;
	line->root.col = line->prompt.root.col + col_diff;
	return term_display_line(line, 0);
}
