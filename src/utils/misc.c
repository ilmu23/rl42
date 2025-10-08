// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<misc.c>>

#include <unistd.h>

#define __RL42_INTERNAL
#include "internal/_data.h"
#include "internal/_defs.h"
#include "internal/_vector.h"

extern rl42_numeric_arg	n_arg;

static inline u8	isprint_uc(const u32 ucp);

ssize_t	__putchar(const char c) {
	return write(1, &c, sizeof(c));
}

size_t	calculate_cursor_offset(const rl42_line *line) {
	size_t	offset;
	size_t	len;
	size_t	i;

	for (i = offset = 0, len = vector_size(line->line); i < line->i && i < len; i++)
		offset += (isprint_uc(*(u32 *)vector_get(line->line, i))) ? 1 : 2;
	return offset;
}

i64	get_numeric_arg(rl42_line *line) {
	if (!n_arg.set)
		return 0;
	vector_delete(line->prompt.sprompt);
	line->prompt.sprompt = NULL;
	n_arg.set = 0;
	return (!n_arg.neg) ? n_arg.val : (n_arg.val) ?  -n_arg.val : -NUMERIC_ARG_MAX - 1;
}

static inline u8	isprint_uc(const u32 ucp) {
	return (ucp < 0x20U || ucp == 0x7F) ? 0 : 1;
}
