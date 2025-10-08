// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<ring.c>>

#include "internal/_list.h"
#include "internal/_vector.h"

static list	ring;

static void	_free_text(cvector *text) {
	vector_delete((vector)*text);
}

u8	kill_add_to_ring(cvector text) {
	if (!ring) {
		ring = list(cvector, 4, (void (*)(void *))_free_text);
		if (!ring)
			return 0;
	}
	return list_push_front(ring, text);
}

cvector	kill_get_top_of_ring(void) {
	return (ring) ? *(cvector *)(list_first(ring)->data) : NULL;
}

u8	kill_rotate_ring(void) {
	list_node	node;

	if (ring) {
		node = list_first(ring);
		if (!list_push_back(ring, *(cvector *)(node->data)))
			return 0;
		list_pop_front(ring);
	}
	return 1;
}
