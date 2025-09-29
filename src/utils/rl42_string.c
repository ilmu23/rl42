// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_string.c>>

#include <stdlib.h>
#include <string.h>

#include "internal/_utils.h"
#include "internal/_vector.h"

vector	cstr_to_rl42str(const char *s) {
	vector	out;
	size_t	len;
	size_t	i;
	u32		ucp;

	len = strlen(s);
	out = vector(u32, (size_t){strlen_utf8(s) + ((len == 0) ? 1 : 0)}, NULL);
	if (out) for (i = 0; i < len; i++) {
		ucp = utf8_decode(&s[i]);
		if (!vector_push(out, ucp))
			goto cstr_to_rl42str_err;
		if (ucp > 0xFFFFU)
			i += 3;
		else if (ucp > 0x7FFU)
			i += 2;
		else if (ucp > 0x7FU)
			i++;
	}
	return out;
cstr_to_rl42str_err:
	vector_delete(out);
	return NULL;
}

char	*rl42str_to_cstr(cvector s) {
	size_t	i;
	size_t	j;
	size_t	len;
	char	*out;

	len = vector_size(s);
	if (!len)
		return strdup("");
	out = malloc(((len * sizeof(u32)) + 1) * sizeof(*out));
	if (out) for (i = j = 0; i < len; i++) {
		utf8_encode(*(u32 *)vector_get(s, i), &out[j]);
		while (out[j])
			j++;
	}
	return out;
}
