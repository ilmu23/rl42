// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<settings.c>>

#include "internal/_term.h"

term_settings	old;
term_settings	new;

u8	term_apply_settings(const u8 settings) {
	u8	rv;

	switch (settings) {
		case TERM_SETTINGS_DEFAULT:
			rv = (tcsetattr(0, TCSANOW, &old) != -1) ? 1 : 0;
			break ;
		case TERM_SETTINGS_RL42:
			rv = (tcsetattr(0, TCSANOW, &new) != -1) ? 1 : 0;
			break ;
		default:
			rv = 0;
	}
	return rv;
}
