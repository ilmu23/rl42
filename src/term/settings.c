// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<settings.c>>

#if __STDC_VERSION__ < 202311L
# include <bsd/string.h>
#endif

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "rl42.h"

#include "internal/_kb.h"
#include "internal/_defs.h"
#include "internal/_term.h"
#include "internal/_utils.h"

#ifdef __RL42_USE_INTERNAL_TERMINFO
#include "internal/_terminfo.h"
#else
#include <ti42/ti42.h>
#endif

#define _TERM_SCROLL_UP		"\x1b[%p1%dS"
#define _TERM_SCROLL_DOWN	"\x1b[%p1%dT"
#define _TERM_MOVE_CURSOR	"\x1b[%i%p1%d;%p2%dH"

#define _TERM_BPM_ON		"\x1b[?2004h"
#define _TERM_BPM_OFF	"\x1b[?2004l"

#define sgr_opt(n)	((opts >> (n - 1)) & 0x1U)

typedef struct termios	term_settings;

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

extern const char	*scroll_up;
extern const char	*scroll_down;
extern const char	*move_cursor;

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
	scroll_up = (esc_seqs.indn) ? esc_seqs.indn : _TERM_SCROLL_UP;
	scroll_down = (esc_seqs.rin) ? esc_seqs.rin : _TERM_SCROLL_DOWN;
	move_cursor = (esc_seqs.cup) ? esc_seqs.cup : _TERM_MOVE_CURSOR;
	return term_apply_settings(TERM_SETTINGS_DEFAULT);
}

u8	term_apply_settings(const u8 settings) {
	u8	rv;

	switch (settings) {
		case TERM_SETTINGS_DEFAULT:
			rv = (tcsetattr(0, TCSANOW, &old) != -1) ? 1 : 0;
			if (esc_seqs.rmkx != TI42_ABS_STR)
				rv |= (ti42_tputs(esc_seqs.rmkx, 1, term_putchar_unbuffered)) ? 1 : 0;
			if (esc_seqs.cnorm != TI42_ABS_STR)
				rv |= term_show_cursor();
			break ;
		case TERM_SETTINGS_RL42:
			rv = (tcsetattr(0, TCSANOW, &new) != -1) ? 1 : 0;
			if (esc_seqs.smkx != TI42_ABS_STR)
				rv |= (ti42_tputs(esc_seqs.smkx, 1, term_putchar_unbuffered)) ? 1 : 0;
			if (esc_seqs.civis != TI42_ABS_STR)
				rv |= term_hide_cursor();
			break ;
		default:
			rv = 0;
	}
	return rv;
}

u8	term_set_bpm(const rl42_bpm_state state) {
	static u8	bpm_enabled = 0;

	switch (state) {
		case BPM_ENABLED:
			if (!bpm_enabled && write(1, _TERM_BPM_ON, strl_len(_TERM_BPM_ON)) != (ssize_t)strl_len(_TERM_BPM_ON))
				return 0;
			bpm_enabled = 1;
			return 1;
		case BPM_DISABLED:
			if (bpm_enabled && write(1, _TERM_BPM_OFF, strl_len(_TERM_BPM_OFF)) != (ssize_t)strl_len(_TERM_BPM_OFF))
				return 0;
			bpm_enabled = 0;
			return 1;
		case BPM_TOGGLE:
			if ((bpm_enabled) ? write(1, _TERM_BPM_OFF, strl_len(_TERM_BPM_OFF)) != (ssize_t)strl_len(_TERM_BPM_OFF) : write(1, _TERM_BPM_ON, strl_len(_TERM_BPM_ON)) != (ssize_t)strl_len(_TERM_BPM_OFF))
				return 0;
			bpm_enabled ^= 1;
			return 1;
	}
	return 0;
}

