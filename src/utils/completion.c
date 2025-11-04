// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<completion.c>>

#include <ctype.h>
#include <string.h>

#include "complete.h"

#include "internal/_vector.h"
#include "internal/_complete.h"

static rl42_completion_fn(_complete_files);

rl42_completion_fn	cmp_fn = _complete_files;

static inline u8	_cmp(const char c1, const char c2);
static inline u8	_cmp_ign_case(const char c1, const char c2);
static inline u8	_cmp_map_case(const char c1, const char c2);

static u8	(*compare[3])(const char, const char) = { _cmp, _cmp_ign_case, _cmp_map_case };

void	set_completion_fn(rl42_completion_fn f) {
	cmp_fn = (f) ? f : _complete_files;
}

cvector	cmp_get_common(cvector completions) {
	enum {
		NORMAL = 0,
		IGN_CASE = 1,
		MAP_CASE = 2
	}			cmp_type;
	const char	*s1;
	const char	*tmp;
	size_t		count;
	size_t		len;
	size_t		i;
	size_t		j;

	if (completions && vector_size(completions) > 1) {
		count = vector_size(completions);
		s1 = *(const char **)vector_get(completions, 0);
		cmp_type = NORMAL;
		for (i = 0, len = strlen(s1); i < len; i++) {
			for (j = 1; j < count; j++) {
				tmp = *(const char **)vector_get(completions, j);
				if (!compare[cmp_type](s1[i], tmp[i]))
					break ;
			}
			if (j != count)
				break ;
		}
		if (i > 0) {
			vector_resize((vector)completions, 1);
			((char *)s1)[i] = '\0';
		}
	}
	return completions;
}

static inline u8	_cmp(const char c1, const char c2) {
	return (c1 == c2) ? 1 : 0;
}

static inline u8	_cmp_ign_case(const char c1, const char c2) {
	return (toupper(c1) == toupper(c2)) ? 1 : 0;
}

static inline u8	_cmp_map_case(const char c1, const char c2) {
	return (_cmp_ign_case(c1, c2) || (c1 == '-' && c2 == '_') || (c1 == '_' && c2 == '-')) ? 1 : 0;
}


#include <stdlib.h>
#include "internal/test/defs.h"

static rl42_completion_fn(_complete_files) {
	vector	out;

	(void)pattern;
	out = vector(const char *, 3, free);
	if (out) switch (rand_range(0, 2)) {
		case 0:
			if (!vector_push(out, (const char *){strdup("ayy")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("ayylmao")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("ayy lmao")}))
				return NULL;
			break ;
		case 1:
			if (!vector_push(out, (const char *){strdup("ayy lmao")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("lolmao")}))
				return NULL;
			if (!vector_push(out, (const char *){strdup("lul")}))
				return NULL;
			break ;
		case 2:
			if (!vector_push(out, (const char *){strdup("lul")}))
				return NULL;
	}
	return out;
}
