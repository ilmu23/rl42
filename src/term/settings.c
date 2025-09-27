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
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "internal/_kb.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_terminfo.h"

typedef struct {
	const char	*val;
	u16			name;
}	escape;

static struct {
	const char	*kbs;	// Backspace
	escape		kf1;	// F-1
	escape		kf2;	// F-2
	escape		kf3;	// F-3
	escape		kf4;	// F-4
	escape		kf5;	// F-5
	escape		kf6;	// F-6
	escape		kf7;	// F-7
	escape		kf8;	// F-8
	escape		kf9;	// F-9
	escape		kf10;	// F-10
	escape		kf11;	// F-11
	escape		kf12;	// F-12
	escape		kcuu1;	// Up
	escape		kcud1;	// Down
	escape		kcub1;	// Left
	escape		kcuf1;	// Right
	escape		kich1;	// Insert
	escape		khome;	// Home
	escape		knp;	// Page-Up
	escape		kdch1;	// Delete
	escape		kend;	// End
	escape		kpp;	// Pade-Down
	escape		kri;	// Shift + Up
	escape		kind;	// Shift + Down
	escape		kLFT;	// Shift + Left
	escape		kRIT;	// Shift + Right
	escape		kIC;	// Shift + Insert
	escape		kHOM;	// Shift + Home
	escape		kNXT;	// Shift + Page-Up
	escape		kDC;	// Shift + Delete
	escape		kEND;	// Shift + End
	escape		kPRV;	// Shift + Page-Down
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
			if (esc_seqs.rmkx != TI_ABS_STR)
				rv = (write(1, esc_seqs.rmkx, strlen(esc_seqs.rmkx)) != -1) ? 1 : 0;
			break ;
		case TERM_SETTINGS_RL42:
			rv = (tcsetattr(0, TCSANOW, &new) != -1) ? 1 : 0;
			if (esc_seqs.smkx != TI_ABS_STR)
				rv = (write(1, esc_seqs.smkx, strlen(esc_seqs.smkx)) != -1) ? 1 : 0;
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
			return key_seqs.kf1.val;
		case ti_kf2:
			return key_seqs.kf2.val;
		case ti_kf3:
			return key_seqs.kf3.val;
		case ti_kf4:
			return key_seqs.kf4.val;
		case ti_kf5:
			return key_seqs.kf5.val;
		case ti_kf6:
			return key_seqs.kf6.val;
		case ti_kf7:
			return key_seqs.kf7.val;
		case ti_kf8:
			return key_seqs.kf8.val;
		case ti_kf9:
			return key_seqs.kf9.val;
		case ti_kf10:
			return key_seqs.kf10.val;
		case ti_kf11:
			return key_seqs.kf11.val;
		case ti_kf12:
			return key_seqs.kf12.val;
		case ti_kcuu1:
			return key_seqs.kcuu1.val;
		case ti_kcud1:
			return key_seqs.kcud1.val;
		case ti_kcub1:
			return key_seqs.kcub1.val;
		case ti_kcuf1:
			return key_seqs.kcuf1.val;
		case ti_kich1:
			return key_seqs.kich1.val;
		case ti_khome:
			return key_seqs.khome.val;
		case ti_knp:
			return key_seqs.knp.val;
		case ti_kdch1:
			return key_seqs.kdch1.val;
		case ti_kend:
			return key_seqs.kend.val;
		case ti_kpp:
			return key_seqs.kpp.val;
		case ti_kri:
			return key_seqs.kri.val;
		case ti_kind:
			return key_seqs.kind.val;
		case ti_kLFT:
			return key_seqs.kLFT.val;
		case ti_kRIT:
			return key_seqs.kRIT.val;
		case ti_kIC:
			return key_seqs.kIC.val;
		case ti_kHOM:
			return key_seqs.kHOM.val;
		case ti_kNXT:
			return key_seqs.kNXT.val;
		case ti_kDC:
			return key_seqs.kDC.val;
		case ti_kEND:
			return key_seqs.kEND.val;
		case ti_kPRV:
			return key_seqs.kPRV.val;
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

u16	term_match_key_seq(const char *seq) {
	enum { CSI, SS3 }	seq_type;
	const escape		*esc;
	size_t				len;

	seq_type = (seq[1] == '[') ? CSI : SS3;
	if (seq_type == CSI) for (len = 2; seq[len]; len++) {
		if (in_range((u8)seq[len], 0x40U, 0x7EU)) {
			len++;
			break ;
		}
	} else
		len = 3;
	esc = &key_seqs.kf1;
	do {
		if (strncmp(seq, esc->val, len) == 0)
			return esc->name;
		esc++;
	} while (esc != &key_seqs.kPRV);
	return 0;
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
	key_seqs.kf1 = (escape){ .name = ti_kf1, .val = ti_getstr(ti_kf1) };
	key_seqs.kf2 = (escape){ .name = ti_kf2, .val = ti_getstr(ti_kf2) };
	key_seqs.kf3 = (escape){ .name = ti_kf3, .val = ti_getstr(ti_kf3) };
	key_seqs.kf4 = (escape){ .name = ti_kf4, .val = ti_getstr(ti_kf4) };
	key_seqs.kf5 = (escape){ .name = ti_kf5, .val = ti_getstr(ti_kf5) };
	key_seqs.kf6 = (escape){ .name = ti_kf6, .val = ti_getstr(ti_kf6) };
	key_seqs.kf7 = (escape){ .name = ti_kf7, .val = ti_getstr(ti_kf7) };
	key_seqs.kf8 = (escape){ .name = ti_kf8, .val = ti_getstr(ti_kf8) };
	key_seqs.kf9 = (escape){ .name = ti_kf9, .val = ti_getstr(ti_kf9) };
	key_seqs.kf10 = (escape){ .name = ti_kf10, .val = ti_getstr(ti_kf10) };
	key_seqs.kf11 = (escape){ .name = ti_kf11, .val = ti_getstr(ti_kf11) };
	key_seqs.kf12 = (escape){ .name = ti_kf12, .val = ti_getstr(ti_kf12) };
	key_seqs.kcuu1 = (escape){ .name = ti_kcuu1, .val = ti_getstr(ti_kcuu1) };
	key_seqs.kcud1 = (escape){ .name = ti_kcud1, .val = ti_getstr(ti_kcud1) };
	key_seqs.kcub1 = (escape){ .name = ti_kcub1, .val = ti_getstr(ti_kcub1) };
	key_seqs.kcuf1 = (escape){ .name = ti_kcuf1, .val = ti_getstr(ti_kcuf1) };
	key_seqs.kich1 = (escape){ .name = ti_kich1, .val = ti_getstr(ti_kich1) };
	key_seqs.khome = (escape){ .name = ti_khome, .val = ti_getstr(ti_khome) };
	key_seqs.knp = (escape){ .name = ti_knp, .val = ti_getstr(ti_knp) };
	key_seqs.kdch1 = (escape){ .name = ti_kdch1, .val = ti_getstr(ti_kdch1) };
	key_seqs.kend = (escape){ .name = ti_kend, .val = ti_getstr(ti_kend) };
	key_seqs.kpp = (escape){ .name = ti_kpp, .val = ti_getstr(ti_kpp) };
	key_seqs.kri = (escape){ .name = ti_kri, .val = ti_getstr(ti_kri) };
	key_seqs.kind = (escape){ .name = ti_kind, .val = ti_getstr(ti_kind) };
	key_seqs.kLFT = (escape){ .name = ti_kLFT, .val = ti_getstr(ti_kLFT) };
	key_seqs.kRIT = (escape){ .name = ti_kRIT, .val = ti_getstr(ti_kRIT) };
	key_seqs.kIC = (escape){ .name = ti_kIC, .val = ti_getstr(ti_kIC) };
	key_seqs.kHOM = (escape){ .name = ti_kHOM, .val = ti_getstr(ti_kHOM) };
	key_seqs.kNXT = (escape){ .name = ti_kNXT, .val = ti_getstr(ti_kNXT) };
	key_seqs.kDC = (escape){ .name = ti_kDC, .val = ti_getstr(ti_kDC) };
	key_seqs.kEND = (escape){ .name = ti_kEND, .val = ti_getstr(ti_kEND) };
	key_seqs.kPRV = (escape){ .name = ti_kPRV, .val = ti_getstr(ti_kPRV) };
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
