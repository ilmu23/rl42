/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/06 15:34:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define E_UNTERMINATED_ESCAPE 0
#define E_UNRECOGNIZED_ESCAPE 1

#define incr(x)	(esc = (x == '\\') ? ((esc) ? 0 : 1) : 0, i++)

static const char	soh[3] = {'\e', 1, 0};
static const char	stx[3] = {'\e', 2, 0};
static const char	etx[3] = {'\e', 3, 0};
static const char	eot[3] = {'\e', 4, 0};
static const char	enq[3] = {'\e', 5, 0};
static const char	ack[3] = {'\e', 6, 0};
static const char	so[3] = {'\e', 14, 0};
static const char	si[3] = {'\e', 15, 0};
static const char	dle[3] = {'\e', 16, 0};
static const char	dc1[3] = {'\e', 17, 0};
static const char	dc2[3] = {'\e', 18, 0};
static const char	dc3[3] = {'\e', 19, 0};
static const char	dc4[3] = {'\e', 20, 0};
static const char	nak[3] = {'\e', 21, 0};
static const char	syn[3] = {'\e', 22, 0};
static const char	etb[3] = {'\e', 23, 0};
static const char	can[3] = {'\e', 24, 0};
static const char	em[3] = {'\e', 25, 0};
static const char	sub[3] = {'\e', 26, 0};
static const char	fs[3] = {'\e', 28, 0};
static const char	gs[3] = {'\e', 29, 0};
static const char	rs[3] = {'\e', 30, 0};
static const char	us[3] = {'\e', 31, 0};
static const char	bck[3] = {'\e', 127, 0};

static inline const char	*_esc(char *s);

rl_keytree_t	*ft_rl_getcurtree(void)
{
	switch (ft_rl_geteditmode())
	{
		case _MD_EMACS:
			return __mapget(g_binds, "emacs");
		case _MD_VI_INS:
			return __mapget(g_binds, "vi-ins");
		case _MD_VI_CMD:
			return __mapget(g_binds, "vi-cmd");
		case _MD_HLCOLOR:
			return __mapget(g_binds, "hlcolor");
		case _MD_SSI:
			return __mapget(g_binds, "__ssi");
	}
	return NULL;
}

