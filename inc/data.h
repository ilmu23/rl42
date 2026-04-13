// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<data.h>>

#pragma once

#include "defs.h"

// Changes the behaviour of the rl42_bind function in the case that
// the key sequence the user is trying to bind is already bound
// WARN = Issue a warning to the user
// QUIET = Silently fail
// REMAP = Notify the user and remap the key sequence, it it isn't const bound
// QREMAP = Silently remap the key sequence, if it isn't const bound
typedef enum __bind_mode {
	RL42_BM_WARN = 0,
	RL42_BM_QUIET = 1,
	RL42_BM_REMAP = 2,
	RL42_BM_QREMAP = 3,
#ifdef __RL42_INTERNAL
	RL42_BM_CONST = 4
#endif
}	rl42_bind_mode;

// Editing mode identifiers
// EMACS = emacs editing mode
// VI_CMD = vi command mode
// VI_INS = vi insert mode
// CURRENT = Currently active editing mode
typedef enum __editing_mode {
	RL42_EM_EMACS = 0,
	RL42_EM_VI_CMD = 1,
	RL42_EM_VI_INS = 2,
	RL42_EM_CUSTOM_1 = 3,
	RL42_EM_CUSTOM_2 = 4,
	RL42_EM_CUSTOM_3 = 5,
	RL42_EM_CUSTOM_4 = 6,
	RL42_EM_CUSTOM_5 = 7,
	RL42_EM_CURRENT = 8
}	rl42_editing_mode;

// Terminal bell style
// NONE = no bell
// AUDIBLE = audible bell
// VISIBLE = visual bell
typedef enum __bell_style {
	RL42_BELL_NONE = 0,
	RL42_BELL_AUDIBLE = 1,
	RL42_BELL_VISIBLE = 2
}	rl42_bell_style;

// Highlight color
// type = type of highlight color
// val.index = terminal color index
// val.rgb = rgb color value
typedef struct __hl_color {
	enum {
		RL42_HL_INDEX,
		RL42_HL_RGB
	}	type;
	union {
		u8	index;
		struct {
			u8	r;
			u8	g;
			u8	b;
		}	rgb;
	}	val;
}	rl42_hl_color;

// Setting names
typedef enum {
	RL42_BELL_STYLE = 0,
	RL42_COLORED_COMPLETION_PREFIX = 1,
	RL42_COMPLETION_DISPLAY_WIDTH = 2,
	RL42_COMPLETION_IGNORE_CASE = 3,
	RL42_COMPLETION_MAP_CASE = 4,
	RL42_COMPLETION_QUERY_ITEMS = 5,
	RL42_DISABLE_COMPLETION = 6,
	RL42_EDITING_MODE = 7,
	RL42_ENABLE_HIGHLIGHT = 8,
	RL42_EXPAND_TILDE = 9,
	RL42_HISTORY_SIZE = 10,
	RL42_HORIZONTAL_SCROLL_MODE = 11,
	RL42_KEYSEQ_TIMEOUT = 12,
	RL42_MARK_DIRECTORIES = 13,
	RL42_MARK_SYMLINKED_DIRECTORIES = 14,
	RL42_MATCH_HIDDEN_FILES = 15,
	RL42_PAGE_COMPLETIONS = 16,
	RL42_SEARCH_IGNORE_CASE = 17,
	RL42_VISIBLE_STATS = 18,
	RL42_HIGHLIGHT_COLOR = 19,
	RL42_ENABLE_BRACKETED_PASTE = 20,
	RL42_DISABLE_HISTORY = 21,
	RL42_SETTING_NONE = 22
}	rl42_setting;

// Stores the value of a setting
typedef union __cfg_setting_val {
	rl42_hl_color	hlc;
	i64				i64;
	u64				u64;
}	rl42_setting_val;
