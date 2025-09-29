// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<keyseq.c>>

#include <string.h>

#include "internal/_kb.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_hashes.h"
#include "internal/_vector.h"
#include "internal/_keybinds.h"
#include "internal/_terminfo_caps.h"

#define _INVALID_ESCAPE	UINT32_MAX

typedef struct {
	const char	*str;
	u32			ucp;
}	escape;

static const char	*nul = "\x1b\x0";
static const char	*soh = "\x1b\x1";
static const char	*stx = "\x1b\x2";
static const char	*etx = "\x1b\x3";
static const char	*eot = "\x1b\x4";
static const char	*enq = "\x1b\x5";
static const char	*ack = "\x1b\x6";
static const char	*bel = "\x1b\x7";
static const char	*bs  = "\x1b\x8";
static const char	*ht  = "\x1b\x9";
static const char	*lf  = "\x1b\xa";
static const char	*vt  = "\x1b\xb";
static const char	*ff  = "\x1b\xc";
static const char	*cr  = "\x1b\xd";
static const char	*so  = "\x1b\xe";
static const char	*si  = "\x1b\xf";
static const char	*dle = "\x1b\x10";
static const char	*dc1 = "\x1b\x11";
static const char	*dc2 = "\x1b\x12";
static const char	*dc3 = "\x1b\x13";
static const char	*dc4 = "\x1b\x14";
static const char	*nak = "\x1b\x15";
static const char	*syn = "\x1b\x16";
static const char	*etb = "\x1b\x17";
static const char	*can = "\x1b\x18";
static const char	*em  = "\x1b\x19";
static const char	*sub = "\x1b\x1a";
static const char	*fs  = "\x1b\x1c";
static const char	*gs  = "\x1b\x1d";
static const char	*rs  = "\x1b\x1e";
static const char	*us  = "\x1b\x1f";
static const char	*bck = "\x1b\x7f";

extern u32	kcbs;

static inline escape	_match_escape(const char *seq, size_t len);

vector	expand_seq(const char *seq) {
	vector	out;
	escape	tmp;
	size_t	i;
	size_t	j;
	u8		esc;

	out = vector(u32, 256, NULL);
	if (!out)
		return NULL;
	for (i = j = esc = 0, tmp.ucp = 0; seq[i]; j = ++i, tmp.ucp = 0) {
		if (seq[i] == '\\' && !esc) {
			esc = 1;
			continue ;
		}
		if (seq[i] == '<' && !esc) {
			i++;
			while (seq[i] && (seq[i] != '>' || esc))
				if (seq[i++] == '\\')
					esc ^= 1;
			if (!seq[i])
				goto err;
			tmp = _match_escape(&seq[j], i - j + 1);
			if (tmp.str) do {
				tmp.ucp = utf8_decode(tmp.str);
				if (!vector_push(out, tmp.ucp))
					goto err;
				tmp.str += charsize_utf8(*tmp.str);
			} while (*tmp.str);
			else if (!tmp.ucp || !vector_push(out, tmp.ucp))
				goto err;
		} else {
			tmp.ucp = utf8_decode(&seq[i]);
			if (!vector_push(out, tmp.ucp))
				goto err;
			if (tmp.ucp > 0xFFFFU)
				i += 3;
			else if (tmp.ucp > 0x7FFU)
				i += 2;
			else if (tmp.ucp > 0x7FU)
				i++;
		}
	}
	return out;
err:
	vector_delete(out);
	return EXPAND_INVALID_SEQ;
}

