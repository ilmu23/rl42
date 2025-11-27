// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_defs.h>>

#pragma once

#define SGR0		"\x1b[m"
#define SGR_INFO	"\x1b[1;38;5;190m"
#define SGR_WARN	"\x1b[1;38;5;202m"
#define SGR_ERROR	"\x1b[1;38;5;196m"

#ifndef NUMERIC_ARG_MAX
# define NUMERIC_ARG_MAX	1000000
#endif

#define NUMERIC_ARG_NOT_SET	NUMERIC_ARG_MAX + 1

#define add_mark(m, p)	(m.pos = p, m.set = 1)

// Checks whether s1 equals s2
#define str_equals(s1, s2)	(s1 == s2 || (s1 && s2 && strcmp(s1, s2) == 0))

// Checks whether s1 equals the string literal s2
#define strl_equals(s1, s2)	(s1 && strcmp(s1, s2) == 0)

#ifdef USE_LIBICU
#include <unicode/uchar.h>

#define is_lower	u_islower
#define is_upper	u_isupper
#define is_title	u_istitle
#define is_space	u_isspace
#define is_print	u_isprint

#define to_lower	u_tolower
#define to_upper	u_toupper
#define to_title	u_totitle
#else
#include <ctype.h>

#define is_lower	islower
#define is_upper	isupper
#define is_title	isupper
#define is_space	isspace
#define is_print	isprint

#define to_lower	tolower
#define to_upper	toupper
#define to_title	toupper
#endif
