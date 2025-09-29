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
