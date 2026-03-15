// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<history.c>>

#if __STDC_VERSION__ < 202311L
# define _GNU_SOURCE
# include <bsd/string.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

#include "internal/_kb.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_display.h"
#include "internal/_history.h"
#include "internal/_keybinds.h"

#include "internal/fn/move.h"
#include "internal/fn/text.h"

#define _HFNAME_BUF_SIZE	PATH_MAX + 1

#define _DEFAULT_HIST_FILE	".rl42_history"

#define _SEARCH_PROMPT_FWD	"inc-fwd-search: "
#define _SEARCH_PROMPT_BCK	"inc-bck-search: "

#define hist_node(n, l, e)	((rl42_hist_node){.entry_n = n, .line = l, .edit = e, .new = load_done})

#define search_fn_is_allowed(f)	(f == self_insert || f == backward_char || f == forward_char || f == delete_char || f == backward_delete_char)

typedef struct {
	const rl42_hist_node	*node;
	size_t					i;
}	_match;

extern rl42_hist_node	*current;

extern rl42_fn	prev_fn;

extern u16	term_height;

static const char	*search_prompts[2][2] = {
	{ &_SEARCH_PROMPT_FWD[4], _SEARCH_PROMPT_FWD },
	{ &_SEARCH_PROMPT_BCK[4], _SEARCH_PROMPT_BCK }
};

static char	histfile_name[_HFNAME_BUF_SIZE];

static size_t	entries;
static list		history;
static u8		load_done;

static inline _match	_search_get_match(const char *match_str, const rl42_direction direction);
static inline u8		_search_process_query(rl42_line *line, cdarray query, const rl42_hist_node **match, const rl42_direction direction);
static inline u8		_search_get_query(rl42_line *query, rl42_fn *fn, const u8 incremental);

static void	_free_hist_node(rl42_hist_node *node);

rl42_hist_node	*hist_get_next_node(const rl42_hist_node *node, const rl42_direction direction) {
	list_node	lnode;

	switch (direction) {
		case FORWARD:
			lnode = (node->entry_n != list_size(history)) ? list_nth(history, entries - (node->entry_n + 1)) : NULL;
			break ;
		case BACKWARD:
			lnode = (node->entry_n != 0) ? list_nth(history, entries - (node->entry_n - 1)) : NULL;
			break ;
		default:
			lnode = NULL;
	}
	return (lnode) ? (rl42_hist_node *)lnode->data : (rl42_hist_node *)node;
}

rl42_hist_node	*hist_get_nth_node(i64 n) {
	rl42_hist_node	*out;
	rl42_direction	direction;
	i64				i;
	u8				neg;

	if (n < 0) {
		neg = 1;
		n = -n;
	} else
		neg = 0;
	if (n < (i64)entries / 2) {
		i = 1;
		direction = (!neg) ? BACKWARD : FORWARD;
		out = (!neg) ? hist_get_first_node() : hist_get_last_node();
		while (i++ < n)
			out = hist_get_next_node(out, direction);
	} else {
		i = (i64)entries;
		direction = (!neg) ? FORWARD : BACKWARD;
		out = (!neg) ? hist_get_last_node() : hist_get_first_node();
		while (i-- > n)
			out = hist_get_next_node(out, direction);
	}
	return out;
}

rl42_hist_node	*hist_get_first_node(void) {
	return (entries) ? (rl42_hist_node *)list_first(history)->data : NULL;
}

rl42_hist_node	*hist_get_last_node(void) {
	return (entries) ? (rl42_hist_node *)list_last(history)->data : NULL;
}

