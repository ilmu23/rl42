// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<history.c>>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "internal/_kb.h"
#include "internal/_list.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_history.h"
#include "internal/_terminfo.h"

#include "internal/fn/move.h"
#include "internal/fn/text.h"

#define _DEFAULT_HIST_FILE	".rl42_history"

#define _SEARCH_PROMPT_FWD	"inc-fwd-search: "
#define _SEARCH_PROMPT_BCK	"inc-bck-search: "

#define hist_node(n, l, e)	((rl42_hist_node){.entry_n = n, .line = l, .edit = e})

#define search_fn_is_allowed(f)	(f == self_insert || f == backward_char || f == forward_char || f == delete_char || f == backward_delete_char)

extern rl42_hist_node	*current;
extern rl42_fn			prev_fn;

static const char	*search_prompts[2][2] = {
	{ &_SEARCH_PROMPT_FWD[4], _SEARCH_PROMPT_FWD },
	{ &_SEARCH_PROMPT_BCK[4], _SEARCH_PROMPT_BCK }
};

static const char	*histfile_name;

static size_t	first_new;
static size_t	entries;
static list		history;

static inline const rl42_hist_node	*_search_get_match(const char *match_str, const rl42_direction direction);
static inline u8					_search_process_query(rl42_line *line, cvector query, const rl42_hist_node **match, const rl42_direction direction);
static inline u8					_search_get_query(rl42_line *query, rl42_fn *fn, const u8 incremental);

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
	return (rl42_hist_node *)list_first(history)->data;
}

rl42_hist_node	*hist_get_last_node(void) {
	return (rl42_hist_node *)list_last(history)->data;
}

u8	hist_search(rl42_line *line, const rl42_direction direction, const u8 incremental) {
	const rl42_hist_node	*match;
	const char				*match_str;
	rl42_line				query;
	rl42_fn					fn;
	size_t					old_i;
	u8						rv;

	query = (rl42_line){
		.prompt.prompt = cstr_to_rl42str(search_prompts[direction][incremental]),
		.keyseq = vector(u32, 8, NULL),
		.line = vector(u32, 16, NULL),
		.i = 0,
	};
	match = NULL;
	match_str = NULL;
	if (current->edit)
		free((void *)current->edit);
	current->edit = rl42str_to_cstr(line->line);
	if (!current->edit)
		goto _hist_search_error;
	if (!query.prompt.prompt || !query.keyseq || !query.line)
		goto _hist_search_error;
	old_i = line->i;
	line->i = vector_size(line->line);
	term_cursor_move_to_i(line);
	line->i = old_i;
	ti_tputs("\n", 1, __putchar);
	term_cursor_get_pos(&query.prompt.root.row, &query.prompt.root.col);
	term_display_line(&query, DISPLAY_PROMPT_ONLY);
	term_cursor_get_pos(&query.root.row, &query.root.col);
	if (incremental) do {
		if (vector_size(query.line) && !_search_process_query(line, query.line, &match, direction))
			goto _hist_search_error;
		if (!term_display_line(line, DISPLAY_HIGHLIGHT_SUBSTR, query.line))
			goto _hist_search_error;
		rv = _search_get_query(&query, &fn, incremental);
	} while (rv == 1); else {
		rv = _search_get_query(&query, &fn, incremental);
		if (vector_size(query.line) && !_search_process_query(line, query.line, &match, direction))
			goto _hist_search_error;
	}
	if (!rv)
		goto _hist_search_error;
	if (match)
		current = (rl42_hist_node *)match;
	if (!term_display_line(line, DISPLAY_HIGHLIGHT_SUBSTR, query.line))
		goto _hist_search_error;
	vector_delete(query.prompt.prompt);
	vector_delete(query.keyseq);
	vector_delete(query.line);
	free((void *)match_str);
	if (incremental) {
		rv = fn(line);
		prev_fn = fn;
	}
	return (incremental) ? rv : 1;
_hist_search_error:
	vector_delete(query.prompt.prompt);
	vector_delete(query.keyseq);
	vector_delete(query.line);
	free((void *)match_str);
	return 0;
}

