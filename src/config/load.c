// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<load.c>>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rl42.h"

#include "internal/_defs.h"
#include "internal/_utils.h"
#include "internal/_vector.h"

#define _DEFAULT_CFG_FILE	".rl42rc"

#define _BELL_STYLE_HASH					0xC586B53AEF5ECCBBU
#define _COLORED_COMPLETION_PREFIX_HASH		0x1022B137EBD1599CU
#define _COMPLETION_DISPLAY_WIDTH_HASH		0xF5AEE7FFC47A60C2U
#define _COMPLETION_IGNORE_CASE_HASH		0x38D58D56383FE6DEU
#define _COMPLETION_MAP_CASE_HASH			0x7A203DAB82820392U
#define _COMPLETION_QUERY_ITEMS_HASH		0xAADD4F348B5E2646U
#define _DISABLE_COMPLETION_HASH			0x7EFA4A655259DD95U
#define _EDITING_MODE_HASH					0x486190232D1C68DCU
#define _ENABLE_BRACKETED_PASTE_HASH		0x43E712EBBFE5CE43U
#define _ENABLE_HIGHLIGHT_HASH				0x4CC594A77A12B3A6U
#define _EXPAND_TILDE_HASH					0x86B3842050B80119U
#define _HISTORY_SIZE_HASH					0xB855D24FC9911D4AU
#define _HORIZONTAL_SCROLL_MODE_HASH		0x6A112BE62B74E0FEU
#define _KEYSEQ_TIMEOUT_HASH				0x1F6B9E0F52408628U
#define _MARK_DIRECTORIES_HASH				0xF4094B976F01307FU
#define _MARK_SYMLINKED_DIRECTORIES_HASH	0x4254D0CADBED6436U
#define _MATCH_HIDDEN_FILES_HASH			0x27615217B8D36228U
#define _PAGE_COMPLETIONS_HASH				0xB731159D0F129F6DU
#define _SEARCH_IGNORE_CASE_HASH			0x6B3D10CD7D0E3762U
#define _VISIBLE_STATS_HASH					0x11CE0D87591B49A8U
#define _HIGHLIGHT_COLOR_HASH				0xB32F3587B1CA2F3EU

static inline rl42_editing_mode	_parse_mode(const char *mode);
static inline rl42_setting		_parse_var(const char *var);
static inline const char		*_convert_spaces(char *s);
static inline u8				_parse_line(vector lines, cvector line, const size_t line_n);
static inline u8				_exec_lines(cvector lines);
static inline u8				_is_int(const char *s);
static inline u8				_is_uint(const char *s);
static inline u8				_is_rgb(const char *s);

static void	_free_cfg_line(rl42_cfg_line *line);

u8	rl42_load_config(const char *fname) {
	vector	lines;
	size_t	line_n;
	FILE	*file;
	char	*line;
	char	buf[4096];
	u8		rv;

	lines = vector(rl42_cfg_line, 32, (void (*)(void *))_free_cfg_line);
	if (!lines)
		return 0;
	rv = 0;
	if (!fname)
		snprintf(buf, 4096, "%s/" _DEFAULT_CFG_FILE, getenv("HOME"));
	else
		snprintf(buf, 4096, "%s", fname);
	file = fopen(buf, "r");
	if (!file)
		goto _cfg_load_del_lines;
	for (line_n = 1, line = fgets(buf, 4096, file); line; line_n++, line = fgets(buf, 4096, file))
		if (strlen(line) > 1 && !_parse_line(lines, cstr_split(_convert_spaces(line), ' ', "'\""), line_n))
			goto _cfg_load_close_file;
	rv = _exec_lines(lines);
_cfg_load_close_file:
	fclose(file);
_cfg_load_del_lines:
	vector_delete(lines);
	return rv;
}

