// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<completion.c>>

#include "data.h"
#include <ctype.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

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

#ifndef __DEBUG_PAGE_AFTER_ROWS
# define __DEBUG_PAGE_AFTER_ROWS	4
#endif

typedef enum {
	CT_NORMAL = 0,
	CT_IGN_CASE = 1,
	CT_MAP_CASE = 2
}	cmp_type;

typedef enum {
	ST_DIR = 0,
	ST_EXEC = 1,
	ST_SYMLINK = 2,
	ST_CHAR_DEV = 3,
	ST_BLOCK_DEV = 4,
	ST_SOCKET = 5,
	ST_FIFO = 6,
	ST_REGULAR
}	stat_type;

static inline rl42_completion_fn(_complete_files);

extern u16	term_height;
extern u16	term_width;

rl42_completion_fn	cmp_fn = _complete_files;
static const char	*stat_chars[ST_FIFO + 1] = { "/", "*", "@", "%", "#", "=", "|" };

// cmp_get_common
static inline size_t	_find_longest(cvector completions);

// cmp_display
static inline const char	*_get_sgr0(void);
static inline u8			_select_next(rl42_line *line, rl42_fn *next);
static inline u8			_query(rl42_line *line, const size_t completions);

// _complete_files
static inline stat_type	_get_file_type(const char *path);
static inline vector	_match_files(const char *pattern, DIR *dir);
static inline vector	_build_path(vector completions, const char *path);
static inline u8		_cmp_fname(const char *fname, const char *pattern, const size_t n, const cmp_type type);
static inline u8		_is_sldir(const char *path);
static inline u8		_is_dir(const char *path);

// comparison modes
static inline u8	_cmp(const char c1, const char c2);
static inline u8	_cmp_ign_case(const char c1, const char c2);
static inline u8	_cmp_map_case(const char c1, const char c2);

static u8	(*compare[3])(const char, const char) = { _cmp, _cmp_ign_case, _cmp_map_case };

void	set_completion_fn(rl42_completion_fn f) {
	cmp_fn = (f) ? f : _complete_files;
}

