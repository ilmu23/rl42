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

u8	kill_region(rl42_line *line) {
	vector	killed;
	size_t	len;

	if (!kill_start.set || !kill_end.set || kill_start.pos >= kill_end.pos)
		return 1;
	len = kill_end.pos - kill_start.pos;
	if (~state_flags & STATE_KILL_DONT_UPDATE_RING) {
		killed = vector(u32, len, NULL);
		if (!killed)
			return 0;
	}
	while (len--)
		vector_erase(line->line, kill_start.pos);
	if (~state_flags & STATE_KILL_DONT_UPDATE_RING)
		vector_delete(killed); // TODO: Add killed to kill ring
	return 1;
}