static inline rl42_editing_mode	_parse_mode(const char *mode) {
	if (strcasecmp(mode, "emacs") == 0)
		return EMACS;
	if (strcasecmp(mode, "vi-cmd") == 0)
		return VI_CMD;
	if (strcasecmp(mode, "vi-ins") == 0)
		return VI_INS;
	return CURRENT;
}

static inline rl42_setting		_parse_var(const char *var) {
	u64	hash;

	hash = cstr_hash(var, 347, UINT64_MAX);
	switch (hash) {
		case _BELL_STYLE_HASH:
			return RL42_BELL_STYLE;
		case _COLORED_COMPLETION_PREFIX_HASH:
			return RL42_COLORED_COMPLETION_PREFIX;
		case _COMPLETION_DISPLAY_WIDTH_HASH:
			return RL42_COMPLETION_DISPLAY_WIDTH;
		case _COMPLETION_IGNORE_CASE_HASH:
			return RL42_COMPLETION_IGNORE_CASE;
		case _COMPLETION_MAP_CASE_HASH:
			return RL42_COMPLETION_MAP_CASE;
		case _COMPLETION_QUERY_ITEMS_HASH:
			return RL42_COMPLETION_QUERY_ITEMS;
		case _DISABLE_COMPLETION_HASH:
			return RL42_DISABLE_COMPLETION;
		case _EDITING_MODE_HASH:
			return RL42_EDITING_MODE;
		case _ENABLE_BRACKETED_PASTE_HASH:
			return RL42_ENABLE_BRACKETED_PASTE;
		case _ENABLE_HIGHLIGHT_HASH:
			return RL42_ENABLE_HIGHLIGHT;
		case _EXPAND_TILDE_HASH:
			return RL42_EXPAND_TILDE;
		case _HISTORY_SIZE_HASH:
			return RL42_HISTORY_SIZE;
		case _HORIZONTAL_SCROLL_MODE_HASH:
			return RL42_HORIZONTAL_SCROLL_MODE;
		case _KEYSEQ_TIMEOUT_HASH:
			return RL42_KEYSEQ_TIMEOUT;
		case _MARK_DIRECTORIES_HASH:
			return RL42_MARK_DIRECTORIES;
		case _MARK_SYMLINKED_DIRECTORIES_HASH:
			return RL42_MARK_SYMLINKED_DIRECTORIES;
		case _MATCH_HIDDEN_FILES_HASH:
			return RL42_MATCH_HIDDEN_FILES;
		case _PAGE_COMPLETIONS_HASH:
			return RL42_PAGE_COMPLETIONS;
		case _SEARCH_IGNORE_CASE_HASH:
			return RL42_SEARCH_IGNORE_CASE;
		case _VISIBLE_STATS_HASH:
			return RL42_VISIBLE_STATS;
		case _HIGHLIGHT_COLOR_HASH:
			return RL42_HIGHLIGHT_COLOR;
	}
	return RL42_SETTING_NONE;
}

static inline const char	*_convert_spaces(char *s) {
	size_t	i;

	for (i = 0; s[i]; i += charsize_utf8(s[i]))
		if (is_space(utf8_decode(&s[i])))
			s[i] = ' ';
	return s;
}