const char	*ft_rl_parse_escape(const char *esc)
{
	switch (__strhash(esc, 347, UINT64_MAX))
	{
		case _KEY_SPC_HASH:
			return " ";
		case _KEY_ESC_HASH:
			return "\e";
		case _KEY_C_A_HASH:
			return soh + 1;
		case _KEY_C_B_HASH:
			return stx + 1;
		case _KEY_C_C_HASH:
			return etx + 1;
		case _KEY_C_D_HASH:
			return eot + 1;
		case _KEY_C_E_HASH:
			return enq + 1;
		case _KEY_C_F_HASH:
			return ack + 1;
		case _KEY_C_G_HASH:
			return "\a";
		case _KEY_C_H_HASH:
			return "\b";
		case _KEY_C_I_HASH:
			return "\t";
		case _KEY_C_J_HASH:
			return "\n";
		case _KEY_C_K_HASH:
			return "\v";
		case _KEY_C_L_HASH:
			return "\f";
		case _KEY_C_M_HASH:
			return "\r";
		case _KEY_C_N_HASH:
			return so + 1;
		case _KEY_C_O_HASH:
			return si + 1;
		case _KEY_C_P_HASH:
			return dle + 1;
		case _KEY_C_Q_HASH:
			return dc1 + 1;
		case _KEY_C_R_HASH:
			return dc2 + 1;
		case _KEY_C_S_HASH:
			return dc3 + 1;
		case _KEY_C_T_HASH:
			return dc4 + 1;
		case _KEY_C_U_HASH:
			return nak + 1;
		case _KEY_C_V_HASH:
			return syn + 1;
		case _KEY_C_W_HASH:
			return etb + 1;
		case _KEY_C_X_HASH:
			return can + 1;
		case _KEY_C_Y_HASH:
			return em + 1;
		case _KEY_C_Z_HASH:
			return sub + 1;
		case _KEY_C_OSBRACKET_HASH:
			return "\e";
		case _KEY_C_BSLASH_HASH:
			return fs + 1;
		case _KEY_C_CSBRACKET_HASH:
			return gs + 1;
		case _KEY_C_TILDE_HASH:
			return rs + 1;
		case _KEY_C_USCORE_HASH:
			return us + 1;
		case _KEY_C_AT_HASH:
			return "\0";
		case _KEY_M_SPC_HASH:
			return "\e ";
		case _KEY_M_BANG_HASH:
			return "\e!";
		case _KEY_M_DQUOTE_HASH:
			return "\e\"";
		case _KEY_M_POUNG_HASH:
			return "\e#";
		case _KEY_M_DOLLAR_HASH:
			return "\e$";
		case _KEY_M_PERCENT_HASH:
			return "\e%";
		case _KEY_M_AMP_HASH:
			return "\e&";
		case _KEY_M_SQUOTE_HASH:
			return "\e'";
		case _KEY_M_OPARENT_HASH:
			return "\e(";
		case _KEY_M_CPARENT_HASH:
			return "\e)";
		case _KEY_M_ASTERISK_HASH:
			return "\e*";
		case _KEY_M_PLUS_HASH:
			return "\e+";
		case _KEY_M_COMMA_HASH:
			return "\e,";
		case _KEY_M_MINUS_HASH:
			return "\e-";
		case _KEY_M_PERIOD_HASH:
			return "\e.";
		case _KEY_M_SLASH_HASH:
			return "\e/";
		case _KEY_M_0_HASH:
			return "\e0";
		case _KEY_M_1_HASH:
			return "\e1";
		case _KEY_M_2_HASH:
			return "\e2";
		case _KEY_M_3_HASH:
			return "\e3";
		case _KEY_M_4_HASH:
			return "\e4";
		case _KEY_M_5_HASH:
			return "\e5";
		case _KEY_M_6_HASH:
			return "\e6";
		case _KEY_M_7_HASH:
			return "\e7";
		case _KEY_M_8_HASH:
			return "\e8";
		case _KEY_M_9_HASH:
			return "\e9";
		case _KEY_M_COLON_HASH:
			return "\e:";
		case _KEY_M_SCOLON_HASH:
			return "\e;";
		case _KEY_M_LTHAN_HASH:
			return "\e<";
		case _KEY_M_EQUAL_HASH:
			return "\e=";
		case _KEY_M_MTAHN_HASH:
			return "\e>";
		case _KEY_M_QUESTION_HASH:
			return "\e?";
		case _KEY_M_AT_HASH:
			return "\e@";
		case _KEY_M_UP_A_HASH:
			return "\eA";
		case _KEY_M_UP_B_HASH:
			return "\eB";
		case _KEY_M_UP_C_HASH:
			return "\eC";
		case _KEY_M_UP_D_HASH:
			return "\eD";
		case _KEY_M_UP_E_HASH:
			return "\eE";
		case _KEY_M_UP_F_HASH:
			return "\eF";
		case _KEY_M_UP_G_HASH:
			return "\eG";
		case _KEY_M_UP_H_HASH:
			return "\eH";
		case _KEY_M_UP_I_HASH:
			return "\eI";
		case _KEY_M_UP_J_HASH:
			return "\eJ";
		case _KEY_M_UP_K_HASH:
			return "\eK";
		case _KEY_M_UP_L_HASH:
			return "\eL";
		case _KEY_M_UP_M_HASH:
			return "\eM";
		case _KEY_M_UP_N_HASH:
			return "\eN";
		case _KEY_M_UP_O_HASH:
			return "\eO";
		case _KEY_M_UP_P_HASH:
			return "\eP";
		case _KEY_M_UP_Q_HASH:
			return "\eQ";
		case _KEY_M_UP_R_HASH:
			return "\eR";
		case _KEY_M_UP_S_HASH:
			return "\eS";
		case _KEY_M_UP_T_HASH:
			return "\eT";
		case _KEY_M_UP_U_HASH:
			return "\eU";
		case _KEY_M_UP_V_HASH:
			return "\eV";
		case _KEY_M_UP_W_HASH:
			return "\eW";
		case _KEY_M_UP_X_HASH:
			return "\eX";
		case _KEY_M_UP_Y_HASH:
			return "\eY";
		case _KEY_M_UP_Z_HASH:
			return "\eZ";
		case _KEY_M_OSBRACKET_HASH:
			return "\e[";
		case _KEY_M_BSLASH_HASH:
			return "\e\\";
		case _KEY_M_CSBRACKET_HASH:
			return "\e]";
		case _KEY_M_CARET_HASH:
			return "\e^";
		case _KEY_M_USCORE_HASH:
			return "\e_";
		case _KEY_M_BTICK_HASH:
			return "\e`";
		case _KEY_M_DN_A_HASH:
			return "\ea";
		case _KEY_M_DN_B_HASH:
			return "\eb";
		case _KEY_M_DN_C_HASH:
			return "\ec";
		case _KEY_M_DN_D_HASH:
			return "\ed";
		case _KEY_M_DN_E_HASH:
			return "\ee";
		case _KEY_M_DN_F_HASH:
			return "\ef";
		case _KEY_M_DN_G_HASH:
			return "\eg";
		case _KEY_M_DN_H_HASH:
			return "\eh";
		case _KEY_M_DN_I_HASH:
			return "\ei";
		case _KEY_M_DN_J_HASH:
			return "\ej";
		case _KEY_M_DN_K_HASH:
			return "\ek";
		case _KEY_M_DN_L_HASH:
			return "\el";
		case _KEY_M_DN_M_HASH:
			return "\em";
		case _KEY_M_DN_N_HASH:
			return "\en";
		case _KEY_M_DN_O_HASH:
			return "\eo";
		case _KEY_M_DN_P_HASH:
			return "\ep";
		case _KEY_M_DN_Q_HASH:
			return "\eq";
		case _KEY_M_DN_R_HASH:
			return "\er";
		case _KEY_M_DN_S_HASH:
			return "\es";
		case _KEY_M_DN_T_HASH:
			return "\et";
		case _KEY_M_DN_U_HASH:
			return "\eu";
		case _KEY_M_DN_V_HASH:
			return "\ev";
		case _KEY_M_DN_W_HASH:
			return "\ew";
		case _KEY_M_DN_X_HASH:
			return "\ex";
		case _KEY_M_DN_Y_HASH:
			return "\ey";
		case _KEY_M_DN_Z_HASH:
			return "\ez";
		case _KEY_M_OCBRACKET_HASH:
			return "\e{";
		case _KEY_M_PIPE_HASH:
			return "\e|";
		case _KEY_M_CCBRACKET_HASH:
			return "\e}";
		case _KEY_M_TILDE_HASH:
			return "\e~";
		case _KEY_M_BCK_HASH:
			return bck;
		case _KEY_M_C_A_HASH:
			return soh;
		case _KEY_M_C_B_HASH:
			return stx;
		case _KEY_M_C_C_HASH:
			return etx;
		case _KEY_M_C_D_HASH:
			return eot;
		case _KEY_M_C_E_HASH:
			return enq;
		case _KEY_M_C_F_HASH:
			return ack;
		case _KEY_M_C_G_HASH:
			return "\e\a";
		case _KEY_M_C_H_HASH:
			return "\e\b";
		case _KEY_M_C_I_HASH:
			return "\e\t";
		case _KEY_M_C_J_HASH:
			return "\e\n";
		case _KEY_M_C_K_HASH:
			return "\e\v";
		case _KEY_M_C_L_HASH:
			return "\e\f";
		case _KEY_M_C_M_HASH:
			return "\e\r";
		case _KEY_M_C_N_HASH:
			return so;
		case _KEY_M_C_O_HASH:
			return si;
		case _KEY_M_C_P_HASH:
			return dle;
		case _KEY_M_C_Q_HASH:
			return dc1;
		case _KEY_M_C_R_HASH:
			return dc2;
		case _KEY_M_C_S_HASH:
			return dc3;
		case _KEY_M_C_T_HASH:
			return dc4;
		case _KEY_M_C_U_HASH:
			return nak;
		case _KEY_M_C_V_HASH:
			return syn;
		case _KEY_M_C_W_HASH:
			return etb;
		case _KEY_M_C_X_HASH:
			return can;
		case _KEY_M_C_Y_HASH:
			return em;
		case _KEY_M_C_Z_HASH:
			return sub;
		case _KEY_M_C_OSBRACKET_HASH:
			return "\e\e";
		case _KEY_M_C_BSLASH_HASH:
			return fs;
		case _KEY_M_C_CSBRACKET_HASH:
			return gs;
		case _KEY_M_C_TILDE_HASH:
			return rs;
		case _KEY_M_C_USCORE_HASH:
			return us;
		case _KEY_M_C_AT_HASH:
			return "\0";
		case _KEY_F_1_HASH:
			return g_escapes.kf1;
		case _KEY_F_2_HASH:
			return g_escapes.kf2;
		case _KEY_F_3_HASH:
			return g_escapes.kf3;
		case _KEY_F_4_HASH:
			return g_escapes.kf4;
		case _KEY_F_5_HASH:
			return g_escapes.kf5;
		case _KEY_F_6_HASH:
			return g_escapes.kf6;
		case _KEY_F_7_HASH:
			return g_escapes.kf7;
		case _KEY_F_8_HASH:
			return g_escapes.kf8;
		case _KEY_F_9_HASH:
			return g_escapes.kf9;
		case _KEY_F_10_HASH:
			return g_escapes.kf10;
		case _KEY_F_11_HASH:
			return g_escapes.kf11;
		case _KEY_F_12_HASH:
			return g_escapes.kf12;
		case _KEY_UP_HASH:
			return g_escapes.kcuu1;
		case _KEY_DOWN_HASH:
			return g_escapes.kcud1;
		case _KEY_LEFT_HASH:
			return g_escapes.kcub1;
		case _KEY_RIGHT_HASH:
			return g_escapes.kcuf1;
		case _KEY_S_LEFT_HASH:
			return g_escapes.kLFT;
		case _KEY_S_RIGHT_HASH:
			return g_escapes.kRIT;
		case _KEY_BCK_HASH:
			return bck + 1;
		case _KEY_TAB_HASH:
			return g_escapes.ht;
		case _KEY_ENT_HASH:
			return g_escapes.kent;
		case _KEY_RET_HASH:
			return g_escapes.cr;
		case _KEY_INS_HASH:
			return g_escapes.kich1;
		case _KEY_HME_HASH:
			return g_escapes.khome;
		case _KEY_PGU_HASH:
			return g_escapes.knp;
		case _KEY_DEL_HASH:
			return g_escapes.kdch1;
		case _KEY_END_HASH:
			return g_escapes.kend;
		case _KEY_PGD_HASH:
			return g_escapes.kpp;
		case _KEY_S_HME_HASH:
			return g_escapes.kHOM;
		case _KEY_S_DEL_HASH:
			return g_escapes.kDC;
		case _KEY_S_END_HASH:
			return g_escapes.kEND;
		default:
			ft_rl_bind_error(E_UNRECOGNIZED_ESCAPE, esc);
	}
	return NULL;
}

