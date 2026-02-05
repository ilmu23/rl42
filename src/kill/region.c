// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<region.c>>

#include "internal/_kill.h"
#include "internal/_rl42.h"
#include "internal/_darray.h"

extern rl42_state	state_flags;

rl42_mark	kill_start;
rl42_mark	kill_end;

u8	kill_region_internal(rl42_line *line) {
	darray	killed;
	size_t	len;

	if (!kill_start.set || !kill_end.set || kill_start.pos >= kill_end.pos)
		return 1;
	len = kill_end.pos - kill_start.pos;
	if (~state_flags & STATE_KILL_DONT_UPDATE_RING) {
		killed = darray(u32, len, NULL);
		if (!killed)
			return 0;
		if (!kill_add_to_ring(killed)) {
			darray_delete(killed);
			return 0;
		}
	}
	if (~state_flags & STATE_KILL_DONT_UPDATE_RING) do {
		__dar_psh(killed, darray_get(line->line, kill_start.pos));
		darray_erase(line->line, kill_start.pos);
	} while (--len); else do
		darray_erase(line->line, kill_start.pos);
	while (--len);
	return 1;
}

u8	kill_copy_region(rl42_line *line) {
	darray	copy;
	size_t	len;
	size_t	i;

	if (!kill_start.set || !kill_end.set || kill_start.pos >= kill_end.pos)
		return 1;
	len = kill_end.pos - kill_start.pos;
	copy = darray(u32, len, NULL);
	if (!copy)
		return 0;
	if (!kill_add_to_ring(copy)) {
		darray_delete(copy);
		return 0;
	}
	for (i = 0; len--; i++)
		__dar_psh(copy, darray_get(line->line, kill_start.pos + i));
	return 1;
}
