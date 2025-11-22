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
