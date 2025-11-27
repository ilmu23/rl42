// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<transpose_words.c>>

#include <alloca.h>
#include <string.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"

static inline void	_transpose(rl42_line *line, size_t w1, size_t w2, const size_t len);
static inline u8	_on_first_word(const rl42_line *line);

rl42_fn(transpose_words) {
	size_t	w1;
	size_t	w2;

	if (NEED_REPEAT) {
		if (!repeat(line, transpose_words, NULL))
			return 0;
	} else {
		if (_on_first_word(line))
			return 2;
		move_to_start_of_word(line);
		w2 = line->i;
		move_to_start_of_word(line);
		w1 = line->i;
		if (w1 == w2) {
			move_to_end_of_word(line);
			move_to_end_of_word(line);
			move_to_start_of_word(line);
			w2 = line->i;
			if (w1 == w2)
				return 2;
		}
		_transpose(line, w1, w2, vector_size(line->line));
		move_to_end_of_word(line);
	}
	return (~state_flags & STATE_REPEAT) ? term_display_line(line, 0) : 1;
}

static inline void	_transpose(rl42_line *line, size_t w1, size_t w2, const size_t len) {
	size_t	i;
	size_t	len1;
	size_t	len2;
	u32		*buf;

	for (len1 = 0; !is_space(*(u32 *)vector_get(line->line, w1 + len1)); len1++)
		;
	for (len2 = 0; w2 + len2 < len && !is_space(*(u32 *)vector_get(line->line, w2 + len2)); len2++)
		;
	for (i = 0; w2 + len2 + i < len && is_space(*(u32 *)vector_get(line->line, w2 + len2 + i)); i++)
		;
	if (w2 + len2 + i == len)
		len2 += i;
	if (len1 < len2) {
		buf = alloca(len2 * sizeof(*buf));
		memcpy(buf, vector_get(line->line, w2), len2 * sizeof(u32));
		for (i = 0; i < len2 - len1; i++)
			vector_set(line->line, w2 + i, (u32){' '});
		memcpy(vector_get(line->line, w2 + len2 - len1), vector_get(line->line, w1), len1 * sizeof(u32));
		memcpy(vector_get(line->line, w1), buf, len2 * sizeof(u32));
	} else {
		buf = alloca(len1 * sizeof(*buf));
		memcpy(buf, vector_get(line->line, w1), len1 * sizeof(u32));
		for (i = 0; i < len1 - len2; i++)
			vector_set(line->line, w1 + len2 + i, (u32){' '});
		memcpy(vector_get(line->line, w1), vector_get(line->line, w2), len2 * sizeof(u32));
		memcpy(vector_get(line->line, w2 - i), buf, len1 * sizeof(u32));
	}
}

static inline u8	_on_first_word(const rl42_line *line) {
	size_t	len;
	size_t	i;

	len = vector_size(line->line);
	if (len == 0)
		return 1;
	i = (line->i < len) ? line->i : line->i - 1;
	if (is_space(*(u32 *)vector_get(line->line, i)))
		return 0;
	while (i > 0 && !is_space(*(u32 *)vector_get(line->line, i)))
		i--;
	return (i == 0) ? 1 : 0;
}
