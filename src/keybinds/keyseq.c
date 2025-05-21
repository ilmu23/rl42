// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<keyseq.c>>

#include "internal/_hashes.h"
#include "internal/_keybinds.h"

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

static inline const char	*_match_escape(const char *seq, size_t len);

rl42_string	*expand_seq(const char *seq) {
	rl42_string	*out;
	const char	*tmp;
	vector		*expanded_seq;
	size_t		i;
	size_t		j;
	u8			csize;
	u8			esc;

	expanded_seq = vector_new(256, NONE);
	if (!expanded_seq)
		return NULL;
	for (i = j = esc = 0, tmp = NULL; seq[i]; j = ++i, tmp = NULL) {
		csize = charsize_utf8(seq[i]);
		if (csize == 1 && seq[i] == '<' && !esc) {
			i++;
			while (seq[i] && (seq[i] != '>' || esc))
				if (seq[i++] == '\\')
					esc ^= 1;
			if (!seq[i])
				goto err;
			tmp = _match_escape(&seq[j], i - j + 1);
			if (!tmp)
				goto err;
			csize = strlen(tmp);
		}
		while (csize--) {
			if (!vector_add(expanded_seq, (tmp) ? *tmp++ : seq[j++]))
				goto err;
		}
	}
	tmp = vector_get_raw_data(expanded_seq, sizeof(*tmp), SIZE_MAX);
	if (!tmp)
		goto err;
	vector_delete(expanded_seq, NULL);
	out = cstr_to_rl42str(tmp);
	free((void *)tmp);
	return out;
err:
	vector_delete(expanded_seq, NULL);
	return EXPAND_INVALID_SEQ;
}