const char	*term_get_seq(const u16 name) {
	switch (name) {
		case ti42_kbs:
			return key_seqs.kbs;
		case ti42_kf1:
			return key_seqs.kf1.val;
		case ti42_kf2:
			return key_seqs.kf2.val;
		case ti42_kf3:
			return key_seqs.kf3.val;
		case ti42_kf4:
			return key_seqs.kf4.val;
		case ti42_kf5:
			return key_seqs.kf5.val;
		case ti42_kf6:
			return key_seqs.kf6.val;
		case ti42_kf7:
			return key_seqs.kf7.val;
		case ti42_kf8:
			return key_seqs.kf8.val;
		case ti42_kf9:
			return key_seqs.kf9.val;
		case ti42_kf10:
			return key_seqs.kf10.val;
		case ti42_kf11:
			return key_seqs.kf11.val;
		case ti42_kf12:
			return key_seqs.kf12.val;
		case ti42_kcuu1:
			return key_seqs.kcuu1.val;
		case ti42_kcud1:
			return key_seqs.kcud1.val;
		case ti42_kcub1:
			return key_seqs.kcub1.val;
		case ti42_kcuf1:
			return key_seqs.kcuf1.val;
		case ti42_kich1:
			return key_seqs.kich1.val;
		case ti42_khome:
			return key_seqs.khome.val;
		case ti42_knp:
			return key_seqs.knp.val;
		case ti42_kdch1:
			return key_seqs.kdch1.val;
		case ti42_kend:
			return key_seqs.kend.val;
		case ti42_kpp:
			return key_seqs.kpp.val;
		case ti42_kri:
			return key_seqs.kri.val;
		case ti42_kind:
			return key_seqs.kind.val;
		case ti42_kLFT:
			return key_seqs.kLFT.val;
		case ti42_kRIT:
			return key_seqs.kRIT.val;
		case ti42_kIC:
			return key_seqs.kIC.val;
		case ti42_kHOM:
			return key_seqs.kHOM.val;
		case ti42_kNXT:
			return key_seqs.kNXT.val;
		case ti42_kDC:
			return key_seqs.kDC.val;
		case ti42_kEND:
			return key_seqs.kEND.val;
		case ti42_kPRV:
			return key_seqs.kPRV.val;
		case ti42_cup:
			return esc_seqs.cup;
		case ti42_bel:
			return esc_seqs.bel;
		case ti42_flash:
			return esc_seqs.flash;
		case ti42_civis:
			return esc_seqs.civis;
		case ti42_cnorm:
			return esc_seqs.cnorm;
		case ti42_smkx:
			return esc_seqs.smkx;
		case ti42_rmkx:
			return esc_seqs.rmkx;
		case ti42_el1:
			return esc_seqs.el1;
		case ti42_el:
			return esc_seqs.el;
		case ti42_ed:
			return esc_seqs.ed;
		case ti42_clear:
			return esc_seqs.clear;
		case ti42_indn:
			return esc_seqs.indn;
		case ti42_rin:
			return esc_seqs.rin;
		case ti42_bold:
			return esc_seqs.bold;
		case ti42_dim:
			return esc_seqs.dim;
		case ti42_rev:
			return esc_seqs.rev;
		case ti42_sitm:
			return esc_seqs.sitm;
		case ti42_ritm:
			return esc_seqs.ritm;
		case ti42_smso:
			return esc_seqs.smso;
		case ti42_rmso:
			return esc_seqs.rmso;
		case ti42_smul:
			return esc_seqs.smul;
		case ti42_rmul:
			return esc_seqs.rmul;
		case ti42_setaf:
			return esc_seqs.setaf;
		case ti42_setab:
			return esc_seqs.setab;
		case ti42_op:
			return esc_seqs.op;
		case ti42_sgr:
			return esc_seqs.sgr;
		case ti42_sgr0:
			return esc_seqs.sgr0;
	}
	return NULL;
}

#define _BUFFER_SIZE	1023

