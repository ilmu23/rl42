// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<settings.c>>

#include <signal.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "internal/_kb.h"
#include "internal/_term.h"

term_settings	old;
term_settings	new;

u16	term_width;
u16	term_height;

static void	_update_window_size(i32 sig);

u8	term_init(void) {
	struct sigaction	action;

	if (tcgetattr(0, &old) == -1)
		return 0;
	new = old;
	new.c_iflag &= ~(ICRNL | IXON);
	new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	if (!init_kb_listener())
		return 0;
	memset(&action, 0, sizeof(action));
	action.sa_handler = _update_window_size;
	if (sigaction(SIGWINCH, &action, NULL) == -1)
		return 0;
	if (!term_apply_settings(TERM_SETTINGS_RL42))
		return 0;
	_update_window_size(0);
	return term_apply_settings(TERM_SETTINGS_DEFAULT);
}

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

static void	_update_window_size([[maybe_unused]] i32 sig) {
	struct winsize	win_size;

	if (ioctl(1, TIOCGWINSZ, &win_size) != -1) {
		term_height = win_size.ws_row;
		term_width = win_size.ws_col;
	}
}