const char	*ft_rl_parse_sequence(const char *seq)
{
	size_t		i;
	size_t		j;
	uint8_t		esc;
	const char	*tmp;
	char		*out;

	for (i = 0, j = i, esc = 0, out = NULL, tmp = NULL; seq[i]; j = i, tmp = NULL)
	{
		if (seq[i] == '<' && (i == 0 || seq[i - 1] != '\\'))
		{
			while (seq[i] && (seq[i] != '>' || esc))
				incr(seq[i]);
			if (!seq[i])
				ft_rl_bind_error(E_UNTERMINATED_ESCAPE, seq);
			else
				tmp = ft_rl_parse_escape(_esc(__substr(seq, j, ++i - j)));
		}
		else
		{
			while (seq[i] && (seq[i] != '<' || esc))
				incr(seq[i]);
			tmp = _esc(__substr(seq, j, i - j));
		}
		if (!tmp)
			return NULL;
		__popblk(out);
		out = __push(__strjoin(out, tmp));
		__popblk(tmp);
	}
	return out;
}

uint8_t		ft_rl_isbound(const char *seq)
{
	return (ft_rl_getfn(seq)) ? 1 : 0;
}

uint8_t		ft_rl_unmap(const char *seq)
{
	rl_keytree_t	*tmp;
	size_t			i;

	seq = ft_rl_parse_sequence(seq);
	if (!seq)
		return 0;
	for (i = 0, tmp = ft_rl_getcurtree(); tmp && seq[i]; i++)
		tmp = tmp->next[(uint8_t)seq[i]];
	__popblk(seq);
	if (!tmp)
		return 0;
	tmp->fn = NULL;
	return 1;
}