u8	hist_yank_arg(rl42_line *line, const rl42_hist_node *node, const i64 n) {
	cvector	args;
	cvector	word;
	size_t	word_i;
	size_t	len;
	size_t	i;
	u8		rv;

	if (n == 0)
		return 1;
	args = cstr_split((node->edit) ? node->edit : node->line, ' ');
	if (!args)
		return 0;
	rv = 0;
	word_i = (n > 0) ? min((size_t)n, vector_size(args)) : (size_t)max(0, (i64)vector_size(args) - 1 - -n);
	word = cstr_to_rl42str(*(char **)vector_get(args, word_i - 1));
	if (!word)
		goto _hist_yank_arg_ret;
	for (i = 0, len = vector_size(word); i < len; i++)
		if (!__vec_ins(line->line, line->i++, vector_get(word, i)))
			goto _hist_yank_arg_ret;
	rv = 1;
_hist_yank_arg_ret:
	vector_delete((vector)word);
	vector_delete((vector)args);
	return rv;
}

void	hist_remove_node(rl42_hist_node *node) {
	list_erase(history, list_nth(history, entries-- - node->entry_n));
}

u8	hist_add_line(const char *line) {
	return list_push_front(history, hist_node(++entries, line, NULL));
}

u8	hist_load(const char *fname) {
	FILE	*file;
	char	*line;
	char	buf[4096];
	u8		rv;

	if (history)
		return 1;
	history = list(rl42_hist_node, 500, (void (*)(void *))_free_hist_node);
	if (!history)
		return 0;
	if (!fname) {
		snprintf(buf, 4096, "%s/" _DEFAULT_HIST_FILE, getenv("HOME"));
	} else
		snprintf(buf, 4096, "%s", fname);
	file = fopen(buf, "r");
	if (!file)
		return 0;
	histfile_name = strdup(buf);
	for (rv = 1, line = fgets(buf, 4096, file); rv && line; line = fgets(buf, 4096, file))
		if (!hist_add_line(strndup(line, strlen(line) - 1)))
			rv = 0;
	fclose(file);
	first_new = entries;
	return rv;
}

void	hist_clean(void) {
	rl42_hist_node	*node;
	FILE			*file;

	if (first_new != entries) {
		file = fopen(histfile_name, "a");
		for (node = hist_get_first_node(); node->entry_n > first_new; node = hist_get_next_node(node, BACKWARD))
			;
		do {
			node = hist_get_next_node(node, FORWARD);
			fprintf(file, "%s\n", node->line);
		} while (node->entry_n != entries);
		fclose(file);
	}
	free((void *)histfile_name);
	list_delete(history);
}

static inline const rl42_hist_node	*_search_get_match(const char *match_str, const rl42_direction direction) {
	const rl42_hist_node	*prev;
	const rl42_hist_node	*cur;

	// TODO: Check if search-ignore-case is set
	for (cur = current, prev = NULL; cur != prev; prev = cur, cur = hist_get_next_node(cur, direction))
		if (strstr((cur->edit) ? cur->edit : cur->line, match_str))
			break ;
	return (cur != prev) ? cur : NULL;
}

static inline u8	_search_process_query(rl42_line *line, cvector query, const rl42_hist_node **match, const rl42_direction direction) {
	const char	*query_str;
	u8			rv;

	query_str = rl42str_to_cstr(query);
	if (!query_str)
		return 0;
	rv = 0;
	(*match) = _search_get_match(query_str, direction);
	if (*match) {
		vector_delete(line->line);
		line->line = cstr_to_rl42str(((*match)->edit) ? (*match)->edit : (*match)->line);
		if (!line->line)
			goto _search_process_query_ret;
		line->i = vector_size(line->line);
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
			vector_clear(query->keyseq);
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
