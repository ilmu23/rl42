// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42str_utils.c>>

#include "internal/_defs.h"
#include "internal/_utils.h"
#include "internal/_darray.h"

size_t	rl42str_find(cdarray s, cdarray substr) {
	size_t	sslen;
	size_t	slen;
	size_t	i;
	size_t	j;

	if (s && substr) {
		slen = darray_size(s);
		sslen = darray_size(substr);
		if (sslen == 0)
			return RL42STR_SUBSTR_NOT_FOUND;
		for (i = 0; i + sslen <= slen; i++) {
			for (j = 0; j < sslen; j++)
				if (*(u32 *)darray_get(s, i + j) != *(u32 *)darray_get(substr, j))
					break ;
			if (j == sslen)
				return i;
		}
	}
	return RL42STR_SUBSTR_NOT_FOUND;
}

size_t	rl42str_find_case(cdarray s, cdarray substr) {
	size_t	sslen;
	size_t	slen;
	size_t	i;
	size_t	j;

	if (s && substr) {
		slen = darray_size(s);
		sslen = darray_size(substr);
		if (sslen == 0)
			return RL42STR_SUBSTR_NOT_FOUND;
		for (i = 0; i + sslen <= slen; i++) {
			for (j = 0; j < sslen; j++)
				if (to_upper(*(u32 *)darray_get(s, i + j)) != to_upper(*(u32 *)darray_get(substr, j)))
					break ;
			if (j == sslen)
				return i;
		}
	}
	return RL42STR_SUBSTR_NOT_FOUND;
}
