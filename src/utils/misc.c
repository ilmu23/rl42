// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<misc.c>>

#include <ctype.h>
#include <unistd.h>

#include "internal/_defs.h"
#include "internal/_vector.h"
#include "internal/_display.h"

extern rl42_numeric_arg	n_arg;

extern u16	term_width;

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

size_t	calculate_scroll_space(const rl42_line *line) {
	size_t	prompt_length;

	prompt_length = line->root.col + ((line->prompt.sprompt) ? vector_size(line->prompt.sprompt) : 0);
	return (prompt_length > (size_t)term_width - 1) ? term_width : term_width - prompt_length - 1;
}

i64	get_numeric_arg(rl42_line *line, const u8 redisplay) {
	if (!n_arg.set)
		return NUMERIC_ARG_NOT_SET;
	vector_delete(line->prompt.sprompt);
	line->prompt.sprompt = NULL;
	n_arg.set = 0;
	if (redisplay)
		term_display_line(line, 0);
	return (!n_arg.neg) ? n_arg.val : (n_arg.val) ?  -n_arg.val : -NUMERIC_ARG_MAX - 1;
}

u8	move_to_start_of_word(rl42_line *line) {
	if (line->i == 0)
		return 0;
	if (isspace(*(u32 *)vector_get(line->line, line->i - 1))) do
		line->i--;
	while (line->i > 0 && isspace(*(u32 *)vector_get(line->line, line->i)));
	while (line->i > 0 && !isspace(*(u32 *)vector_get(line->line, line->i - 1)))
		line->i--;
	return 1;
}

u8	move_to_end_of_word(rl42_line *line) {
	size_t	len;

	len = vector_size(line->line);
	if (line->i == len)
		return 0;
	if (isspace(*(u32 *)vector_get(line->line, line->i))) do
		line->i++;
	while (line->i < len && isspace(*(u32 *)vector_get(line->line, line->i)));
	while (line->i < len && !isspace(*(u32 *)vector_get(line->line, line->i)))
		line->i++;
	return 1;
}

static inline u8	isprint_uc(const u32 ucp) {
	return (ucp < 0x20U || ucp == 0x7F) ? 0 : 1;
}