static inline u8	_parse_line(vector lines, cvector line, const size_t line_n) {
	struct {
		i64	i64;
		u64	u64;
	}				n;
	rl42_cfg_line	_line;
	const char		*s;
	size_t			elements;

	s = *(const char **)vector_get(line, 0);
	if (*s == '#') {
		vector_delete((vector)line);
		return 1;
	}
	if (strcmp(s, "bind") == 0)
		_line.type = BIND;
	else if (strcmp(s, "set") == 0)
		_line.type = SETTING;
	else {
		warn("rl42: rl42_load_config: unrecognized keyword on line #%zu: %s\n", line_n, s);
		vector_delete((vector)line);
		return 1;
	}
	elements = vector_size(line);
	if (_line.type == BIND) {
		switch (elements) {
			case 1:
				warn("rl42: rl42_load_config: missing key-sequence on line #%zu\n", line_n);
				vector_delete((vector)line);
				return 1;
			case 2:
				warn("rl42: rl42_load_config: missing command on line #%zu\n", line_n);
				vector_delete((vector)line);
				return 1;
			case 3:
				warn("rl42: rl42_load_config: missing mode on line #%zu\n", line_n);
				vector_delete((vector)line);
				return 1;
		}
		s = *(const char **)vector_get(line, 3);
		_line.line.bind.mode = _parse_mode(s);
		if (_line.line.bind.mode == CURRENT) {
			warn("rl42: rl42_load_config: unrecognized mode on line #%zu: %s\n", line_n, s);
			vector_delete((vector)line);
			return 1;
		}
		_line.line.bind.keyseq = strdup(*(const char **)vector_get(line, 1));
		_line.line.bind.val = strdup(*(const char **)vector_get(line, 2));
		_line.line.bind.type = (*_line.line.bind.val == '\'' || *_line.line.bind.val == '"') ? MACRO : CMD;
	} else {
		switch (vector_size(line)) {
			case 1:
				warn("rl42: rl42_load_config: missing variable name on line #%zu\n", line_n);
				vector_delete((vector)line);
				return 1;
			case 2:
				warn("rl42: rl42_load_config: missing value on line #%zu\n", line_n);
				vector_delete((vector)line);
				return 1;
		}
		s = *(const char **)vector_get(line, 1);
		_line.line.setting.var = _parse_var(s);
		switch (_line.line.setting.var) {
			case RL42_SETTING_NONE:
				warn("rl42: rl42_load_config: unrecognized setting on line #%zu: %s\n", line_n, s);
				vector_delete((vector)line);
				return 1;
			case RL42_BELL_STYLE:
				s = *(const char **)vector_get(line, 2);
				if (strcasecmp(s, "none") == 0)
					_line.line.setting.val.i64 = RL42_BELL_NONE;
				else if (strcasecmp(s, "audible") == 0)
					_line.line.setting.val.i64 = RL42_BELL_AUDIBLE;
				else if (strcasecmp(s, "visible") == 0)
					_line.line.setting.val.i64 = RL42_BELL_VISIBLE;
				else {
					warn("rl42: rl42_load_config: unrecognized bell style on line #%zu: %s\n", line_n, s);
					vector_delete((vector)line);
					return 1;
				}
				break ;
			case RL42_EDITING_MODE:
				s = *(const char **)vector_get(line, 2);
				if (strcasecmp(s, "vi") == 0)
					_line.line.setting.val.i64 = VI_CMD;
				else if (strcasecmp(s, "emacs") == 0)
					_line.line.setting.val.i64 = EMACS;
				else {
					warn("rl42: rl42_load_config: unrecognized editing mode on line #%zu: %s\n", line_n, s);
					vector_delete((vector)line);
					return 1;
				}
				break ;
			case RL42_HIGHLIGHT_COLOR:
				s = *(const char **)vector_get(line, 2);
				if (_is_uint(s)) {
					n.u64 = strtoul(s, NULL, 10);
					if (n.u64 > 255) {
						warn("rl42: rl42_load_config: highlight color index out of range on line #%zu: %s\n", line_n, s);
						vector_delete((vector)line);
						return 1;
					}
					_line.line.setting.val.hlc = (rl42_hl_color){
						.type = RL42_HL_INDEX,
						.val.index = n.u64
					};
				} else if (_is_rgb(s)) {
					n.u64 = strtoul((const char *)((uintptr_t)s + 1), NULL, 16);
					_line.line.setting.val.hlc = (rl42_hl_color){
						.type = RL42_HL_RGB,
						.val.rgb.r = (n.u64 >> 16) & 0xFFU,
						.val.rgb.g = (n.u64 >> 8) & 0xFFU,
						.val.rgb.b = n.u64 & 0xFFU
					};
				} else {
					warn("rl42: rl42_load_config: invalid highlight color on line #%zu: %s\n", line_n, s);
					vector_delete((vector)line);
					return 1;
				}
				break ;
			case RL42_COLORED_COMPLETION_PREFIX:
			case RL42_COMPLETION_IGNORE_CASE:
			case RL42_COMPLETION_MAP_CASE:
			case RL42_DISABLE_COMPLETION:
			case RL42_ENABLE_BRACKETED_PASTE:
			case RL42_ENABLE_HIGHLIGHT:
			case RL42_EXPAND_TILDE:
			case RL42_HORIZONTAL_SCROLL_MODE:
			case RL42_MARK_DIRECTORIES:
			case RL42_MARK_SYMLINKED_DIRECTORIES:
			case RL42_MATCH_HIDDEN_FILES:
			case RL42_PAGE_COMPLETIONS:
			case RL42_SEARCH_IGNORE_CASE:
			case RL42_VISIBLE_STATS:
				s = *(const char **)vector_get(line, 2);
				if (strcasecmp(s, "on") == 0)
					_line.line.setting.val.u64 = 1;
				else if (strcasecmp(s, "off") == 0)
					_line.line.setting.val.u64 = 0;
				else {
					warn("rl42: rl42_load_config: invalid argument to on/off setting on line #%zu: %s\n", line_n, s);
					vector_delete((vector)line);
					return 1;
				}
				break ;
			case RL42_COMPLETION_DISPLAY_WIDTH:
			case RL42_HISTORY_SIZE:
			case RL42_KEYSEQ_TIMEOUT:
				s = *(const char **)vector_get(line, 2);
				if (!_is_int(s)) {
					warn("rl42: rl42_load_config: invalid argument to signed numeric setting on line #%zu: %s\n", line_n, s);
					vector_delete((vector)line);
					return 1;
				}
				n.i64 = strtol(s, NULL, 10);
				_line.line.setting.val.i64 = (n.i64 >= 0) ? n.i64 : -1;
				break ;
			case RL42_COMPLETION_QUERY_ITEMS:
				s = *(const char **)vector_get(line, 2);
				if (!_is_uint(s)) {
					warn("rl42: rl42_load_config: invalid argument to unsigned numeric setting on line #%zu: %s\n", line_n, s);
					vector_delete((vector)line);
					return 1;
				}
				_line.line.setting.val.u64 = strtoul(s, NULL, 10);
		}
	}
	vector_delete((vector)line);
	return vector_push(lines, _line);
}