u8	hist_search(rl42_line *line, const rl42_direction direction, const u8 incremental) {
	const rl42_hist_node	*match;
	const char				*match_str;
	rl42_line				query;
	rl42_fn					fn;
	size_t					old_i;
	u8						vi_cmd_mode;
	u8						rv;

	query = (rl42_line){
		.prompt.prompt = cstr_to_rl42str(search_prompts[direction][incremental]),
		.keyseq = darray(u32, 8, NULL),
		.line = darray(u32, 16, NULL),
		.i = 0,
	};
	match = NULL;
	match_str = NULL;
	vi_cmd_mode = (get_editing_mode() == RL42_EM_VI_CMD) ? 1 : 0;
	if (vi_cmd_mode)
		set_editing_mode(RL42_EM_VI_INS);
	if (current->edit)
		free((void *)current->edit);
	current->edit = rl42str_to_cstr(line->line);
	if (!current->edit)
		goto _hist_search_error;
	if (!query.prompt.prompt || !query.keyseq || !query.line)
		goto _hist_search_error;
	old_i = line->i;
	line->i = darray_size(line->line);
	term_cursor_move_to_i(line);
	line->i = old_i;
	term_cursor_next_line();
	query.prompt.root = term_cursor_new_anchor();
	if (!query.prompt.root)
		goto _hist_search_error;
	term_display_line(&query, DISPLAY_PROMPT_ONLY);
	query.root = term_cursor_new_anchor();
	if (!query.root)
		goto _hist_search_error;
	if (incremental) do {
		if (darray_size(query.line) && !_search_process_query(line, query.line, &match, direction))
			goto _hist_search_error;
		if (!term_display_line(line, DISPLAY_HIGHLIGHT_SUBSTR | ((rl42_get(RL42_SEARCH_IGNORE_CASE).u64) ? DISPLAY_HIGHLIGHT_IGNORE_CASE : 0), query.line))
			goto _hist_search_error;
		((rl42_cursor_pos *)query.prompt.root)->row = line->root->row + line->rows;
		((rl42_cursor_pos *)query.root)->row = line->root->row + line->rows;
		if (query.root->row > term_height)
			term_scroll_display(1, 0);
		rv = _search_get_query(&query, &fn, incremental);
	} while (rv == 1); else {
		rv = _search_get_query(&query, &fn, incremental);
		if (darray_size(query.line) && !_search_process_query(line, query.line, &match, direction))
			goto _hist_search_error;
	}
	if (!rv)
		goto _hist_search_error;
	if (match)
		current = (rl42_hist_node *)match;
	if (!term_display_line(line, DISPLAY_HIGHLIGHT_SUBSTR, query.line))
		goto _hist_search_error;
	term_cursor_delete_anchor(query.prompt.root);
	term_cursor_delete_anchor(query.root);
	darray_delete(query.prompt.prompt);
	darray_delete(query.keyseq);
	darray_delete(query.line);
	free((void *)match_str);
	if (incremental) {
		rv = fn(line);
		prev_fn = fn;
	}
	if (vi_cmd_mode)
		set_editing_mode(RL42_EM_VI_CMD);
	return (incremental) ? rv : 1;
_hist_search_error:
	term_cursor_delete_anchor(query.prompt.root);
	term_cursor_delete_anchor(query.root);
	darray_delete(query.prompt.prompt);
	darray_delete(query.keyseq);
	darray_delete(query.line);
	free((void *)match_str);
	if (vi_cmd_mode)
		set_editing_mode(RL42_EM_VI_CMD);
	return 0;
}

u8	hist_yank_arg(rl42_line *line, const rl42_hist_node *node, const i64 n) {
	cdarray	args;
	cdarray	word;
	size_t	word_i;
	size_t	len;
	u8		rv;

	if (n == 0)
		return 1;
	args = cstr_split((node->edit) ? node->edit : node->line, ' ', "'\"");
	if (!args)
		return 0;
	rv = 0;
	word_i = (n > 0) ? min((size_t)n, darray_size(args)) : (size_t)max(1, (i64)darray_size(args) + 1 - -n);
	word = cstr_to_rl42str(*(char **)darray_get(args, word_i - 1));
	if (!word)
		goto _hist_yank_arg_ret;
	len = darray_size(word);
	if (!darray_insert_n(line->line, line->i, len, darray_start(word)))
		goto _hist_yank_arg_ret;
	line->i += len;
	rv = 1;
_hist_yank_arg_ret:
	darray_delete((darray)word);
	darray_delete((darray)args);
	return rv;
}

void	hist_remove_extra_nodes(void) {
	rl42_hist_node	*node;
	rl42_hist_node	*tmp;
	size_t			i;
	FILE			*file;
	i64				max_size;

	max_size = rl42_get(RL42_HISTORY_SIZE).i64;
	if (max_size++ == -1 || entries <= (size_t)max_size)
		return ;
	for (i = 0, node = hist_get_last_node(); i < entries - (size_t)max_size; i++) {
		if (node->new)
			break ;
		node = hist_get_next_node(node, FORWARD);
	}
	if (i < entries - (size_t)max_size) {
		i = entries - (size_t)max_size;
		file = fopen(histfile_name, "a");
		while (i--) {
			fprintf(file, "%s\n", node->line);
			node = hist_get_next_node(node, FORWARD);
		}
		fclose(file);
	}
	for (i = 0, node = hist_get_first_node(); i < (size_t)max_size; i++) {
		tmp = node;
		node = hist_get_next_node(node, BACKWARD);
		tmp->entry_n = max_size - i;
	}
	list_resize(history, max_size);
	entries = (size_t)max_size;
}

void	hist_remove_node(rl42_hist_node *node) {
	list_erase(history, list_nth(history, entries-- - node->entry_n));
}

