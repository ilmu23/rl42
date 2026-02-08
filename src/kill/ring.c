// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ring.c>>

#define __RL42_INTERNAL
#include "internal/_data.h"

static list	ring;

static void	_free_text(cdarray *text) {
	darray_delete((darray)*text);
}

u8	kill_add_to_ring(cdarray text) {
	if (!ring) {
		ring = list(cdarray, 4, (void (*)(void *))_free_text);
		if (!ring)
			return 0;
	}
	return list_push_front(ring, text);
}

cdarray	kill_get_top_of_ring(void) {
	return (ring) ? *(cdarray *)(list_first(ring)->data) : NULL;
}

u8	kill_rotate_ring(void) {
	if (ring && list_size(ring) > 1)
		list_move_after(ring, list_last(ring), list_first(ring));
	return 1;
}

void	kill_clear_ring(void) {
	list_delete(ring);
	ring = NULL;
}
