// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<transpose_words.c>>

#include <ctype.h>
#include <alloca.h>
#include <string.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_vector.h"
#include "internal/_display.h"

static inline void	_transpose(rl42_line *line, size_t w1, size_t w2, const size_t len);
static inline u8	_on_first_word(const rl42_line *line);

// 1. Set w2 start to start of next word
// 2. If there is no next word, set w2 start to start of current word
// 3. Set w1 start to start of word preceding start of w2

rl42_fn(transpose_words) {
	size_t	len;
	size_t	w1;
	size_t	w2;

	if (_on_first_word(line))
		return 1;
	len = vector_size(line->line);
	w2 = (line->i < len) ? line->i : line->i - 1;
	while (w2 < len && !isspace(*(u32 *)vector_get(line->line, w2)))
		w2++;
	while (w2 < len && isspace(*(u32 *)vector_get(line->line, w2)))
		w2++;
	if (w2 == len) {
		w2 = (line->i < len) ? line->i : line->i - 1;
		while (isspace(*(u32 *)vector_get(line->line, w2 - 1)))
			w2--;
		while (!isspace(*(u32 *)vector_get(line->line, w2 - 1)))
			w2--;
	}
	w1 = w2 - 1;
	while (w1 > 0 && isspace(*(u32 *)vector_get(line->line, w1 - 1)))
		w1--;
	while (w1 > 0 && !isspace(*(u32 *)vector_get(line->line, w1 - 1)))
		w1--;
	_transpose(line, w1, w2, len);
	while (w2 < len && isspace(*(u32 *)vector_get(line->line, w2)))
		w2++;
	while (w2 < len && !isspace(*(u32 *)vector_get(line->line, w2)))
		w2++;
	line->i = w2;
	return term_display_line(line, 0);
}

void	_transpose(rl42_line *line, size_t w1, size_t w2, const size_t len) {
	size_t	i;
	size_t	len1;
	size_t	len2;
	u32		*buf;

	for (len1 = 0; !isspace(*(u32 *)vector_get(line->line, w1 + len1)); len1++)
		;
	for (len2 = 0; w2 + len2 < len && !isspace(*(u32 *)vector_get(line->line, w2 + len2)); len2++)
		;
	for (i = 0; w2 + len2 + i < len && isspace(*(u32 *)vector_get(line->line, w2 + len2 + i)); i++)
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
		memcpy(vector_get(line->line, w2 - i), buf, len2 * sizeof(u32));
	}
}

static inline u8	_on_first_word(const rl42_line *line) {
	size_t	len;
	size_t	i;

	len = vector_size(line->line);
	i = (line->i < len) ? line->i : line->i - 1;
	if (isspace(*(u32 *)vector_get(line->line, i)))
		return 0;
	while (i > 0 && !isspace(*(u32 *)vector_get(line->line, i)))
		i--;
	return (i == 0) ? 1 : 0;
}