cvector	cmp_get_common(cvector completions, const size_t pattern_len) {
	const char	*s1;
	const char	*tmp;
	size_t		count;
	size_t		len;
	size_t		i;
	size_t		j;

	if (completions && vector_size(completions) > 1) {
		count = vector_size(completions);
		s1 = *(const char **)vector_get(completions, 0);
		for (i = 0, len = _find_longest(completions); i < len; i++) {
			for (j = 1; j < count; j++) {
				tmp = *(const char **)vector_get(completions, j);
				if (s1[i] != tmp[i])
					break ;
			}
			if (j != count)
				break ;
		}
		if (i > 0 && i != pattern_len) {
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
	size_t		cpp;
	size_t		rows;
	size_t		len;
	size_t		i;
	size_t		j;
	size_t		n;
	size_t		page;
	char		buf[_BUF_SIZE];
	i64			dwidth;
	u8			paging;

	count = vector_size(completions);
	if ((i64)count >= rl42_get(RL42_COMPLETION_QUERY_ITEMS).i64 && !_query(line, count))
		return 1;
	for (i = widest = 0; i < count; i++) {
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
		rows = (cpr > 1) ? count / cpr + 1 : count;
#ifdef __DEBUG_FORCE_CMP_PAGING
		if (rows >= __DEBUG_PAGE_AFTER_ROWS) {
			cpp = cpr * __DEBUG_PAGE_AFTER_ROWS;
			page = (cur != SIZE_MAX) ? cur / cpp : 0;
			paging = 1;
		}
#endif
		if (line->rows < term_height) {
			if (line->prompt.root->row + line->rows - 1 + rows > term_height) {
				scroll = line->root->row + line->rows + rows - term_height;
				if (scroll > line->rows) {
					term_scroll_display(line->prompt.root->row - 1, 0);
					if (rl42_get(RL42_PAGE_COMPLETIONS).u64 == rl42_conf_on) {
						cpp = cpr * (term_height - line->rows);
						page = (cur != SIZE_MAX) ? cur / cpp : 0;
						paging = 1;
					} else
						rows = term_height - line->rows;
				} else
					term_scroll_display(scroll, 0);
			}
			if (paging) for (i = j = n = 0; j < page; i++) {
				if (++n == cpp) {
					n = 0;
					j++;
				}
			} else
				i = 0;
			rv = snprintf(buf, _BUF_SIZE, "%s", term_get_seq(ti_ed));
			if (rv == -1)
				return 0;
			j = (size_t)rv;
			for (n = 0; i < count; i++) {
				completion = *(const char **)vector_get(completions, i);
				if (i != cur)
					rv = snprintf(&buf[j], _BUF_SIZE - j, "%-*s", (i32)widest, completion);
				else
					rv = snprintf(&buf[j], _BUF_SIZE - j, "%s%-*s%s", term_get_hl_seq(), (i32)widest, completion, _get_sgr0());
				if (rv == -1)
					return 0;
				j += (size_t)rv;
				if (paging && --cpp == 0)
					break ;
				if (++n == cpr) {
					if (--rows == 0)
						break ;
					buf[j++] = '\n';
					n = 0;
				} else
					buf[j++] = ' ';
			}
			buf[j] = '\0';
			term_cursor_set_pos(line->root->row + line->rows, 1);
			if (ti_tputs(buf, 1, __putchar) == -1)
				return 0;
			term_cursor_move_to_i(line);
		}
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

static inline size_t	_find_longest(cvector completions) {
	size_t	longest;
	size_t	count;
	size_t	len;
	size_t	i;

	for (i = longest = 0, count = vector_size(completions); i < count; i++) {
		len = strlen(*(const char **)vector_get(completions, i));
		if (len > longest)
			longest = len;
	}
	return longest;
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

static inline u8	_query(rl42_line *line, const size_t completions) {
	rl42_fn_match	match;
	rl42_line		dummy;
	size_t			i;
	char			buf[64];

	dummy.keyseq = vector(u32, 8, NULL);
	if (!dummy.keyseq)
		return 0;
	i = line->i;
	line->i = vector_size(line->line);
	if (!term_cursor_move_to_i(line) || !term_cursor_next_line()) {
		line->i = i;
		return 0;
	}
	line->i = i;
	if (snprintf(buf, 64, "rl42: display all %zu completions? ", completions) == -1)
		return 0;
	if (!ti_tputs(buf, 1, __putchar))
		return 0;
	match.fn = NULL;
__query_match_seq:
	match = kb_match_seq(&dummy, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
	if (match.fn && !match.run)
		goto __query_match_seq;
	vector_delete(dummy.keyseq);
	return (match.fn && match.fn->f == complete) ? 1 : 0;
}

static inline rl42_completion_fn(_complete_files) {
	const char	*tmp;
	const char	*path;
	vector		completions;

	tmp = strrchr(pattern, '/');
	if (tmp) {
		path = (tmp != pattern) ? cstr_substr(pattern, 0, (uintptr_t)tmp - (uintptr_t)pattern) : strdup("/");
		if (!path)
			return NULL;
		pattern = (const char *)((uintptr_t)tmp + 1);
	} else
		path = ".";
	completions = _build_path(_match_files(pattern, opendir(path)), path);
	if (!strl_equals(path, "."))
		free((void *)path);
	return completions;
}

static inline stat_type	_get_file_type(const char *path) {
	struct stat	file;

	if (lstat(path, &file) == -1)
		return ST_REGULAR;
	switch (file.st_mode & S_IFMT) {
		case S_IFDIR:
			return ST_DIR;
		case S_IFLNK:
			return ST_SYMLINK;
		case S_IFCHR:
			return ST_CHAR_DEV;
		case S_IFBLK:
			return ST_BLOCK_DEV;
		case S_IFSOCK:
			return ST_SOCKET;
		case S_IFIFO:
			return ST_FIFO;
	}
	return (access(path, X_OK) == 0) ? ST_EXEC : ST_REGULAR;
}

static inline vector	_match_files(const char *pattern, DIR *dir) {
	struct dirent	*data;
	const char		*tmp;
	cmp_type		type;
	vector			matches;
	size_t			pattern_len;
	u8				match_hidden;

	matches = vector(const char *, 16, free);
	if (matches) {
		if (!dir)
			return matches;
		type = (rl42_get(RL42_COMPLETION_IGNORE_CASE).u64) ? CT_IGN_CASE : CT_NORMAL;
		if (type == CT_IGN_CASE && rl42_get(RL42_COMPLETION_MAP_CASE).u64)
			type = CT_MAP_CASE;
		pattern_len = strlen(pattern);
		match_hidden = (rl42_get(RL42_MATCH_HIDDEN_FILES).u64 == rl42_conf_on) ? 1 : 0;
		for (data = readdir(dir); data; data = readdir(dir)) {
			if (strl_equals(data->d_name, ".") || strl_equals(data->d_name, "..") || (*data->d_name == '.' && !match_hidden && *pattern != '.'))
				continue ;
			if (_cmp_fname(data->d_name, pattern, pattern_len, type)) {
				tmp = strdup(data->d_name);
				if (!tmp || !vector_push(matches, tmp)) {
					vector_delete(matches);
					closedir(dir);
					return NULL;
				}
			}
		}
	}
	closedir(dir);
	return matches;
}

static inline vector	_build_path(vector completions, const char *path) {
	const char	*tmp;
	stat_type	type;
	size_t		i;
	size_t		count;
	u8			(*is_dir)(const char *);

	if (completions) {
		count = vector_size(completions);
		if (!strl_equals(path, ".")) {
			if (path[strlen(path) - 1] == '/') for (i = 0; i < count; i++) {
				tmp = cstr_join(path, *(const char **)vector_get(completions, i));
				if (!tmp)
					goto __build_path_err;
				vector_replace(completions, i, tmp);
			} else for (i = 0; i < count; i++) {
				tmp = cstr_joins(path, *(const char **)vector_get(completions, i), '/');
				if (!tmp)
					goto __build_path_err;
				vector_replace(completions, i, tmp);
			}
		}
		switch (rl42_get(RL42_MARK_DIRECTORIES).u64 << 1 | rl42_get(RL42_VISIBLE_STATS).u64) {
			case rl42_conf_on:
			case rl42_conf_on << 1 | rl42_conf_on:
				for (i = 0; i < count; i++) {
					type = _get_file_type(*(const char **)vector_get(completions, i));
					if (type != ST_REGULAR) {
						tmp = cstr_join(*(const char **)vector_get(completions, i), stat_chars[type]);
						if (!tmp)
							goto __build_path_err;
						vector_replace(completions, i, tmp);
					}
				}
				break ;
			case rl42_conf_on << 1:
				is_dir = (rl42_get(RL42_MARK_SYMLINKED_DIRECTORIES).u64 == rl42_conf_on) ? _is_sldir : _is_dir;
				for (i = 0; i < count; i++) {
					if (is_dir(*(const char **)vector_get(completions, i))) {
						tmp = cstr_join(*(const char **)vector_get(completions, i), stat_chars[ST_DIR]);
						if (!tmp)
							goto __build_path_err;
						vector_replace(completions, i, tmp);
					}
				}
		}
	}
	return completions;
__build_path_err:
	vector_delete(completions);
	return NULL;
}

static inline u8	_cmp_fname(const char *fname, const char *pattern, const size_t n, const cmp_type type) {
	size_t	i;

	for (i = 0; i < n; i++)
		if (!compare[type](fname[i], pattern[i]))
			return 0;
	return 1;
}

static inline u8		_is_sldir(const char *path) {
	struct stat	file;

	if (stat(path, &file) == -1)
		return 0;
	return (S_ISDIR(file.st_mode)) ? 1 : 0;
}

static inline u8		_is_dir(const char *path) {
	struct stat	file;

	if (lstat(path, &file) == -1)
		return 0;
	return (S_ISDIR(file.st_mode)) ? 1 : 0;
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
