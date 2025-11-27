// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<misc.c>>

#include <unistd.h>

#include "internal/_defs.h"
#include "internal/_vector.h"
#include "internal/_display.h"

extern rl42_numeric_arg	n_arg;

extern u16	term_width;

size_t	calculate_cursor_offset(const rl42_line *line) {
	size_t	offset;
	size_t	len;
	size_t	i;
	u32		ucp;

	for (i = offset = 0, len = vector_size(line->line); i < line->i && i < len; i++) {
		ucp = *(u32 *)vector_get(line->line, i);
		if (!is_print(ucp)) {
			if (ucp < 0x80U)
				offset += 2;
			else if (ucp < 0x100U)
				offset += 4;
			else if (ucp < 0x1000U)
				offset += 5;
			else if (ucp < 0x10000U)
				offset += 6;
			else if (ucp < 0x100000U)
				offset += 7;
			else if (ucp < 0x1000000U)
				offset += 8;
			else
				offset += 9;
		} else
			offset++;
	}
	return offset;
}

size_t	calculate_scroll_space(const rl42_line *line) {
	size_t	prompt_length;

	prompt_length = line->root->col + ((line->prompt.sprompt) ? vector_size(line->prompt.sprompt) : 0);
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
	if (is_space(*(u32 *)vector_get(line->line, line->i - 1))) do
		line->i--;
	while (line->i > 0 && is_space(*(u32 *)vector_get(line->line, line->i)));
	while (line->i > 0 && !is_space(*(u32 *)vector_get(line->line, line->i - 1)))
		line->i--;
	return 1;
}

u8	move_to_end_of_word(rl42_line *line) {
	size_t	len;

	len = vector_size(line->line);
	if (line->i == len)
		return 0;
	if (is_space(*(u32 *)vector_get(line->line, line->i))) do
		line->i++;
	while (line->i < len && is_space(*(u32 *)vector_get(line->line, line->i)));
	while (line->i < len && !is_space(*(u32 *)vector_get(line->line, line->i)))
		line->i++;
	return 1;
}
