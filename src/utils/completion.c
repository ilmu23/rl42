// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<completion.c>>

#include "data.h"
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
#include "internal/_darray.h"
#include "internal/_display.h"
#include "internal/_complete.h"

#ifndef __RL42_USE_EXTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

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
static inline size_t	_find_longest(cdarray completions);

// cmp_display
static inline const char	*_get_sgr0(void);

static inline size_t		_path_len(const char *path);
static inline u8			_is_path(const char *s);
static inline u8			_select_next(rl42_line *line, rl42_fn *next);
static inline u8			_query(rl42_line *line, const size_t completions);

// _complete_files
static inline stat_type	_get_file_type(const char *path);
static inline darray	_match_files(const char *pattern, DIR *dir);
static inline darray	_build_path(darray completions, const char *path);
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

cdarray	cmp_get_common(cdarray completions, const size_t pattern_len) {
	const char	*s1;
	const char	*tmp;
	size_t		count;
	size_t		len;
	size_t		i;
	size_t		j;

	if (completions && darray_size(completions) > 1) {
		count = darray_size(completions);
		s1 = *(const char **)darray_get(completions, 0);
		for (i = 0, len = _find_longest(completions); i < len; i++) {
			for (j = 1; j < count; j++) {
				tmp = *(const char **)darray_get(completions, j);
				if (s1[i] != tmp[i])
					break ;
			}
			if (j != count)
				break ;
		}
		if (i > 0 && i != pattern_len) {
			darray_resize((darray)completions, 1);
			((char *)s1)[i] = '\0';
		}
	}
	return completions;
}

u8	cmp_display(rl42_line *line, cdarray completions) {
	const char	*completion;
	rl42_fn		next;
	ssize_t		rv;
	darray		starts;
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
	i64			dwidth;
	u8			paging;
	u8			pathed;

	count = darray_size(completions);
	if ((i64)count >= rl42_get(RL42_COMPLETION_QUERY_ITEMS).i64 && !_query(line, count))
		return 1;
	pathed = _is_path(*(const char **)darray_get(completions, 0));
	starts = darray(size_t, count, NULL);
	for (i = widest = 0; i < count; i++) {
		completion = *(const char **)darray_get(completions, i);
		if (pathed) {
			darray_push(starts, (size_t){_path_len(completion)});
			completion = &completion[*(size_t *)darray_get(starts, i)];
		} else
			darray_push(starts, (size_t){0});
		len = strlen(completion);
		if (len > widest)
			widest = len;
	}
	cpp = 0;
	page = 0;
	cur = SIZE_MAX;
	term_show_cursor();
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
			rv = ti_tputs(term_get_seq(ti_ed), 1, term_putchar);
			if (rv == -1) {
				darray_delete(starts);
				return 0;
			}
			j = (size_t)rv;
			for (n = 0; i < count; i++) {
				completion = *(const char **)darray_get(completions, i);
				if (i != cur)
					rv = term_putsf("%-*s", (i32)widest, &completion[*(size_t *)darray_get(starts, i)]);
				else
					rv = term_putsf("%s%-*s%s", term_get_hl_seq(), (i32)widest, &completion[*(size_t *)darray_get(starts, i)], _get_sgr0());
				if (rv == -1) {
					darray_delete(starts);
					return 0;
				}
				j += (size_t)rv;
				if (paging && --cpp == 0)
					break ;
				if (++n == cpr) {
					if (--rows == 0)
						break ;
					if (term_putchar('\n') == -1) {
						darray_delete(starts);
						return 0;
					}
					n = 0;
				} else if (term_putchar(' ') == -1) {
					darray_delete(starts);
					return 0;
				}
			}
			term_cursor_set_pos(line->root->row + line->rows, 1);
			if (!term_flush_outbuf()) {
				darray_delete(starts);
				return 0;
			}
			term_cursor_move_to_i(line);
		}
		if (!_select_next(line, &next))
			break ;
		if (++cur == count)
			cur = 0;
		if (!cmp_insert(line, *(const char **)darray_get(completions, cur))) {
			darray_delete(starts);
			return 0;
		}
		add_mark(kill_end, line->i);
	}
	darray_delete(starts);
	return (next && term_display_line(line, DISPLAY_FORCE_SCREEN_CLEAR)) ? next(line) : 0;
}

u8	cmp_insert(rl42_line *line, const char *completion) {
	u32	ucp;

	if (!kill_region_internal(line))
		return 0;
	line->i = kill_start.pos;
	while (*completion) {
		ucp = utf8_decode(completion);
		if (!darray_insert(line->line, line->i++, ucp))
			return 0;
		completion += charsize_utf8(*completion);
	}
	return term_display_line(line, 0);
}

