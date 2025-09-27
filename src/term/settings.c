// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<settings.c>>

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "internal/_kb.h"
#include "internal/_term.h"
#include "internal/_terminfo.h"

static struct {
	const char	*kbs;	// Backspace
	const char	*kf1;	// F-1
	const char	*kf2;	// F-2
	const char	*kf3;	// F-3
	const char	*kf4;	// F-4
	const char	*kf5;	// F-5
	const char	*kf6;	// F-6
	const char	*kf7;	// F-7
	const char	*kf8;	// F-8
	const char	*kf9;	// F-9
	const char	*kf10;	// F-10
	const char	*kf11;	// F-11
	const char	*kf12;	// F-12
	const char	*kcuu1;	// Up
	const char	*kcud1;	// Down
	const char	*kcub1;	// Left
	const char	*kcuf1;	// Right
	const char	*kich1;	// Insert
	const char	*khome;	// Home
	const char	*knp;	// Page-Up
	const char	*kdch1;	// Delete
	const char	*kend;	// End
	const char	*kpp;	// Pade-Down
	const char	*kri;	// Shift + Up
	const char	*kind;	// Shift + Down
	const char	*kLFT;	// Shift + Left
	const char	*kRIT;	// Shift + Right
	const char	*kIC;	// Shift + Insert
	const char	*kHOM;	// Shift + Home
	const char	*kNXT;	// Shift + Page-Up
	const char	*kDC;	// Shift + Delete
	const char	*kEND;	// Shift + End
	const char	*kPRV;	// Shift + Page-Down
}	key_seqs;

static struct {
	const char	*cup;	// Move cursor to row #1 and col #2
	const char	*bel;	// Ring the bell
	const char	*flash;	// Visual bell
	const char	*civis;	// Hide cursor
	const char	*cnorm;	// Show cursor
	const char	*smkx;	// Enter keyboard transmit mode
	const char	*rmkx;	// Leave keyboard transmit mode
	const char	*el1;	// Clear to start of line
	const char	*el;	// Clear to end of line
	const char	*ed;	// Clear to end of screen
	const char	*clear;	// Clear whole screen
	const char	*indn;	// Scroll up #1 lines
	const char	*rin;	// Scroll down #1 lines
	const char	*bold;	// Turn on bold mode
	const char	*dim;	// Turn on half-bright mode
	const char	*rev;	// Turn on reverse video
	const char	*sitm;	// Turn on italic mode
	const char	*ritm;	// Turn off italic mode
	const char	*smso;	// Turn on standout mode
	const char	*rmso;	// Turn off standout mode
	const char	*smul;	// Turn on underline mode
	const char	*rmul;	// Turn off underline mode
	const char	*setaf;	// Set foreground color to #1
	const char	*setab;	// Set background color to #1
	const char	*op;	// Set default pair to its original value
	const char	*sgr;	// Define video attributes #1 - #9
	const char	*sgr0;	// Turn off all attributes
}	esc_seqs;

term_settings	old;
term_settings	new;

u16	term_width;
u16	term_height;

static inline u8	_init_seqs(void);

static void	_update_window_size(i32 sig);

