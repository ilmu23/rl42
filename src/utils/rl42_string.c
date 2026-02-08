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

darray	cstr_to_rl42str(const char *s) {
	darray	out;
	size_t	len;
	size_t	i;
	u32		ucp;

	len = strlen(s);
	out = darray(u32, (size_t){strlen_utf8(s) + ((len == 0) ? 1 : 0)}, NULL);
	if (out) for (i = 0; i < len; i++) {
		ucp = utf8_decode(&s[i]);
		if (!darray_push(out, ucp))
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
	darray_delete(out);
	return NULL;
}

char	*rl42str_to_cstr(cdarray s) {
	size_t	i;
	size_t	j;
	size_t	len;
	char	*out;

	len = darray_size(s);
	if (!len)
		return strdup("");
	out = malloc(((len * sizeof(u32)) + 1) * sizeof(*out));
	if (out) for (i = j = 0; i < len; i++) {
		utf8_encode(*(u32 *)darray_get(s, i), &out[j]);
		while (out[j])
			j++;
	}
	return out;
}
