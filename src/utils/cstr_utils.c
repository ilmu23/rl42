// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<cstr_utils.c>>

#include <string.h>
#include <stdlib.h>

#include "internal/_utils.h"
#include "internal/_vector.h"

char	*cstr_join(const char *s1, const char *s2) {
	size_t	size;
	char	*out;

	if (!s1 && !s2)
		return strdup("");
	if (!s1)
		return strdup(s2);
	if (!s2)
		return strdup(s1);
	size = strlen(s1) + 1;
	size += strlen(s2);
	out = malloc(size * sizeof(*out));
	return cstr_joinb(s1, s2, out, size);
}

char	*cstr_joinb(const char *s1, const char *s2, char *buf, const size_t size) {
	size_t	len;
	size_t	i;

	if (!buf || !size)
		return NULL;
	if (s1) {
		len = strlen(s1);
		i = (len < size) ? len : size;
		memcpy(buf, s1, i);
	} else
		i = 0;
	if (s2) {
		len = strlen(s2);
		memcpy(&buf[i], s2, (len < size - i) ? len : size - i);
		i += (len < size  - 1) ? len : size - i;
	}
	buf[size - i] = '\0';
	return buf;
}

char	*cstr_substr(const char *s, const size_t start, const size_t len) {
	size_t	slen;

	if (start >= strlen(s))
		slen = 1;
	else
		slen = strnlen(&s[start], len) + 1;
	return cstr_substrb(s, start, len, malloc(slen * sizeof(char *)), slen * sizeof(char *));
}

char	*cstr_substrb(const char *s, const size_t start, const size_t len, char *buf, const size_t buf_size) {
	size_t	slen;
	size_t	i;

	if (!buf || !buf_size)
		return NULL;
	i = 0;
	if (buf_size != 1) for (slen = strnlen(&s[start], min(len, buf_size - 1)); i < slen; i++)
		buf[i] = s[start + i];
	buf[i] = '\0';
	return buf;
}

vector	cstr_split(const char *s, const char c) {
	vector	out;
	size_t	i;
	size_t	j;

	out = vector(char *, 1, free);
	if (!out)
		return NULL;
	if (c == '\0') {
		vector_push(out, (const char *){strdup(s)});
		return out;
	}
	for (i = j = 0; s[i]; i++) {
		if (s[i] == c) {
			if (i == j) do {
				i++;
				j++;
			} while (s[i] == c); else {
				if (!vector_push(out, (const char *){cstr_substr(s, j, i - j)})) {
					vector_delete(out);
					return NULL;
				}
				j = i + 1;
			}
		}
	}
	if ((i != j || vector_size(out) == 0) && !vector_push(out, (const char *){cstr_substr(s, j, i - j)})) {
		vector_delete(out);
		return NULL;
	}
	return out;
}
