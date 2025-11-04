// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<completion.c>>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define __RL42_INTERNAL
#include "rl42.h"
#include "complete.h"

#include "internal/_kb.h"
#include "internal/_defs.h"
#include "internal/_kill.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_complete.h"
#include "internal/_terminfo.h"

#include "internal/fn/text.h"

#define _BUF_SIZE	16384

static rl42_completion_fn(_complete_files);

extern u16	term_height;
extern u16	term_width;

rl42_completion_fn	cmp_fn = _complete_files;

static inline const char	*_get_sgr0(void);
static inline u8			_select_next(rl42_line *line, rl42_fn *next);

static inline u8	_cmp(const char c1, const char c2);
static inline u8	_cmp_ign_case(const char c1, const char c2);
static inline u8	_cmp_map_case(const char c1, const char c2);

static u8	(*compare[3])(const char, const char) = { _cmp, _cmp_ign_case, _cmp_map_case };

void	set_completion_fn(rl42_completion_fn f) {
	cmp_fn = (f) ? f : _complete_files;
}

cvector	cmp_get_common(cvector completions) {
	enum {
		NORMAL = 0,
		IGN_CASE = 1,
		MAP_CASE = 2
	}			cmp_type;
	const char	*s1;
	const char	*tmp;
	size_t		count;
	size_t		len;
	size_t		i;
	size_t		j;

	if (completions && vector_size(completions) > 1) {
		count = vector_size(completions);
		s1 = *(const char **)vector_get(completions, 0);
		cmp_type = (rl42_get(RL42_COMPLETION_IGNORE_CASE).u64) ? IGN_CASE : NORMAL;
		if (cmp_type == IGN_CASE && rl42_get(RL42_COMPLETION_MAP_CASE).u64)
			cmp_type = MAP_CASE;
		for (i = 0, len = strlen(s1); i < len; i++) {
			for (j = 1; j < count; j++) {
				tmp = *(const char **)vector_get(completions, j);
				if (!compare[cmp_type](s1[i], tmp[i]))
					break ;
			}
			if (j != count)
				break ;
		}
		if (i > 0) {
			vector_resize((vector)completions, 1);
			((char *)s1)[i] = '\0';
		}
	}
	return completions;
}

u8	cmp_display(rl42_line *line, cvector completions) {
	const char	*completion;
	rl42_fn		next;
	ssize_t		rv;
	size_t		widest;
	size_t		scroll;
	size_t		count;
	size_t		cur;
	size_t		cpr;
	size_t		rows;
	size_t		len;
	size_t		i;
	size_t		j;
	size_t		n;
	char		buf[_BUF_SIZE];
	i64			dwidth;

	for (i = widest = 0, count = vector_size(completions); i < count; i++) {
		len = strlen(*(const char **)vector_get(completions, i));
		if (len > widest)
			widest = len;
	}
	cur = SIZE_MAX;
	while (1) {
		dwidth = rl42_get(RL42_COMPLETION_DISPLAY_WIDTH).i64;
		if (dwidth == -1 || dwidth > term_width)
			dwidth = term_width;
		cpr = max(dwidth / (widest + 1), 1);
		rows = count / cpr + 1;
		if (line->root->row + line->rows + rows > term_height) {
			scroll = line->root->row + line->rows + rows - term_height;
			if (scroll > term_height - line->rows)
				return 1; // TODO: page completions
			term_scroll_display(scroll, 0);
		}
		for (i = j = n = 0; i < count; i++) {
			completion = *(const char **)vector_get(completions, i);
			if (i != cur)
				rv = snprintf(&buf[j], _BUF_SIZE - j, "%-*s", (i32)widest, completion);
			else
				rv = snprintf(&buf[j], _BUF_SIZE - j, "%s%-*s%s", term_get_hl_seq(), (i32)widest, completion, _get_sgr0());
			if (rv == -1)
				return 0;
			j += (size_t)rv;
			if (++n == cpr) {
				buf[j++] = '\n';
				n = 0;
			} else
				buf[j++] = ' ';
		}
		term_cursor_set_pos(line->root->row + line->rows, 1);
		if (ti_tputs(buf, 1, __putchar) == -1)
			return 0;
		term_cursor_move_to_i(line);
		if (!_select_next(line, &next))
			break ;
		if (++cur == count)
			cur = 0;
		if (!cmp_insert(line, *(const char **)vector_get(completions, cur)))
			return 0;
		add_mark(kill_end, line->i);
	}
	return (next && term_display_line(line, 0)) ? next(line) : 0;
}

u8	cmp_insert(rl42_line *line, const char *completion) {
	u32	ucp;

	if (!kill_region_internal(line))
		return 0;
	line->i = kill_start.pos;
	while (*completion) {
		ucp = utf8_decode(completion);
		if (!vector_insert(line->line, line->i++, ucp))
			return 0;
		completion += charsize_utf8(*completion);
	}
	return term_display_line(line, 0);
}

static inline const char	*_get_sgr0(void) {
	const char	*seq;

	seq = term_get_seq(ti_sgr0);
	if (seq == TI_ABS_STR)
		seq = "\x1b[m";
	return seq;
}

static inline u8	_select_next(rl42_line *line, rl42_fn *next) {
	rl42_fn_match	match;

	match.fn = NULL;
	vector_clear(line->keyseq);
	do
		match = kb_match_seq(line, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
	while (!match.fn || !match.run);
	*next = match.fn->f;
	return (match.fn->f == complete) ? 1 : 0;
}


static inline u8	_cmp(const char c1, const char c2) {
	return (c1 == c2) ? 1 : 0;
}

static inline u8	_cmp_ign_case(const char c1, const char c2) {
	return (toupper(c1) == toupper(c2)) ? 1 : 0;
}

static inline u8	_cmp_map_case(const char c1, const char c2) {
	return (_cmp_ign_case(c1, c2) || (c1 == '-' && c2 == '_') || (c1 == '_' && c2 == '-')) ? 1 : 0;
}

#include <stdlib.h>
#include "internal/test/defs.h"

static rl42_completion_fn(_complete_files) {
	vector	out;

	(void)pattern;
	out = vector(const char *, 3, free);
	if (out) switch (rand_range(0, 2)) {
		case 0:
			if (!vector_push(out, (const char *){strdup("ayy")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("ayylmao")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("ayy lmao")}))
				return NULL;
			break ;
		case 1:
			if (!vector_push(out, (const char *){strdup("ayy lmao")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("lolmao")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("lul")}))
				return NULL;
			break ;
		case 2:
			if (!vector_push(out, (const char *){strdup("lul")}))
				return NULL;
	}
	return out;
}