static inline escape	_match_escape(const char *seq, size_t len) {
	size_t	i;
	char	esc[16 + 1];
	u64		hash;

	if (len > 16)
		return (escape){.str = NULL, .ucp = _INVALID_ESCAPE};
	memcpy(esc, seq, len);
	esc[len] = '\0';
	for (i = 0; i < len; i++)
		if (esc[i] == '\\')
			memmove(&esc[i], &esc[i + 1], len-- - i);
	hash = cstr_hash(esc, 347, UINT64_MAX);
	switch (hash) {
		case KEY_C_AT_HASH:
			return (escape){.str = &nul[1]};
		case KEY_C_A_HASH:
			return (escape){.str = &soh[1]};
		case KEY_C_B_HASH:
			return (escape){.str = &stx[1]};
		case KEY_C_C_HASH:
			return (escape){.str = &etx[1]};
		case KEY_C_D_HASH:
			return (escape){.str = &eot[1]};
		case KEY_C_E_HASH:
			return (escape){.str = &enq[1]};
		case KEY_C_F_HASH:
			return (escape){.str = &ack[1]};
		case KEY_C_G_HASH:
			return (escape){.str = &bel[1]};
		case KEY_C_H_HASH:
			return (escape){.str = &bs[1]};
		case KEY_TAB_HASH:
		case KEY_C_I_HASH:
			return (escape){.str = &ht[1]};
		case KEY_ENT_HASH:
		case KEY_C_J_HASH:
			return (escape){.str = &lf[1]};
		case KEY_C_K_HASH:
			return (escape){.str = &vt[1]};
		case KEY_C_L_HASH:
			return (escape){.str = &ff[1]};
		case KEY_RET_HASH:
		case KEY_C_M_HASH:
			return (escape){.str = &cr[1]};
		case KEY_C_N_HASH:
			return (escape){.str = &so[1]};
		case KEY_C_O_HASH:
			return (escape){.str = &si[1]};
		case KEY_C_P_HASH:
			return (escape){.str = &dle[1]};
		case KEY_C_Q_HASH:
			return (escape){.str = &dc1[1]};
		case KEY_C_R_HASH:
			return (escape){.str = &dc2[1]};
		case KEY_C_S_HASH:
			return (escape){.str = &dc3[1]};
		case KEY_C_T_HASH:
			return (escape){.str = &dc4[1]};
		case KEY_C_U_HASH:
			return (escape){.str = &nak[1]};
		case KEY_C_V_HASH:
			return (escape){.str = &syn[1]};
		case KEY_C_W_HASH:
			return (escape){.str = &etb[1]};
		case KEY_C_X_HASH:
			return (escape){.str = &can[1]};
		case KEY_C_Y_HASH:
			return (escape){.str = &em[1]};
		case KEY_C_Z_HASH:
			return (escape){.str = &sub[1]};
		case KEY_ESC_HASH:
		case KEY_C_OSBRACKET_HASH:
			return (escape){.str = "\x1b"};
		case KEY_C_BSLASH_HASH:
			return (escape){.str = &fs[1]};
		case KEY_C_CSBRACKET_HASH:
			return (escape){.str = &gs[1]};
		case KEY_C_TILDE_HASH:
			return (escape){.str = &rs[1]};
		case KEY_C_USCORE_HASH:
			return (escape){.str = &us[1]};
		case KEY_M_C_AT_HASH:
			return (escape){.str = nul};
		case KEY_M_C_A_HASH:
			return (escape){.str = soh};
		case KEY_M_C_B_HASH:
			return (escape){.str = stx};
		case KEY_M_C_C_HASH:
			return (escape){.str = etx};
		case KEY_M_C_D_HASH:
			return (escape){.str = eot};
		case KEY_M_C_E_HASH:
			return (escape){.str = enq};
		case KEY_M_C_F_HASH:
			return (escape){.str = ack};
		case KEY_M_C_G_HASH:
			return (escape){.str = bel};
		case KEY_M_C_H_HASH:
			return (escape){.str = bs};
		case KEY_M_C_I_HASH:
			return (escape){.str = ht};
		case KEY_M_C_J_HASH:
			return (escape){.str = lf};
		case KEY_M_C_K_HASH:
			return (escape){.str = vt};
		case KEY_M_C_L_HASH:
			return (escape){.str = ff};
		case KEY_M_C_M_HASH:
			return (escape){.str = cr};
		case KEY_M_C_N_HASH:
			return (escape){.str = so};
		case KEY_M_C_O_HASH:
			return (escape){.str = si};
		case KEY_M_C_P_HASH:
			return (escape){.str = dle};
		case KEY_M_C_Q_HASH:
			return (escape){.str = dc1};
		case KEY_M_C_R_HASH:
			return (escape){.str = dc2};
		case KEY_M_C_S_HASH:
			return (escape){.str = dc3};
		case KEY_M_C_T_HASH:
			return (escape){.str = dc4};
		case KEY_M_C_U_HASH:
			return (escape){.str = nak};
		case KEY_M_C_V_HASH:
			return (escape){.str = syn};
		case KEY_M_C_W_HASH:
			return (escape){.str = etb};
		case KEY_M_C_X_HASH:
			return (escape){.str = can};
		case KEY_M_C_Y_HASH:
			return (escape){.str = em};
		case KEY_M_C_Z_HASH:
			return (escape){.str = sub};
		case KEY_M_C_OSBRACKET_HASH:
			return (escape){.str = "\x1b\x1b"};
		case KEY_M_C_BSLASH_HASH:
			return (escape){.str = fs};
		case KEY_M_C_CSBRACKET_HASH:
			return (escape){.str = gs};
		case KEY_M_C_TILDE_HASH:
			return (escape){.str = rs};
		case KEY_M_C_USCORE_HASH:
			return (escape){.str = us};
		case KEY_SPC_HASH:
			return (escape){.str = " "};
		case KEY_BCK_HASH:
			if (kcbs != '\x7f')
				return (escape){.str = &bck[1]};
			return (escape){.str = &bs[1]};
		case KEY_M_SPC_HASH:
			return (escape){.str = "\x1b "};
		case KEY_M_BCK_HASH:
			if (kcbs != '\x7f')
				return (escape){.str = bck};
			return (escape){.str = bs};
		case KEY_M_BANG_HASH:
			return (escape){.str = "\x1b!"};
		case KEY_M_DQUOTE_HASH:
			return (escape){.str = "\x1b\""};
		case KEY_M_POUNG_HASH:
			return (escape){.str = "\x1b#"};
		case KEY_M_DOLLAR_HASH:
			return (escape){.str = "\x1b$"};
		case KEY_M_PERCENT_HASH:
			return (escape){.str = "\x1b%"};
		case KEY_M_AMP_HASH:
			return (escape){.str = "\x1b&"};
		case KEY_M_SQUOTE_HASH:
			return (escape){.str = "\x1b'"};
		case KEY_M_OPARENT_HASH:
			return (escape){.str = "\x1b("};
		case KEY_M_CPARENT_HASH:
			return (escape){.str = "\x1b)"};
		case KEY_M_ASTERISK_HASH:
			return (escape){.str = "\x1b*"};
		case KEY_M_PLUS_HASH:
			return (escape){.str = "\x1b+"};
		case KEY_M_COMMA_HASH:
			return (escape){.str = "\x1b,"};
		case KEY_M_MINUS_HASH:
			return (escape){.str = "\x1b-"};
		case KEY_M_PERIOD_HASH:
			return (escape){.str = "\x1b."};
		case KEY_M_SLASH_HASH:
			return (escape){.str = "\x1b/"};
		case KEY_M_0_HASH:
			return (escape){.str = "\x1b" "0"};
		case KEY_M_1_HASH:
			return (escape){.str = "\x1b" "1"};
		case KEY_M_2_HASH:
			return (escape){.str = "\x1b" "2"};
		case KEY_M_3_HASH:
			return (escape){.str = "\x1b" "3"};
		case KEY_M_4_HASH:
			return (escape){.str = "\x1b" "4"};
		case KEY_M_5_HASH:
			return (escape){.str = "\x1b" "5"};
		case KEY_M_6_HASH:
			return (escape){.str = "\x1b" "6"};
		case KEY_M_7_HASH:
			return (escape){.str = "\x1b" "7"};
		case KEY_M_8_HASH:
			return (escape){.str = "\x1b" "8"};
		case KEY_M_9_HASH:
			return (escape){.str = "\x1b" "9"};
		case KEY_M_COLON_HASH:
			return (escape){.str = "\x1b:"};
		case KEY_M_SCOLON_HASH:
			return (escape){.str = "\x1b;"};
		case KEY_M_LTHAN_HASH:
			return (escape){.str = "\x1b<"};
		case KEY_M_EQUAL_HASH:
			return (escape){.str = "\x1b="};
		case KEY_M_MTAHN_HASH:
			return (escape){.str = "\x1b>"};
		case KEY_M_QUESTION_HASH:
			return (escape){.str = "\x1b?"};
		case KEY_M_AT_HASH:
			return (escape){.str = "\x1b@"};
		case KEY_M_UP_A_HASH:
			return (escape){.str = "\x1b" "A"};
		case KEY_M_UP_B_HASH:
			return (escape){.str = "\x1b" "B"};
		case KEY_M_UP_C_HASH:
			return (escape){.str = "\x1b" "C"};
		case KEY_M_UP_D_HASH:
			return (escape){.str = "\x1b" "D"};
		case KEY_M_UP_E_HASH:
			return (escape){.str = "\x1b" "E"};
		case KEY_M_UP_F_HASH:
			return (escape){.str = "\x1b" "F"};
		case KEY_M_UP_G_HASH:
			return (escape){.str = "\x1bG"};
		case KEY_M_UP_H_HASH:
			return (escape){.str = "\x1bH"};
		case KEY_M_UP_I_HASH:
			return (escape){.str = "\x1bI"};
		case KEY_M_UP_J_HASH:
			return (escape){.str = "\x1bJ"};
		case KEY_M_UP_K_HASH:
			return (escape){.str = "\x1bK"};
		case KEY_M_UP_L_HASH:
			return (escape){.str = "\x1bL"};
		case KEY_M_UP_M_HASH:
			return (escape){.str = "\x1bM"};
		case KEY_M_UP_N_HASH:
			return (escape){.str = "\x1bN"};
		case KEY_M_UP_O_HASH:
			return (escape){.str = "\x1bO"};
		case KEY_M_UP_P_HASH:
			return (escape){.str = "\x1bP"};
		case KEY_M_UP_Q_HASH:
			return (escape){.str = "\x1bQ"};
		case KEY_M_UP_R_HASH:
			return (escape){.str = "\x1bR"};
		case KEY_M_UP_S_HASH:
			return (escape){.str = "\x1bS"};
		case KEY_M_UP_T_HASH:
			return (escape){.str = "\x1bT"};
		case KEY_M_UP_U_HASH:
			return (escape){.str = "\x1bU"};
		case KEY_M_UP_V_HASH:
			return (escape){.str = "\x1bV"};
		case KEY_M_UP_W_HASH:
			return (escape){.str = "\x1bW"};
		case KEY_M_UP_X_HASH:
			return (escape){.str = "\x1bX"};
		case KEY_M_UP_Y_HASH:
			return (escape){.str = "\x1bY"};
		case KEY_M_UP_Z_HASH:
			return (escape){.str = "\x1bZ"};
		case KEY_M_OSBRACKET_HASH:
			return (escape){.str = "\x1b["};
		case KEY_M_BSLASH_HASH:
			return (escape){.str = "\x1b\\"};
		case KEY_M_CSBRACKET_HASH:
			return (escape){.str = "\x1b]"};
		case KEY_M_CARET_HASH:
			return (escape){.str = "\x1b^"};
		case KEY_M_USCORE_HASH:
			return (escape){.str = "\x1b_"};
		case KEY_M_BTICK_HASH:
			return (escape){.str = "\x1b`"};
		case KEY_M_DN_A_HASH:
			return (escape){.str = "\x1b" "a"};
		case KEY_M_DN_B_HASH:
			return (escape){.str = "\x1b" "a"};
		case KEY_M_DN_C_HASH:
			return (escape){.str = "\x1b" "c"};
		case KEY_M_DN_D_HASH:
			return (escape){.str = "\x1b" "d"};
		case KEY_M_DN_E_HASH:
			return (escape){.str = "\x1b" "e"};
		case KEY_M_DN_F_HASH:
			return (escape){.str = "\x1b" "f"};
		case KEY_M_DN_G_HASH:
			return (escape){.str = "\x1bg"};
		case KEY_M_DN_H_HASH:
			return (escape){.str = "\x1bh"};
		case KEY_M_DN_I_HASH:
			return (escape){.str = "\x1bi"};
		case KEY_M_DN_J_HASH:
			return (escape){.str = "\x1bj"};
		case KEY_M_DN_K_HASH:
			return (escape){.str = "\x1bk"};
		case KEY_M_DN_L_HASH:
			return (escape){.str = "\x1bl"};
		case KEY_M_DN_M_HASH:
			return (escape){.str = "\x1bm"};
		case KEY_M_DN_N_HASH:
			return (escape){.str = "\x1bn"};
		case KEY_M_DN_O_HASH:
			return (escape){.str = "\x1bo"};
		case KEY_M_DN_P_HASH:
			return (escape){.str = "\x1bp"};
		case KEY_M_DN_Q_HASH:
			return (escape){.str = "\x1bq"};
		case KEY_M_DN_R_HASH:
			return (escape){.str = "\x1br"};
		case KEY_M_DN_S_HASH:
			return (escape){.str = "\x1bs"};
		case KEY_M_DN_T_HASH:
			return (escape){.str = "\x1bt"};
		case KEY_M_DN_U_HASH:
			return (escape){.str = "\x1bu"};
		case KEY_M_DN_V_HASH:
			return (escape){.str = "\x1bv"};
		case KEY_M_DN_W_HASH:
			return (escape){.str = "\x1bw"};
		case KEY_M_DN_X_HASH:
			return (escape){.str = "\x1bx"};
		case KEY_M_DN_Y_HASH:
			return (escape){.str = "\x1by"};
		case KEY_M_DN_Z_HASH:
			return (escape){.str = "\x1bz"};
		case KEY_M_OCBRACKET_HASH:
			return (escape){.str = "\x1b{"};
		case KEY_M_PIPE_HASH:
			return (escape){.str = "\x1b|"};
		case KEY_M_CCBRACKET_HASH:
			return (escape){.str = "\x1b}"};
		case KEY_M_TILDE_HASH:
			return (escape){.str = "\x1b~"};
		case KEY_F_1_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F1};
		case KEY_F_2_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F2};
		case KEY_F_3_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F3};
		case KEY_F_4_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F4};
		case KEY_F_5_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F5};
		case KEY_F_6_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F6};
		case KEY_F_7_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F7};
		case KEY_F_8_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F8};
		case KEY_F_9_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F9};
		case KEY_F_10_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F10};
		case KEY_F_11_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F11};
		case KEY_F_12_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F12};
		case KEY_UP_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_F1};
		case KEY_DOWN_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_DOWN};
		case KEY_LEFT_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_LEFT};
		case KEY_RIGHT_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_RIGHT};
		case KEY_INS_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_INSERT};
		case KEY_HME_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_HOME};
		case KEY_PGU_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_PAGE_UP};
		case KEY_DEL_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_DELETE};
		case KEY_END_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_END};
		case KEY_PGD_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_PAGE_DOWN};
		case KEY_S_UP_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_UP};
		case KEY_S_DOWN_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_DOWN};
		case KEY_S_LEFT_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_LEFT};
		case KEY_S_RIGHT_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_RIGHT};
		case KEY_S_INS_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_INSERT};
		case KEY_S_HME_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_HOME};
		case KEY_S_PGU_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_PAGE_UP};
		case KEY_S_DEL_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_DELETE};
		case KEY_S_END_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_END};
		case KEY_S_PGD_HASH:
			return (escape){.str = NULL, .ucp = KB_KEY_LEGACY_SHIFT_PAGE_DOWN};
			break ;
	}
	return (escape){.str = NULL};
}
