// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_data.h>>

#pragma once

#include "defs.h"

#include <stddef.h>

#ifndef __RL42_USE_EXTERNAL_CONTAINERS
#include "internal/_darray.h"
#else
#include <libcont/darray.h>
#endif

// Stores a cursor position
// row = row number
// col = column number
typedef struct __cursor_pos {
	i16	row;
	i16	col;
}	rl42_cursor_pos;

// Stores all prompt information
// root = row / col of start of prompt
// sprompt = current special prompt, for example the current digit-argument
// prompt = user provided prompt
typedef struct __prompt {
	const rl42_cursor_pos	*root;
	darray					sprompt;
	darray					prompt;
}	rl42_prompt;

// Stores the current input environment
// root = row / col of start of input
// prompt = current prompt
// keyseq = keyseq that is currently being executed
// line = current input line
// rows = amount of rows taken up
// i = cursor index in the input line
typedef struct __line {
	const rl42_cursor_pos	*root;
	rl42_prompt				prompt;
	darray					keyseq;
	darray					line;
	size_t					rows;
	size_t					i;
}	rl42_line;

// Function for getting completions
// 1st arg = pattern to complete
// 2nd arg = darray of space separated strings before the pattern, NULL if nothing before pattern
// Returns darray containing all possible completions or NULL if no completions were found
typedef cdarray	(*rl42_completion_fn)(const char *, cdarray);

typedef u8	(*rl42_fn)(rl42_line *);

#ifdef __RL42_INTERNAL

#include "data.h"

typedef enum __direction {
	FORWARD = 0,
	BACKWARD = 1
}	rl42_direction;

typedef enum __bpm_state {
	BPM_ENABLED = 0,
	BPM_DISABLED = 1,
	BPM_TOGGLE = 2,
}	rl42_bpm_state;

#ifndef __RL42_USE_EXTERNAL_CONTAINERS
#include "internal/_map.h"
#include "internal/_list.h"
#else
#include <libcont/map.h>
#include <libcont/list.h>
#endif

// History node containing the original line and
// the potentially edited version
// line = stored line
// edit = temporarily edited line
// new = indicates whether this line was loaded from a file (0) or
// entered in the current session (1)
// entry_n = Entry number, starting from 1 for the first entry
typedef struct __hist_node {
	const char	*line;
	const char	*edit;
	const u8	new;
	size_t		entry_n;
}	rl42_hist_node;

// Node in a key sequence tree
// f = function bound to the currently enterd sequence
// next = map containing all possible following characters
// c = indicated whether this sequence has been const bound
typedef struct __key_tree {
	rl42_fn	f;
	map		next;
	u8		c;
}	rl42_key_tree;

// Function match information
// fn = Currently matched position in the key tree
// run = Indicates whether the function found at fn should be run
typedef struct __fn_match {
	rl42_key_tree	*fn;
	u8				run;
}	rl42_fn_match;

// Contains info about rl42 functions
// f = pointer to the function
// fname = name of the function
// binds = all sequences bound to the function, separated by editing mode
// macro = indicates whether f is a macro caller
typedef struct __fn_info {
	rl42_fn		f;
	const char	*fname;
	darray		binds[3];
	u8			macro;
}	rl42_fn_info;

// Stores a rl42 mark position
// pos = position of the mark in the input line
// set = indicates whether the mark is active
typedef struct __mark {
	u64	pos;
	u8	set;
}	rl42_mark;

// Stores a keyboard event
// esc = unrecognized escape sequence, only set when code is KB_UNRECOGNIZED_ESCAPE
// code = key code
// text = text produced
// mods = modifiers present
typedef struct __kb_event {
	cdarray	esc;
	u32		code;
	u32		text;
	u16		mods;
}	rl42_kb_event;

// Stores numeric argument info
// val = value of the argument
// neg = indicates whether val is negative
// set = indicates whether an argument is set
typedef struct __arg {
	i64	val;
	u8	neg;
	u8	set;
}	rl42_numeric_arg;

// Stores config file bind information
// keyseq = keyseq to bind to
// type = type of binding to make
// val = binding to make
// mode = mode to bind in
typedef struct __cfg_bind {
	const char			*keyseq;
	enum {
		CMD,
		MACRO
	}					type;
	const char			*val;
	rl42_editing_mode	mode;
}	rl42_cfg_bind;

// Stores config file setting information
// var = name of the setting
// val = value of the setting
typedef struct __cfg_setting {
	rl42_setting		var;
	rl42_setting_val	val;
}	rl42_cfg_setting;

// Stores a config file line
// type = type of the config line
// line = config line information
typedef struct __cfg_line {
	enum {
		BIND,
		SETTING
	}	type;
	union {
		rl42_cfg_bind		bind;
		rl42_cfg_setting	setting;
	}	line;
}	rl42_cfg_line;

// Bit field for storing display options
#if __STDC_VERSION__ >= 202311L
typedef unsigned _BitInt(4)	rl42_display_opts;
#else
typedef u8	rl42_display_opts;
#endif
#endif