const char	*term_get_hl_seq(void) {
	rl42_hl_color	hl;
	static char		buf[_BUFFER_SIZE + 1];

	if (!rl42_get(RL42_ENABLE_HIGHLIGHT).u64)
		return "";
	hl = rl42_get(RL42_HIGHLIGHT_COLOR).hlc;
	if (hl.type == RL42_HL_INDEX) {
#ifdef __RL42_USE_INTERNAL_TERMINFO
		strlcpy(buf, ti42_tparm(esc_seqs.setaf, hl.val.index), _BUFFER_SIZE + 1);
#else
		strlcpy(buf, ti42_tparm1(esc_seqs.setaf, hl.val.index), _BUFFER_SIZE + 1);
#endif
	} else
		snprintf(buf, _BUFFER_SIZE, "\x1b[38;2;%hhu;%hhu;%hhum", hl.val.rgb.r, hl.val.rgb.g, hl.val.rgb.b);
	return buf;
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

u8	term_set_fg_color(const u8 color) {
	if (esc_seqs.setaf) {
#ifdef __RL42_USE_INTERNAL_TERMINFO
		return (ti42_tputs(ti42_tparm(esc_seqs.setaf, color), 1, term_putchar_unbuffered) != -1) ? 1 : 0;
#else
		return (ti42_tputs(ti42_tparm1(esc_seqs.setaf, color), 1, term_putchar_unbuffered) != -1) ? 1 : 0;
#endif
	}
	return 0;
}

u8	term_set_bg_color(const u8 color) {
	if (esc_seqs.setab) {
#ifdef __RL42_USE_INTERNAL_TERMINFO
		return (ti42_tputs(ti42_tparm(esc_seqs.setab, color), 1, term_putchar_unbuffered) != -1) ? 1 : 0;
#else
		return (ti42_tputs(ti42_tparm1(esc_seqs.setab, color), 1, term_putchar_unbuffered) != -1) ? 1 : 0;
#endif
	}
	return 0;
}

u8	term_set_sgr(const sgr_opts opts) {
	const char	*sgr_str;

	sgr_str = (opts) ? ti42_tparm(esc_seqs.sgr, sgr_opt(1), sgr_opt(2), sgr_opt(3), sgr_opt(4), sgr_opt(5), sgr_opt(6), sgr_opt(7), sgr_opt(8), sgr_opt(9)) : esc_seqs.sgr0;
	if (sgr_str)
		return (ti42_tputs(sgr_str, 1, term_putchar_unbuffered) != -1) ? 1 : 0;
	return 0;
}

u8	term_hide_cursor(void) {
	if (esc_seqs.civis)
		return (ti42_tputs(esc_seqs.civis, 1, term_putchar_unbuffered) != -1) ? 1 : 0;
	return 0;
}

u8	term_show_cursor(void) {
	if (esc_seqs.cnorm)
		return (ti42_tputs(esc_seqs.cnorm, 1, term_putchar_unbuffered) != -1) ? 1 : 0;
	return 0;
}

static inline u8	_init_seqs(void) {
	const char	*term;

	term = rl42_getenv("TERM");
	if (!ti42_load((term) ? term : "dumb"))
		return 0;
	key_seqs.kbs = ti42_getstr(ti42_kbs);
	key_seqs.kf1 = (escape){ .name = ti42_kf1, .val = ti42_getstr(ti42_kf1) };
	key_seqs.kf2 = (escape){ .name = ti42_kf2, .val = ti42_getstr(ti42_kf2) };
	key_seqs.kf3 = (escape){ .name = ti42_kf3, .val = ti42_getstr(ti42_kf3) };
	key_seqs.kf4 = (escape){ .name = ti42_kf4, .val = ti42_getstr(ti42_kf4) };
	key_seqs.kf5 = (escape){ .name = ti42_kf5, .val = ti42_getstr(ti42_kf5) };
	key_seqs.kf6 = (escape){ .name = ti42_kf6, .val = ti42_getstr(ti42_kf6) };
	key_seqs.kf7 = (escape){ .name = ti42_kf7, .val = ti42_getstr(ti42_kf7) };
	key_seqs.kf8 = (escape){ .name = ti42_kf8, .val = ti42_getstr(ti42_kf8) };
	key_seqs.kf9 = (escape){ .name = ti42_kf9, .val = ti42_getstr(ti42_kf9) };
	key_seqs.kf10 = (escape){ .name = ti42_kf10, .val = ti42_getstr(ti42_kf10) };
	key_seqs.kf11 = (escape){ .name = ti42_kf11, .val = ti42_getstr(ti42_kf11) };
	key_seqs.kf12 = (escape){ .name = ti42_kf12, .val = ti42_getstr(ti42_kf12) };
	key_seqs.kcuu1 = (escape){ .name = ti42_kcuu1, .val = ti42_getstr(ti42_kcuu1) };
	key_seqs.kcud1 = (escape){ .name = ti42_kcud1, .val = ti42_getstr(ti42_kcud1) };
	key_seqs.kcub1 = (escape){ .name = ti42_kcub1, .val = ti42_getstr(ti42_kcub1) };
	key_seqs.kcuf1 = (escape){ .name = ti42_kcuf1, .val = ti42_getstr(ti42_kcuf1) };
	key_seqs.kich1 = (escape){ .name = ti42_kich1, .val = ti42_getstr(ti42_kich1) };
	key_seqs.khome = (escape){ .name = ti42_khome, .val = ti42_getstr(ti42_khome) };
	key_seqs.knp = (escape){ .name = ti42_knp, .val = ti42_getstr(ti42_knp) };
	key_seqs.kdch1 = (escape){ .name = ti42_kdch1, .val = ti42_getstr(ti42_kdch1) };
	key_seqs.kend = (escape){ .name = ti42_kend, .val = ti42_getstr(ti42_kend) };
	key_seqs.kpp = (escape){ .name = ti42_kpp, .val = ti42_getstr(ti42_kpp) };
	key_seqs.kri = (escape){ .name = ti42_kri, .val = ti42_getstr(ti42_kri) };
	key_seqs.kind = (escape){ .name = ti42_kind, .val = ti42_getstr(ti42_kind) };
	key_seqs.kLFT = (escape){ .name = ti42_kLFT, .val = ti42_getstr(ti42_kLFT) };
	key_seqs.kRIT = (escape){ .name = ti42_kRIT, .val = ti42_getstr(ti42_kRIT) };
	key_seqs.kIC = (escape){ .name = ti42_kIC, .val = ti42_getstr(ti42_kIC) };
	key_seqs.kHOM = (escape){ .name = ti42_kHOM, .val = ti42_getstr(ti42_kHOM) };
	key_seqs.kNXT = (escape){ .name = ti42_kNXT, .val = ti42_getstr(ti42_kNXT) };
	key_seqs.kDC = (escape){ .name = ti42_kDC, .val = ti42_getstr(ti42_kDC) };
	key_seqs.kEND = (escape){ .name = ti42_kEND, .val = ti42_getstr(ti42_kEND) };
	key_seqs.kPRV = (escape){ .name = ti42_kPRV, .val = ti42_getstr(ti42_kPRV) };
	esc_seqs.cup = ti42_getstr(ti42_cup);
	esc_seqs.bel = ti42_getstr(ti42_bel);
	esc_seqs.flash = ti42_getstr(ti42_flash);
	esc_seqs.civis = ti42_getstr(ti42_civis);
	esc_seqs.cnorm = ti42_getstr(ti42_cnorm);
	esc_seqs.smkx = ti42_getstr(ti42_smkx);
	esc_seqs.rmkx = ti42_getstr(ti42_rmkx);
	esc_seqs.el1 = ti42_getstr(ti42_el1);
	esc_seqs.el = ti42_getstr(ti42_el);
	esc_seqs.ed = ti42_getstr(ti42_ed);
	esc_seqs.clear = ti42_getstr(ti42_clear);
	esc_seqs.indn = ti42_getstr(ti42_indn);
	esc_seqs.rin = ti42_getstr(ti42_rin);
	esc_seqs.bold = ti42_getstr(ti42_bold);
	esc_seqs.dim = ti42_getstr(ti42_dim);
	esc_seqs.rev = ti42_getstr(ti42_rev);
	esc_seqs.sitm = ti42_getstr(ti42_sitm);
	esc_seqs.ritm = ti42_getstr(ti42_ritm);
	esc_seqs.smso = ti42_getstr(ti42_smso);
	esc_seqs.rmso = ti42_getstr(ti42_rmso);
	esc_seqs.smul = ti42_getstr(ti42_smul);
	esc_seqs.rmul = ti42_getstr(ti42_rmul);
	esc_seqs.setaf = ti42_getstr(ti42_setaf);
	esc_seqs.setab = ti42_getstr(ti42_setab);
	esc_seqs.op = ti42_getstr(ti42_op);
	esc_seqs.sgr = ti42_getstr(ti42_sgr);
	esc_seqs.sgr0 = ti42_getstr(ti42_sgr0);
	return 1;
}

static void	_update_window_size([[maybe_unused]] i32 sig) {
	struct winsize	win_size;

	if (ioctl(1, TIOCGWINSZ, &win_size) != -1) {
		term_height = win_size.ws_row;
		term_width = win_size.ws_col;
	}
}
