// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<dump_variables.c>>

#include <stdio.h>
#include <stdarg.h>

#include "rl42.h"

#include "internal/_defs.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_display.h"
#include "internal/_keybinds.h"
#include "internal/_terminfo_caps.h"

#include "internal/fn/misc.h"

typedef struct {
	rl42_setting	code;
	const char		*name;
}	_setting_info;

static const _setting_info	s_numeric[] = {
	{ .code = RL42_COMPLETION_DISPLAY_WIDTH, .name = "completion-display-width" },
	{ .code = RL42_HISTORY_SIZE, .name = "history-size" },
	{ .code = RL42_KEYSEQ_TIMEOUT, .name = "keyseq-timeout" },
};

static const _setting_info	u_numeric[] = {
	{ .code = RL42_COMPLETION_QUERY_ITEMS, .name = "completion-query-items" },
};

static const _setting_info	boolean[] = {
	{ .code = RL42_COLORED_COMPLETION_PREFIX, .name = "colored-completion-prefix" },
	{ .code = RL42_COMPLETION_IGNORE_CASE, .name = "completion-ignore-case" },
	{ .code = RL42_COMPLETION_MAP_CASE, .name = "completion-map-case" },
	{ .code = RL42_DISABLE_COMPLETION, .name = "disable-completion" },
	{ .code = RL42_ENABLE_HIGHLIGHT, .name = "enable-highlight" },
	{ .code = RL42_EXPAND_TILDE, .name = "expand-tilde" },
	{ .code = RL42_HORIZONTAL_SCROLL_MODE, .name = "horizontal-scroll-mode" },
	{ .code = RL42_MARK_DIRECTORIES, .name = "mark-directories" },
	{ .code = RL42_MARK_SYMLINKED_DIRECTORIES, .name = "mark-symlinked-directories" },
	{ .code = RL42_MATCH_HIDDEN_FILES, .name = "match-hidden-files" },
	{ .code = RL42_PAGE_COMPLETIONS, .name = "page-completions" },
	{ .code = RL42_SEARCH_IGNORE_CASE, .name = "search-ignore-case" },
	{ .code = RL42_VISIBLE_STATS, .name = "visible-stats" },
};

#define _S_NUMERIC_SETTING_COUNT	(sizeof(s_numeric) / sizeof(*s_numeric))
#define _U_NUMERIC_SETTING_COUNT	(sizeof(u_numeric) / sizeof(*u_numeric))
#define _BOOLEAN_SETTING_COUNT		(sizeof(boolean) / sizeof(*boolean))

static inline u8	_dump_config(rl42_line *line);
static inline u8	_dump_human(rl42_line *line);

rl42_fn(dump_variables) {
	return (get_numeric_arg(line, 1) == NUMERIC_ARG_NOT_SET) ? _dump_human(line) : _dump_config(line);
}

static inline u8	_dump_config(rl42_line *line) {
	const char	*tmp;
	size_t		i;
	i16			row_diff;
	i16			col_diff;

	tmp = term_get_seq(ti_ed);
	if (term_putsf("\n%s", (tmp) ? tmp : "") == -1)
		return 0;
	switch (rl42_get(RL42_BELL_STYLE).i64) {
		case RL42_BELL_NONE:
			tmp = "none";
			break ;
		case RL42_BELL_AUDIBLE:
			tmp = "audible";
			break ;
		case RL42_BELL_VISIBLE:
			tmp = "visible";
			break ;
		default:
			tmp = NULL;
	}
	if (tmp && term_putsf("set\tbell-style\t%s\n", tmp) == -1)
		return 0;
	switch (get_editing_mode()) {
		case EMACS:
			tmp = "emacs";
			break ;
		default:
			tmp = "vi";
	}
	if (term_putsf("set\tediting-mode\t%s\n", tmp) == -1)
		return 0;
	for (i = 0; i < _S_NUMERIC_SETTING_COUNT; i++)
		if (term_putsf("set\t%s\t%ld\n", s_numeric[i].name, rl42_get(s_numeric[i].code).i64) == -1)
			return 0;
	for (i = 0; i < _U_NUMERIC_SETTING_COUNT; i++)
		if (term_putsf("set\t%s\t%lu\n", u_numeric[i].name, rl42_get(u_numeric[i].code).u64) == -1)
			return 0;
	for (i = 0; i < _BOOLEAN_SETTING_COUNT; i++)
		if (term_putsf("set\t%s\t%s\n",boolean[i].name, (rl42_get(boolean[i].code).u64) ? "on" : "off") == -1)
			return 0;
	if (!term_flush_outbuf())
		return 0;
	row_diff = line->root->row - line->prompt.root->row;
	col_diff = line->root->col - line->prompt.root->col;
	term_cursor_get_pos((i16 *)&line->prompt.root->row, (i16 *)&line->prompt.root->col);
	((rl42_cursor_pos *)line->root)->row = line->prompt.root->row + row_diff;
	((rl42_cursor_pos *)line->root)->col = line->prompt.root->col + col_diff;
	return term_display_line(line, 0);
}

static inline u8	_dump_human(rl42_line *line) {
	const char	*tmp;
	size_t		i;
	i16			row_diff;
	i16			col_diff;

	tmp = term_get_seq(ti_ed);
	if (term_putsf("\n%s", (tmp) ? tmp : "") == -1)
		return 0;
	switch (rl42_get(RL42_BELL_STYLE).i64) {
		case RL42_BELL_NONE:
			tmp = "none";
			break ;
		case RL42_BELL_AUDIBLE:
			tmp = "audible";
			break ;
		case RL42_BELL_VISIBLE:
			tmp = "visible";
			break ;
		default:
			tmp = NULL;
	}
	if (tmp && term_putsf("bell-style is set to '%s'\n", tmp) == -1)
		return 0;
	switch (get_editing_mode()) {
		case EMACS:
			tmp = "emacs";
			break ;
		default:
			tmp = "vi";
	}
	if (term_putsf("editing-mode is set to '%s'\n", tmp) == -1)
		return 0;
	for (i = 0; i < _S_NUMERIC_SETTING_COUNT; i++)
		if (term_putsf("%s is set to '%ld'\n", s_numeric[i].name, rl42_get(s_numeric[i].code).i64) == -1)
			return 0;
	for (i = 0; i < _U_NUMERIC_SETTING_COUNT; i++)
		if (term_putsf("%s is set to '%lu'\n", u_numeric[i].name, rl42_get(u_numeric[i].code).u64) == -1)
			return 0;
	for (i = 0; i < _BOOLEAN_SETTING_COUNT; i++)
		if (term_putsf("%s is set to '%s'\n",boolean[i].name, (rl42_get(boolean[i].code).u64) ? "on" : "off") == -1)
			return 0;
	if (!term_flush_outbuf())
		return 0;
	row_diff = line->root->row - line->prompt.root->row;
	col_diff = line->root->col - line->prompt.root->col;
	term_cursor_get_pos((i16 *)&line->prompt.root->row, (i16 *)&line->prompt.root->col);
	((rl42_cursor_pos *)line->root)->row = line->prompt.root->row + row_diff;
	((rl42_cursor_pos *)line->root)->col = line->prompt.root->col + col_diff;
	return term_display_line(line, 0);
}