u8	hist_add_line(const char *line) {
	if (!list_push_front(history, hist_node(++entries, line, NULL)))
		return 0;
	hist_remove_extra_nodes();
	return 1;
}

u8	hist_load(const char *fname) {
	ssize_t	read;
	size_t	length;
	FILE	*file;
	char	*line;
	u8		rv;

	if (history)
		return 1;
	history = list(rl42_hist_node, 500, (void (*)(void *))_free_hist_node);
	if (!history)
		return 0;
	if (!fname)
		snprintf(histfile_name, _HFNAME_BUF_SIZE, "%s/" _DEFAULT_HIST_FILE, rl42_getenv("HOME"));
	else
		strlcpy(histfile_name, fname, sizeof(histfile_name));
	file = fopen(histfile_name, "r");
	if (!file)
		return 0;
	line = NULL;
	length = 0;
	entries = 0;
	load_done = 0;
	for (rv = 1, read = getdelim(&line, &length, '\xff', file); rv && read != -1; read = getdelim(&line, &length, '\xff', file)) {
		line[read - 1] = '\0';
		if (!hist_add_line(line) || fseek(file, 1, SEEK_CUR) == -1)
			rv = 0;
		line = NULL;
	}
	if (!feof(file))
		rv = 0;
	free(line);
	load_done = 1;
	fclose(file);
	return rv;
}

void	hist_clean(void) {
	rl42_hist_node	*node;
	rl42_hist_node	*prev;
	FILE			*file;

	if (!history)
		return ;
	if (rl42_in_child_process)
		goto _hist_clean_delete_history;
	node = hist_get_first_node();
	if (node && node->new) {
		file = fopen(histfile_name, "a");
		if (!file)
			goto _hist_clean_delete_history;
		for (node = hist_get_last_node(), prev = NULL; node != prev; prev = node, node = hist_get_next_node(node, FORWARD))
			if (node->new)
				break ;
		do {
			prev = node;
			node = hist_get_next_node(node, FORWARD);
			fprintf(file, "%s%c\n", prev->line, '\xff');
		} while (prev != node);
		fclose(file);
	}
_hist_clean_delete_history:
	list_delete(history);
	history = NULL;
	load_done = 0;
	entries = 0;
}

static inline _match _search_get_match(const char *match_str, const rl42_direction direction) {
	const rl42_hist_node	*prev;
	const rl42_hist_node	*cur;
	const char				*start;
	char					*(*cmp_fn)(const char *, const char *);

	cmp_fn = (rl42_get(RL42_SEARCH_IGNORE_CASE).u64 == 0) ? strstr : strcasestr;
	for (cur = current, prev = NULL; cur != prev; prev = cur, cur = hist_get_next_node(cur, direction)) {
		start = cmp_fn((cur->edit) ? cur->edit : cur->line, match_str);
		if (start)
			return (_match){ .node = cur, .i = (size_t)((uintptr_t)start - ((cur->edit) ? (uintptr_t)cur->edit: (uintptr_t)cur->line)) };
	}
	return (_match){ .node = NULL, .i = 0 };
}

static inline u8	_search_process_query(rl42_line *line, cdarray query, const rl42_hist_node **match, const rl42_direction direction) {
	const char	*query_str;
	_match		_match;
	u8			rv;

	query_str = rl42str_to_cstr(query);
	if (!query_str)
		return 0;
	rv = 0;
	_match = _search_get_match(query_str, direction);
	(*match) = _match.node;
	if (*match) {
		darray_delete(line->line);
		line->line = cstr_to_rl42str(((*match)->edit) ? (*match)->edit : (*match)->line);
		if (!line->line)
			goto _search_process_query_ret;
		line->i = (!rl42_get(RL42_HORIZONTAL_SCROLL_MODE).u64) ? darray_size(line->line) : _match.i;
		rv = 1;
	} else
		rv = 1;
_search_process_query_ret:
	free((void *)query_str);
	return rv;
}

static inline u8	_search_get_query(rl42_line *query, rl42_fn *fn, const u8 incremental) {
	rl42_fn_match	match;
	u8				rv;

	rv = 1;
	match.fn = NULL;
	term_display_line(query, 0);
	do {
		match = kb_match_seq(query, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
		if (match.fn && match.run) {
			*fn = match.fn->f;
			if (search_fn_is_allowed(*fn)) {
				rv = (*fn)(query);
				prev_fn = *fn;
			} else
				rv = 2;
			darray_clear(query->keyseq);
			if (incremental)
				break ;
			match.fn = NULL;
		}
	} while (rv == 1);
	return rv;
}

static void	_free_hist_node(rl42_hist_node *node) {
	free((void *)node->line);
	free((void *)node->edit);
}
