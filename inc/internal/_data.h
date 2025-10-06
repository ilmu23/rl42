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

// Generic vector
typedef struct __vec *			vector;
typedef const struct __vec *	cvector;

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
	rl42_cursor_pos	root;
	vector			sprompt;
	vector			prompt;
}	rl42_prompt;

// Stores the current input environment
// root = row / col of start of input
// prompt = current prompt
// keyseq = keyseq that is currently being executed
// line = current input line
// i = cursor index in the input line
typedef struct __line {
	rl42_cursor_pos	root;
	rl42_prompt		prompt;
	vector			keyseq;
	vector			line;
	size_t			i;
}	rl42_line;

typedef u8	(*rl42_fn)(rl42_line *);

#ifdef __RL42_INTERNAL
typedef enum __direction {
	FORWARD,
	BACKWARD
}	rl42_direction;

// Generic map
typedef struct __map *			map;
typedef const struct __map *	cmap;

// Generic list
typedef struct __lst *			list;
typedef const struct __lst *	clist;

// History node containing the original line and
// the potentially edited version
// entry_n = Entry number, starting from 1 for the first entry
// line = stored line
// edit = temporarily edited line
typedef struct __hist_node {
	const size_t	entry_n;
	const char		*line;
	const char		*edit;
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

// Contains info about rl42 functions
// f = pointer to the function
// fname = name of the function
// binds = all sequences bound to the function, separated by editing mode
typedef struct __fn_info {
	rl42_fn		f;
	const char	*fname;
	vector		binds[3];
}	rl42_fn_info;

// Stores a rl42 mark position
// pos = position of the mark in the input line
// set = indicates whether the mark is active
typedef struct __mark {
	u64	pos;
	u8	set;
}	rl42_mark;

// Stores a keyboard event
// code = key code
// text = text produced
// mods = modifiers present
typedef struct __kb_event {
	u32	code;
	u32	text;
	u16	mods;
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

// Placeholder for display options
typedef u8	rl42_display_opts;
#endif
