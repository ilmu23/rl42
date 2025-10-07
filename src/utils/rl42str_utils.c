// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42str_utils.c>>

#include "internal/_utils.h"
#include "internal/_vector.h"

size_t	rl42str_find(cvector s, cvector substr) {
	size_t	sslen;
	size_t	slen;
	size_t	i;
	size_t	j;

	if (s && substr) {
		slen = vector_size(s);
		sslen = vector_size(substr);
		if (sslen == 0)
			return RL42STR_SUBSTR_NOT_FOUND;
		for (i = 0; i + sslen <= slen; i++) {
			for (j = 0; j < sslen; j++)
				if (*(u32 *)vector_get(s, i + j) != *(u32 *)vector_get(substr, j))
					break ;
			if (j == sslen)
				return i;
		}
	}
	return RL42STR_SUBSTR_NOT_FOUND;
}