static inline const char	*_match_escape(const char *seq, size_t len) {
	size_t	i;
	char	esc[16];
	u64		hash;

	memcpy(esc, seq, len);
	esc[len] = '\0';
	for (i = 0; i < len; i++)
		if (esc[i] == '\\')
			memmove(&esc[i], &esc[i + 1], len-- - i);
	hash = cstr_hash(esc, 347, UINT64_MAX);
	switch (hash) {
		case KEY_C_AT_HASH:
			return &nul[1];
		case KEY_C_A_HASH:
			return &soh[1];
		case KEY_C_B_HASH:
			return &stx[1];
		case KEY_C_C_HASH:
			return &etx[1];
		case KEY_C_D_HASH:
			return &eot[1];
		case KEY_C_E_HASH:
			return &enq[1];
		case KEY_C_F_HASH:
			return &ack[1];
		case KEY_C_G_HASH:
			return &bel[1];
		case KEY_C_H_HASH:
			return &bs[1];
		case KEY_C_I_HASH:
			return &ht[1];
		case KEY_C_J_HASH:
			return &lf[1];
		case KEY_C_K_HASH:
			return &vt[1];
		case KEY_C_L_HASH:
			return &ff[1];
		case KEY_C_M_HASH:
			return &cr[1];
		case KEY_C_N_HASH:
			return &so[1];
		case KEY_C_O_HASH:
			return &si[1];
		case KEY_C_P_HASH:
			return &dle[1];
		case KEY_C_Q_HASH:
			return &dc1[1];
		case KEY_C_R_HASH:
			return &dc2[1];
		case KEY_C_S_HASH:
			return &dc3[1];
		case KEY_C_T_HASH:
			return &dc4[1];
		case KEY_C_U_HASH:
			return &nak[1];
		case KEY_C_V_HASH:
			return &syn[1];
		case KEY_C_W_HASH:
			return &etb[1];
		case KEY_C_X_HASH:
			return &can[1];
		case KEY_C_Y_HASH:
			return &em[1];
		case KEY_C_Z_HASH:
			return &sub[1];
		case KEY_ESC_HASH:
		case KEY_C_OSBRACKET_HASH:
			return "\x1b";
		case KEY_C_BSLASH_HASH:
			return &fs[1];
		case KEY_C_CSBRACKET_HASH:
			return &gs[1];
		case KEY_C_TILDE_HASH:
			return &rs[1];
		case KEY_C_USCORE_HASH:
			return &us[1];
		case KEY_M_C_AT_HASH:
			return nul;
		case KEY_M_C_A_HASH:
			return soh;
		case KEY_M_C_B_HASH:
			return stx;
		case KEY_M_C_C_HASH:
			return etx;
		case KEY_M_C_D_HASH:
			return eot;
		case KEY_M_C_E_HASH:
			return enq;
		case KEY_M_C_F_HASH:
			return ack;
		case KEY_M_C_G_HASH:
			return bel;
		case KEY_M_C_H_HASH:
			return bs;
		case KEY_M_C_I_HASH:
			return ht;
		case KEY_M_C_J_HASH:
			return lf;
		case KEY_M_C_K_HASH:
			return vt;
		case KEY_M_C_L_HASH:
			return ff;
		case KEY_M_C_M_HASH:
			return cr;
		case KEY_M_C_N_HASH:
			return so;
		case KEY_M_C_O_HASH:
			return si;
		case KEY_M_C_P_HASH:
			return dle;
		case KEY_M_C_Q_HASH:
			return dc1;
		case KEY_M_C_R_HASH:
			return dc2;
		case KEY_M_C_S_HASH:
			return dc3;
		case KEY_M_C_T_HASH:
			return dc4;
		case KEY_M_C_U_HASH:
			return nak;
		case KEY_M_C_V_HASH:
			return syn;
		case KEY_M_C_W_HASH:
			return etb;
		case KEY_M_C_X_HASH:
			return can;
		case KEY_M_C_Y_HASH:
			return em;
		case KEY_M_C_Z_HASH:
			return sub;
		case KEY_M_C_OSBRACKET_HASH:
			return "\x1b\x1b";
		case KEY_M_C_BSLASH_HASH:
			return fs;
		case KEY_M_C_CSBRACKET_HASH:
			return gs;
		case KEY_M_C_TILDE_HASH:
			return rs;
		case KEY_M_C_USCORE_HASH:
			return us;
		case KEY_SPC_HASH:
			return " ";
		case KEY_BCK_HASH:
			return &bck[1];
		case KEY_M_SPC_HASH:
			return "\x1b ";
		case KEY_M_BCK_HASH:
			return bck;
		case KEY_M_BANG_HASH:
			return "\x1b!";
		case KEY_M_DQUOTE_HASH:
			return "\x1b\"";
		case KEY_M_POUNG_HASH:
			return "\x1b#";
		case KEY_M_DOLLAR_HASH:
			return "\x1b$";
		case KEY_M_PERCENT_HASH:
			return "\x1b%";
		case KEY_M_AMP_HASH:
			return "\x1b&";
		case KEY_M_SQUOTE_HASH:
			return "\x1b'";
		case KEY_M_OPARENT_HASH:
			return "\x1b(";
		case KEY_M_CPARENT_HASH:
			return "\x1b)";
		case KEY_M_ASTERISK_HASH:
			return "\x1b*";
		case KEY_M_PLUS_HASH:
			return "\x1b+";
		case KEY_M_COMMA_HASH:
			return "\x1b,";
		case KEY_M_MINUS_HASH:
			return "\x1b-";
		case KEY_M_PERIOD_HASH:
			return "\x1b.";
		case KEY_M_SLASH_HASH:
			return "\x1b/";
		case KEY_M_0_HASH:
			return "\x1b" "0";
		case KEY_M_1_HASH:
			return "\x1b" "1";
		case KEY_M_2_HASH:
			return "\x1b" "2";
		case KEY_M_3_HASH:
			return "\x1b" "3";
		case KEY_M_4_HASH:
			return "\x1b" "4";
		case KEY_M_5_HASH:
			return "\x1b" "5";
		case KEY_M_6_HASH:
			return "\x1b" "6";
		case KEY_M_7_HASH:
			return "\x1b" "7";
		case KEY_M_8_HASH:
			return "\x1b" "8";
		case KEY_M_9_HASH:
			return "\x1b" "9";
		case KEY_M_COLON_HASH:
			return "\x1b:";
		case KEY_M_SCOLON_HASH:
			return "\x1b;";
		case KEY_M_LTHAN_HASH:
			return "\x1b<";
		case KEY_M_EQUAL_HASH:
			return "\x1b=";
		case KEY_M_MTAHN_HASH:
			return "\x1b>";
		case KEY_M_QUESTION_HASH:
			return "\x1b?";
		case KEY_M_AT_HASH:
			return "\x1b@";
		case KEY_M_UP_A_HASH:
			return "\x1b" "A";
		case KEY_M_UP_B_HASH:
			return "\x1b" "B";
		case KEY_M_UP_C_HASH:
			return "\x1b" "C";
		case KEY_M_UP_D_HASH:
			return "\x1b" "D";
		case KEY_M_UP_E_HASH:
			return "\x1b" "E";
		case KEY_M_UP_F_HASH:
			return "\x1b" "F";
		case KEY_M_UP_G_HASH:
			return "\x1bG";
		case KEY_M_UP_H_HASH:
			return "\x1bH";
		case KEY_M_UP_I_HASH:
			return "\x1bI";
		case KEY_M_UP_J_HASH:
			return "\x1bJ";
		case KEY_M_UP_K_HASH:
			return "\x1bK";
		case KEY_M_UP_L_HASH:
			return "\x1bL";
		case KEY_M_UP_M_HASH:
			return "\x1bM";
		case KEY_M_UP_N_HASH:
			return "\x1bN";
		case KEY_M_UP_O_HASH:
			return "\x1bO";
		case KEY_M_UP_P_HASH:
			return "\x1bP";
		case KEY_M_UP_Q_HASH:
			return "\x1bQ";
		case KEY_M_UP_R_HASH:
			return "\x1bR";
		case KEY_M_UP_S_HASH:
			return "\x1bS";
		case KEY_M_UP_T_HASH:
			return "\x1bT";
		case KEY_M_UP_U_HASH:
			return "\x1bU";
		case KEY_M_UP_V_HASH:
			return "\x1bV";
		case KEY_M_UP_W_HASH:
			return "\x1bW";
		case KEY_M_UP_X_HASH:
			return "\x1bX";
		case KEY_M_UP_Y_HASH:
			return "\x1bY";
		case KEY_M_UP_Z_HASH:
			return "\x1bZ";
		case KEY_M_OSBRACKET_HASH:
			return "\x1b[";
		case KEY_M_BSLASH_HASH:
			return "\x1b\\";
		case KEY_M_CSBRACKET_HASH:
			return "\x1b]";
		case KEY_M_CARET_HASH:
			return "\x1b^";
		case KEY_M_USCORE_HASH:
			return "\x1b_";
		case KEY_M_BTICK_HASH:
			return "\x1b`";
		case KEY_M_DN_A_HASH:
			return "\x1b" "a";
		case KEY_M_DN_B_HASH:
			return "\x1b" "a";
		case KEY_M_DN_C_HASH:
			return "\x1b" "c";
		case KEY_M_DN_D_HASH:
			return "\x1b" "d";
		case KEY_M_DN_E_HASH:
			return "\x1b" "e";
		case KEY_M_DN_F_HASH:
			return "\x1b" "f";
		case KEY_M_DN_G_HASH:
			return "\x1bg";
		case KEY_M_DN_H_HASH:
			return "\x1bh";
		case KEY_M_DN_I_HASH:
			return "\x1bi";
		case KEY_M_DN_J_HASH:
			return "\x1bj";
		case KEY_M_DN_K_HASH:
			return "\x1bk";
		case KEY_M_DN_L_HASH:
			return "\x1bl";
		case KEY_M_DN_M_HASH:
			return "\x1bm";
		case KEY_M_DN_N_HASH:
			return "\x1bn";
		case KEY_M_DN_O_HASH:
			return "\x1bo";
		case KEY_M_DN_P_HASH:
			return "\x1bp";
		case KEY_M_DN_Q_HASH:
			return "\x1bq";
		case KEY_M_DN_R_HASH:
			return "\x1br";
		case KEY_M_DN_S_HASH:
			return "\x1bs";
		case KEY_M_DN_T_HASH:
			return "\x1bt";
		case KEY_M_DN_U_HASH:
			return "\x1bu";
		case KEY_M_DN_V_HASH:
			return "\x1bv";
		case KEY_M_DN_W_HASH:
			return "\x1bw";
		case KEY_M_DN_X_HASH:
			return "\x1bx";
		case KEY_M_DN_Y_HASH:
			return "\x1by";
		case KEY_M_DN_Z_HASH:
			return "\x1bz";
		case KEY_M_OCBRACKET_HASH:
			return "\x1b{";
		case KEY_M_PIPE_HASH:
			return "\x1b|";
		case KEY_M_CCBRACKET_HASH:
			return "\x1b}";
		case KEY_M_TILDE_HASH:
			return "\x1b~";
		// TODO: terminal specific escapes
		case KEY_F_1_HASH:
		case KEY_F_2_HASH:
		case KEY_F_3_HASH:
		case KEY_F_4_HASH:
		case KEY_F_5_HASH:
		case KEY_F_6_HASH:
		case KEY_F_7_HASH:
		case KEY_F_8_HASH:
		case KEY_F_9_HASH:
		case KEY_F_10_HASH:
		case KEY_F_11_HASH:
		case KEY_F_12_HASH:
		case KEY_UP_HASH:
		case KEY_DOWN_HASH:
		case KEY_LEFT_HASH:
		case KEY_RIGHT_HASH:
		case KEY_S_LEFT_HASH:
		case KEY_S_RIGHT_HASH:
		case KEY_TAB_HASH:
		case KEY_ENT_HASH:
		case KEY_RET_HASH:
		case KEY_INS_HASH:
		case KEY_HME_HASH:
		case KEY_PGU_HASH:
		case KEY_DEL_HASH:
		case KEY_END_HASH:
		case KEY_PGD_HASH:
		case KEY_S_HME_HASH:
		case KEY_S_DEL_HASH:
		case KEY_S_END_HASH:
			break ;
	}
	return NULL;
}
