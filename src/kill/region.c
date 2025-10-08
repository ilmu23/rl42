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
#include "internal/_vector.h"

extern rl42_state	state_flags;

rl42_mark	kill_start;
rl42_mark	kill_end;

u8	kill_region_internal(rl42_line *line) {
	vector	killed;
	size_t	len;

	if (!kill_start.set || !kill_end.set || kill_start.pos >= kill_end.pos)
		return 1;
	len = kill_end.pos - kill_start.pos;
	if (~state_flags & STATE_KILL_DONT_UPDATE_RING) {
		killed = vector(u32, len, NULL);
		if (!killed)
			return 0;
		if (!kill_add_to_ring(killed)) {
			vector_delete(killed);
			return 0;
		}
	}
	if (~state_flags & STATE_KILL_DONT_UPDATE_RING) do {
		__vec_psh(killed, vector_get(line->line, kill_start.pos));
		vector_erase(line->line, kill_start.pos);
	} while (--len); else do
		vector_erase(line->line, kill_start.pos);
	while (--len);
	return 1;
}
