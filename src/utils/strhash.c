// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<strhash.c>>

#include "internal/_utils.h"
#include "internal/_vector.h"

static inline u64	_upow(const u64 x, u64 y);

u64	cstr_hash(const char *s, const u64 salt, const size_t max) {
	size_t	i;
	size_t	len;
	u64		hash;

	for (i = hash = 0, len = strlen(s); i < len; i++) {
		hash += _upow(salt, len - i + 1) * s[i];
		hash %= max;
	}
	return hash;
}

u64	rl42str_hash(const vector s, const u64 salt, const size_t max) {
	size_t	i;
	size_t	len;
	u64		hash;

	for (i = hash = 0 , len = vector_size(s); i < len; i++) {
		hash += _upow(salt, len - i + 1) * *(u32 *)vector_get(s, i);
		hash %= max;
	}
	return hash;
}

static inline u64	_upow(const u64 x, u64 y) {
	u64	out;

	if (y < 1)
		return 1;
	out = 1;
	while (y--)
		out *= x;
	return out;
}
