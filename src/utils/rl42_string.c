// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_string.c>>

#include "internal/_utils.h"
#include <limits.h>

static inline u8	_clen_utf8(const char c);

rl42_string	*cstr_to_rl42str(const char *s) {
	rl42_string	*out;
	size_t		i;

	out = malloc(sizeof(*out));
	if (out) {
		out->len = strlen_utf8(s);
		out->cap = out->len;
		out->str = calloc(out->cap, sizeof(*out->str));
		if (out->str) {
			for (i = out->sze = 0; *s; i++) {
				out->str[i].csize = _clen_utf8(*s);
				memcpy(out->str[i].cbuf, s, out->str[i].csize);
				out->sze += out->str[i].csize;
				s += out->str[i].csize;
			}
		} else {
			free(out);
			out = NULL;
		}
	}
	return out;
}

char	*rl42str_to_cstr(const rl42_string *s) {
	size_t	i;
	size_t	j;
	char	*out;

	out = malloc(s->sze + 1);
	if (out) {
		for (i = j = 0; i < s->len; i++) {
			memcpy(&out[j], s->str[i].cbuf, s->str[i].csize);
			j += s->str[i].csize;
		}
		out[j] = '\0';
	}
	return out;
}

size_t	strlen_utf8(const char *s) {
	size_t	len;
	size_t	i;

	for (i = len = 0; s[i]; len++)
		i += _clen_utf8(s[i]);
	return len;
}

static inline u8	_clen_utf8(const char c) {
	if (c & 0x80) {
		switch (c & 0x70) {
			case 0x70:
				return 4;
			case 0x60:
				return 3;
			case 0x40:
				return 2;
		}
	}
	return 1;
}