u8	term_init(void) {
	struct sigaction	action;

	if (!_init_seqs())
		return 0;
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

const char	*term_get_seq(const u16 name) {
	switch (name) {
		case ti_kbs:
			return key_seqs.kbs;
		case ti_kf1:
			return key_seqs.kf1;
		case ti_kf2:
			return key_seqs.kf2;
		case ti_kf3:
			return key_seqs.kf3;
		case ti_kf4:
			return key_seqs.kf4;
		case ti_kf5:
			return key_seqs.kf5;
		case ti_kf6:
			return key_seqs.kf6;
		case ti_kf7:
			return key_seqs.kf7;
		case ti_kf8:
			return key_seqs.kf8;
		case ti_kf9:
			return key_seqs.kf9;
		case ti_kf10:
			return key_seqs.kf10;
		case ti_kf11:
			return key_seqs.kf11;
		case ti_kf12:
			return key_seqs.kf12;
		case ti_kcuu1:
			return key_seqs.kcuu1;
		case ti_kcud1:
			return key_seqs.kcud1;
		case ti_kcub1:
			return key_seqs.kcub1;
		case ti_kcuf1:
			return key_seqs.kcuf1;
		case ti_kich1:
			return key_seqs.kich1;
		case ti_khome:
			return key_seqs.khome;
		case ti_knp:
			return key_seqs.knp;
		case ti_kdch1:
			return key_seqs.kdch1;
		case ti_kend:
			return key_seqs.kend;
		case ti_kpp:
			return key_seqs.kpp;
		case ti_kri:
			return key_seqs.kri;
		case ti_kind:
			return key_seqs.kind;
		case ti_kLFT:
			return key_seqs.kLFT;
		case ti_kRIT:
			return key_seqs.kRIT;
		case ti_kIC:
			return key_seqs.kIC;
		case ti_kHOM:
			return key_seqs.kHOM;
		case ti_kNXT:
			return key_seqs.kNXT;
		case ti_kDC:
			return key_seqs.kDC;
		case ti_kEND:
			return key_seqs.kEND;
		case ti_kPRV:
			return key_seqs.kPRV;
		case ti_cup:
			return esc_seqs.cup;
		case ti_bel:
			return esc_seqs.bel;
		case ti_flash:
			return esc_seqs.flash;
		case ti_civis:
			return esc_seqs.civis;
		case ti_cnorm:
			return esc_seqs.cnorm;
		case ti_smkx:
			return esc_seqs.smkx;
		case ti_rmkx:
			return esc_seqs.rmkx;
		case ti_el1:
			return esc_seqs.el1;
		case ti_el:
			return esc_seqs.el;
		case ti_ed:
			return esc_seqs.ed;
		case ti_clear:
			return esc_seqs.clear;
		case ti_indn:
			return esc_seqs.indn;
		case ti_rin:
			return esc_seqs.rin;
		case ti_bold:
			return esc_seqs.bold;
		case ti_dim:
			return esc_seqs.dim;
		case ti_rev:
			return esc_seqs.rev;
		case ti_sitm:
			return esc_seqs.sitm;
		case ti_ritm:
			return esc_seqs.ritm;
		case ti_smso:
			return esc_seqs.smso;
		case ti_rmso:
			return esc_seqs.rmso;
		case ti_smul:
			return esc_seqs.smul;
		case ti_rmul:
			return esc_seqs.rmul;
		case ti_setaf:
			return esc_seqs.setaf;
		case ti_setab:
			return esc_seqs.setab;
		case ti_op:
			return esc_seqs.op;
		case ti_sgr:
			return esc_seqs.sgr;
		case ti_sgr0:
			return esc_seqs.sgr0;
	}
	return NULL;
}

// TODO: add implementations once ti_tparm exists

u8	term_set_fg_color([[maybe_unused]] const u8 color) {
	return 0;
}

u8	term_set_bg_color([[maybe_unused]] const u8 color) {
	return 0;
}

u8	term_set_sgr([[maybe_unused]] const sgr_opts opts) {
	return 0;
}

static inline u8	_init_seqs(void) {
	const char	*term;

	term = getenv("TERM");
	if (!ti_load((term) ? term : "dumb"))
		return 0;
	key_seqs.kbs = ti_getstr(ti_kbs);
	key_seqs.kf1 = ti_getstr(ti_kf1);
	key_seqs.kf2 = ti_getstr(ti_kf2);
	key_seqs.kf3 = ti_getstr(ti_kf3);
	key_seqs.kf4 = ti_getstr(ti_kf4);
	key_seqs.kf5 = ti_getstr(ti_kf5);
	key_seqs.kf6 = ti_getstr(ti_kf6);
	key_seqs.kf7 = ti_getstr(ti_kf7);
	key_seqs.kf8 = ti_getstr(ti_kf8);
	key_seqs.kf9 = ti_getstr(ti_kf9);
	key_seqs.kf10 = ti_getstr(ti_kf10);
	key_seqs.kf11 = ti_getstr(ti_kf11);
	key_seqs.kf12 = ti_getstr(ti_kf12);
	key_seqs.kcuu1 = ti_getstr(ti_kcuu1);
	key_seqs.kcud1 = ti_getstr(ti_kcud1);
	key_seqs.kcub1 = ti_getstr(ti_kcub1);
	key_seqs.kcuf1 = ti_getstr(ti_kcuf1);
	key_seqs.kich1 = ti_getstr(ti_kich1);
	key_seqs.khome = ti_getstr(ti_khome);
	key_seqs.knp = ti_getstr(ti_knp);
	key_seqs.kdch1 = ti_getstr(ti_kdch1);
	key_seqs.kend = ti_getstr(ti_kend);
	key_seqs.kpp = ti_getstr(ti_kpp);
	key_seqs.kri = ti_getstr(ti_kri);
	key_seqs.kind = ti_getstr(ti_kind);
	key_seqs.kLFT = ti_getstr(ti_kLFT);
	key_seqs.kRIT = ti_getstr(ti_kRIT);
	key_seqs.kIC = ti_getstr(ti_kIC);
	key_seqs.kHOM = ti_getstr(ti_kHOM);
	key_seqs.kNXT = ti_getstr(ti_kNXT);
	key_seqs.kDC = ti_getstr(ti_kDC);
	key_seqs.kEND = ti_getstr(ti_kEND);
	key_seqs.kPRV = ti_getstr(ti_kPRV);
	esc_seqs.cup = ti_getstr(ti_cup);
	esc_seqs.bel = ti_getstr(ti_bel);
	esc_seqs.flash = ti_getstr(ti_flash);
	esc_seqs.civis = ti_getstr(ti_civis);
	esc_seqs.cnorm = ti_getstr(ti_cnorm);
	esc_seqs.smkx = ti_getstr(ti_smkx);
	esc_seqs.rmkx = ti_getstr(ti_rmkx);
	esc_seqs.el1 = ti_getstr(ti_el1);
	esc_seqs.el = ti_getstr(ti_el);
	esc_seqs.ed = ti_getstr(ti_ed);
	esc_seqs.clear = ti_getstr(ti_clear);
	esc_seqs.indn = ti_getstr(ti_indn);
	esc_seqs.rin = ti_getstr(ti_rin);
	esc_seqs.bold = ti_getstr(ti_bold);
	esc_seqs.dim = ti_getstr(ti_dim);
	esc_seqs.rev = ti_getstr(ti_rev);
	esc_seqs.sitm = ti_getstr(ti_sitm);
	esc_seqs.ritm = ti_getstr(ti_ritm);
	esc_seqs.smso = ti_getstr(ti_smso);
	esc_seqs.rmso = ti_getstr(ti_rmso);
	esc_seqs.smul = ti_getstr(ti_smul);
	esc_seqs.rmul = ti_getstr(ti_rmul);
	esc_seqs.setaf = ti_getstr(ti_setaf);
	esc_seqs.setab = ti_getstr(ti_setab);
	esc_seqs.op = ti_getstr(ti_op);
	esc_seqs.sgr = ti_getstr(ti_sgr);
	esc_seqs.sgr0 = ti_getstr(ti_sgr0);
	return 1;
}

static void	_update_window_size([[maybe_unused]] i32 sig) {
	struct winsize	win_size;

	if (ioctl(1, TIOCGWINSZ, &win_size) != -1) {
		term_height = win_size.ws_row;
		term_width = win_size.ws_col;
	}
}