static inline u8	_exec_lines(cvector lines) {
	rl42_cfg_line	*line;
	size_t			size;
	size_t			i;

	for (i = 0, size = vector_size(lines); i < size; i++) {
		line = vector_get(lines, i);
		if (line->type == BIND)
			rl42_bind(line->line.bind.keyseq, line->line.bind.val, REMAP, line->line.bind.mode);
		else
			rl42_set(line->line.setting.var, line->line.setting.val);
	}
	return 1;
}

static inline u8	_is_int(const char *s) {
	if (!*s)
		return 0;
	if (*s == '-' || *s == '+')
		s++;
	while (*s && isdigit(*s))
		s++;
	return (*s == '\0') ? 1 : 0;
}

static inline u8	_is_uint(const char *s) {
	if (!*s)
		return 0;
	if (*s == '+')
		s++;
	while (*s && isdigit(*s))
		s++;
	return (*s == '\0') ? 1 : 0;
}

static inline u8	_is_rgb(const char *s) {
	if (strlen(s) != 7)
		return 0;
	if (*s++ != '#')
		return 0;
	while (*s && isxdigit(*s))
		s++;
	return (*s == '\0') ? 1 : 0;
}

static void	_free_cfg_line(rl42_cfg_line *line) {
	if (line->type == BIND) {
		free((char *)line->line.bind.keyseq);
		free((char *)line->line.bind.val);
	}
}