static inline size_t	_find_longest(cdarray completions) {
	size_t	longest;
	size_t	count;
	size_t	len;
	size_t	i;

	for (i = longest = 0, count = darray_size(completions); i < count; i++) {
		len = strlen(*(const char **)darray_get(completions, i));
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

static inline size_t	_path_len(const char *path) {
	const char	*slash;

	slash = strrchr(path, '/');
	if (slash[1] == '\0') do 
		slash--;
	while (*slash != '/');
	return (size_t)((uintptr_t)slash - (uintptr_t)path) + 1;
}

static inline u8	_is_path(const char *s) {
	const char	*first;
	const char	*last;

	first = strchr(s, '/');
	if (!first)
		return 0;
	last = strrchr(s, '/');
	return (first != last || last[1] != '\0') ? 1 : 0;
}

static inline u8	_select_next(rl42_line *line, rl42_fn *next) {
	rl42_fn_match	match;

	match.fn = NULL;
	darray_clear(line->keyseq);
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

	dummy.keyseq = darray(u32, 8, NULL);
	if (!dummy.keyseq)
		return 0;
	i = line->i;
	line->i = darray_size(line->line);
	if (!term_cursor_move_to_i(line) || !term_cursor_next_line()) {
		line->i = i;
		return 0;
	}
	line->i = i;
	if (term_putsf("rl42: display all %zu completions? ", completions) == -1 || !term_flush_outbuf())
		return 0;
	match.fn = NULL;
__query_match_seq:
	match = kb_match_seq(&dummy, match.fn, kb_listen((match.fn && match.fn->f) ? AMBIGUOUS_TIMEOUT : -1));
	if (match.fn && !match.run)
		goto __query_match_seq;
	darray_delete(dummy.keyseq);
	return (match.fn && match.fn->f == complete) ? 1 : 0;
}

static inline rl42_completion_fn(_complete_files) {
	const char	*tmp;
	const char	*path;
	darray		completions;
	u8			path_set;

	tmp = strrchr(pattern, '/');
	if (tmp) {
		path = (tmp != pattern) ? cstr_substr(pattern, 0, (uintptr_t)tmp - (uintptr_t)pattern) : strdup("/");
		if (!path)
			return NULL;
		path_set = 1;
		pattern = (const char *)((uintptr_t)tmp + 1);
	} else {
		path_set = 0;
		path = ".";
	}
	completions = _build_path(_match_files(pattern, opendir(path)), (path_set) ? path : NULL);
	if (path_set)
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

static inline darray	_match_files(const char *pattern, DIR *dir) {
	struct dirent	*data;
	const char		*tmp;
	cmp_type		type;
	darray			matches;
	size_t			pattern_len;
	u8				match_hidden;

	matches = darray(const char *, 16, free);
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
				if (!tmp || !darray_push(matches, tmp)) {
					darray_delete(matches);
					closedir(dir);
					return NULL;
				}
			}
		}
	}
	closedir(dir);
	return matches;
}

static inline darray	_build_path(darray completions, const char *path) {
	const char	*tmp;
	stat_type	type;
	size_t		i;
	size_t		count;
	u8			(*is_dir)(const char *);

	if (completions) {
		count = darray_size(completions);
		if (path) {
			if (path[strlen(path) - 1] == '/') for (i = 0; i < count; i++) {
				tmp = cstr_join(path, *(const char **)darray_get(completions, i));
				if (!tmp)
					goto __build_path_err;
				darray_replace(completions, i, tmp);
			} else for (i = 0; i < count; i++) {
				tmp = cstr_joins(path, *(const char **)darray_get(completions, i), '/');
				if (!tmp)
					goto __build_path_err;
				darray_replace(completions, i, tmp);
			}
		}
		switch (rl42_get(RL42_MARK_DIRECTORIES).u64 << 1 | rl42_get(RL42_VISIBLE_STATS).u64) {
			case rl42_conf_on:
			case rl42_conf_on << 1 | rl42_conf_on:
				for (i = 0; i < count; i++) {
					type = _get_file_type(*(const char **)darray_get(completions, i));
					if (type != ST_REGULAR) {
						tmp = cstr_join(*(const char **)darray_get(completions, i), stat_chars[type]);
						if (!tmp)
							goto __build_path_err;
						darray_replace(completions, i, tmp);
					}
				}
				break ;
			case rl42_conf_on << 1:
				is_dir = (rl42_get(RL42_MARK_SYMLINKED_DIRECTORIES).u64 == rl42_conf_on) ? _is_sldir : _is_dir;
				for (i = 0; i < count; i++) {
					if (is_dir(*(const char **)darray_get(completions, i))) {
						tmp = cstr_join(*(const char **)darray_get(completions, i), stat_chars[ST_DIR]);
						if (!tmp)
							goto __build_path_err;
						darray_replace(completions, i, tmp);
					}
				}
		}
	}
	return completions;
__build_path_err:
	darray_delete(completions);
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
	return (to_upper(c1) == to_upper(c2)) ? 1 : 0;
}

static inline u8	_cmp_map_case(const char c1, const char c2) {
	return (_cmp_ign_case(c1, c2) || (c1 == '-' && c2 == '_') || (c1 == '_' && c2 == '-')) ? 1 : 0;
}
