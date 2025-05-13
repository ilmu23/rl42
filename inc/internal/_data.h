// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<data.h>>

#pragma once

#include "internal/_defs.h"

#include "data.h"

#include <stddef.h>

// Modes used by rl42_redisplay
// CURSOR = Redisplay starting from the cursor
// LINE = Redisplay the whole input line
// PROMPT = Redisplay the prompt and the input line
// CLEAR = Clear the screen and redisplay everything
typedef enum __redisplay_mode {
	CURSOR,
	LINE,
	PROMPT,
	CLEAR
}	rl42_redisplay_mode;

// Vector sorting modes
// ASCENDING = sort elements in ascending order
// DESCENDING = sort elements in descending order
// NONE = No sorting
typedef enum __vec_sort {
	ASCENDING,
	DESCENDING,
	NONE
}	vector_sort_mode;

// Stores two items as a pair
// item1 = first item
// item2 = second item
typedef struct __pair {
	void	*item1;
	void	*item2;
}	pair;

// Generic map
// items = array for storing the key - value pairs
// capacity = size of the item array
// itemcount = amount of pairs in the array
typedef struct __map {
	pair	**items;
	size_t	capacity;
	size_t	itemcount;
}	map;

// Generic vector
// data = array for storing data
// capacity = size of the array
// size = amount of elements in the array
typedef struct __vector {
	vector_sort_mode	sort_mode;
	uintptr_t			*data;
	size_t				capacity;
	size_t				size;
}	vector;

// History node containing the original line and
// the potentially edited version
typedef struct __hist_node {
	const char	*line;
	const char	*edit;
}	rl42_hist_node;

// Stores a single utf-8 encoded character
// cbuf = buffer storing the character
// csize = amount of bytes the character takes in the buffer
typedef struct __char {
	char	cbuf[4];
	u8		csize;
}	rl42_char;

// Node in a key sequence tree
// f = function bound to the currently enterd sequence
// next = map containing all possible following characters
// c = indicated whether this sequence has been const bound
typedef struct __key_tree {
	rl42_fn	f;
	map		*next;
	u8		c;
}	rl42_key_tree;

// Contains info about rl42 functions
// f = pointer to the function
// fname = name of the function
typedef struct __fn_info {
	rl42_fn		f;
	const char	*fname;
	vector		*binds[3];
}	rl42_fn_info;

// Stores a string of rl42_chars with metadata
// str = string data
// sze = total size in memory
// cap = current maximum capacity (length)
// len = current length
typedef struct __string {
	rl42_char	*str;
	size_t		sze;
	size_t		cap;
	size_t		len;
}	rl42_string;

// Stores information about the cursor
// prompt row / col = starting position of the current prompt
// input row / col = starting position of the current input line
// row / col = current cursor position
typedef struct __cursor {
	i16	prompt_row;
	i16	prompt_col;
	i16	input_row;
	i16	input_col;
	i16	row;
	i16	col;
}	rl42_cursor;

// Stores all prompt information
// sprompt = current special prompt, for example the current digit-argument
// prompt = user provided prompt
typedef struct __prompt {
	rl42_string	*sprompt;
	rl42_string	*prompt;
}	rl42_prompt;

// Stores the current input environment
// prompt = current prompt
// keyseq = keyseq that is currently being executed
// line = current input line
// i = cursor index in the input line
typedef struct __line {
	rl42_prompt			prompt;
	const rl42_string	*keyseq;
	rl42_string			*line;
	size_t				i;
}	rl42_line;

// Stores a rl42 mark position
// set = indicates whether the mark is active
// pos = position of the mark in the input line
typedef struct __mark {
	u8	set;
	u64	pos;
}	rl42_mark;