rl_fn_t		ft_rl_getfn(const char *seq)
{
	rl_keytree_t	*tmp;
	size_t			i;

	seq = ft_rl_parse_sequence(seq);
	if (!seq)
		return NULL;
	for (i = 0, tmp = ft_rl_getcurtree(); tmp && seq[i]; i++)
		tmp = tmp->next[(uint8_t)seq[i]];
	__popblk(seq);
	return (tmp) ? tmp->fn : NULL;
}

void	ft_rl_bind_error(const uint8_t err, const char *context)
{
	switch (err)
	{
		case E_UNTERMINATED_ESCAPE:
			if (context)
				__dprintf(2, "rl42: unterminated escape in keysequence: '%s'\n", context);
			else
				__dprintf(2, "rl42: unterminated escape in keysequence\n");
			break ;
		case E_UNRECOGNIZED_ESCAPE:
			if (context)
				__dprintf(2, "rl42: unrecognized escape in keysequence: '%s'\n", context);
			else
				__dprintf(2, "rl42: unrecognized escape in keysequence\n");
			break ;
		default:
			if (context)
				__dprintf(2, "rl42: undefined error: '%s'\n", context);
			else
				__dprintf(2, "rl42: undefined error\n");
	}
}

static inline const char	*_esc(char *s)
{
	size_t	len;
	size_t	i;

	if (!s)
		return NULL;
	for (i = 0, len = strlen(s); i < len; i++)
		if (s[i] == '\\')
			__strlcpy(&s[i], &s[i + 1], len-- - i);
	return s;
}
