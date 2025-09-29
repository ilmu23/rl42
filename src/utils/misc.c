// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<misc.c>>

#include <unistd.h>

#include "internal/_data.h"
#include "internal/_vector.h"

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

static inline u8	isprint_uc(const u32 ucp) {
	return (ucp < 0x20U || ucp == 0x7F) ? 0 : 1;
}
