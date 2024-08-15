/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initkeys.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:44:17 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:28:52 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_initkeys(void)
{
	char		key[8];
	uint64_t	val;

	ft_rl_addkey("<SPC>", KEY_SPACE);
	__strlcpy(key, "!", 2);
	val = KEY_BANG;
	while (val <= KEY_TILDE)
	{
		ft_rl_addkey(key, val++);
		*key = val;
	}
	ft_rl_addkey("<BCK>", KEY_BACKSPACE);
	ft_rl_addkey("<C-@>", KEY_CTRL_AT);
	__strlcpy(key, "<C-a>", 6);
	val = KEY_CTRL_A;
	while (val <= KEY_CTRL_Z)
	{
		ft_rl_addkey(key, val++);
		key[3] = val + 0x60U;
	}
	ft_rl_addkey("<C-[>", KEY_CTRL_OSBRACKET);
	ft_rl_addkey("<C-\\>", KEY_CTRL_BACKSLASH);
	ft_rl_addkey("<C-]>", KEY_CTRL_CSBRACKET);
	ft_rl_addkey("<C-~>", KEY_CTRL_TILDE);
	ft_rl_addkey("<C-_>", KEY_CTRL_UNDERSCORE);
	ft_rl_addkey("<M-SPC>", KEY_ALT_SPACE);
	__strlcpy(key, "<M-!>", 6);
	val = KEY_ALT_BANG;
	while (val <= (KEY_ALT_TILDE))
	{
		ft_rl_addkey(key, val);
		val += 0x1U << 8;
		key[3] = val >> 8;
	}
	ft_rl_addkey("<M-BCK>", KEY_ALT_BACKSPACE);
	ft_rl_addkey("<M-C-@>", KEY_ALT_CTRL_AT);
	__strlcpy(key, "<M-C-a>", 8);
	val = KEY_ALT_CTRL_A;
	while (val <= (KEY_ALT_CTRL_Z))
	{
		ft_rl_addkey(key, val);
		val += 0x1U << 8;
		key[5] = (val >> 8) + 0x60ULL;
	}
	ft_rl_addkey("<M-C-[>", KEY_ALT_CTRL_OSBRACKET);
	ft_rl_addkey("<M-C-\\>", KEY_ALT_CTRL_BACKSLASH);
	ft_rl_addkey("<M-C-]>", KEY_ALT_CTRL_CSBRACKET);
	ft_rl_addkey("<M-C-~>", KEY_ALT_CTRL_TILDE);
	ft_rl_addkey("<M-C-_>", KEY_ALT_CTRL_UNDERSCORE);
	ft_rl_addkey("<F-1>", KEY_F_1);
	ft_rl_addkey("<F-2>", KEY_F_2);
	ft_rl_addkey("<F-3>", KEY_F_3);
	ft_rl_addkey("<F-4>", KEY_F_4);
	ft_rl_addkey("<F-5>", KEY_F_5);
	ft_rl_addkey("<F-6>", KEY_F_6);
	ft_rl_addkey("<F-7>", KEY_F_7);
	ft_rl_addkey("<F-8>", KEY_F_8);
	ft_rl_addkey("<F-9>", KEY_F_9);
	ft_rl_addkey("<F-10>", KEY_F_10);
	ft_rl_addkey("<F-11>", KEY_F_11);
	ft_rl_addkey("<F-12>", KEY_F_12);
	ft_rl_addkey("<U-F-1>", KEY_UP_F_1);
	ft_rl_addkey("<U-F-2>", KEY_UP_F_2);
	ft_rl_addkey("<U-F-3>", KEY_UP_F_3);
	ft_rl_addkey("<U-F-4>", KEY_UP_F_4);
	ft_rl_addkey("<U-F-5>", KEY_UP_F_5);
	ft_rl_addkey("<U-F-6>", KEY_UP_F_6);
	ft_rl_addkey("<U-F-7>", KEY_UP_F_7);
	ft_rl_addkey("<U-F-8>", KEY_UP_F_8);
	ft_rl_addkey("<U-F-9>", KEY_UP_F_9);
	ft_rl_addkey("<U-F-10>", KEY_UP_F_10);
	ft_rl_addkey("<U-F-11>", KEY_UP_F_11);
	ft_rl_addkey("<U-F-12>", KEY_UP_F_12);
	ft_rl_addkey("<M-F-1>", KEY_ALT_F_1);
	ft_rl_addkey("<M-F-2>", KEY_ALT_F_2);
	ft_rl_addkey("<M-F-3>", KEY_ALT_F_3);
	ft_rl_addkey("<M-F-4>", KEY_ALT_F_4);
	ft_rl_addkey("<M-F-5>", KEY_ALT_F_5);
	ft_rl_addkey("<M-F-6>", KEY_ALT_F_6);
	ft_rl_addkey("<M-F-7>", KEY_ALT_F_7);
	ft_rl_addkey("<M-F-8>", KEY_ALT_F_8);
	ft_rl_addkey("<M-F-9>", KEY_ALT_F_9);
	ft_rl_addkey("<M-F-10>", KEY_ALT_F_10);
	ft_rl_addkey("<M-F-11>", KEY_ALT_F_11);
	ft_rl_addkey("<M-F-12>", KEY_ALT_F_12);
	ft_rl_addkey("<M-U-F-1>", KEY_ALT_UP_F_1);
	ft_rl_addkey("<M-U-F-2>", KEY_ALT_UP_F_2);
	ft_rl_addkey("<M-U-F-3>", KEY_ALT_UP_F_3);
	ft_rl_addkey("<M-U-F-4>", KEY_ALT_UP_F_4);
	ft_rl_addkey("<M-U-F-5>", KEY_ALT_UP_F_5);
	ft_rl_addkey("<M-U-F-6>", KEY_ALT_UP_F_6);
	ft_rl_addkey("<M-U-F-7>", KEY_ALT_UP_F_7);
	ft_rl_addkey("<M-U-F-8>", KEY_ALT_UP_F_8);
	ft_rl_addkey("<M-U-F-9>", KEY_ALT_UP_F_9);
	ft_rl_addkey("<M-U-F-10>", KEY_ALT_UP_F_10);
	ft_rl_addkey("<M-U-F-11>", KEY_ALT_UP_F_11);
	ft_rl_addkey("<M-U-F-12>", KEY_ALT_UP_F_12);
	ft_rl_addkey("<C-F-1>", KEY_CTRL_F_1);
	ft_rl_addkey("<C-F-2>", KEY_CTRL_F_2);
	ft_rl_addkey("<C-F-3>", KEY_CTRL_F_3);
	ft_rl_addkey("<C-F-4>", KEY_CTRL_F_4);
	ft_rl_addkey("<C-F-5>", KEY_CTRL_F_5);
	ft_rl_addkey("<C-F-6>", KEY_CTRL_F_6);
	ft_rl_addkey("<C-F-7>", KEY_CTRL_F_7);
	ft_rl_addkey("<C-F-8>", KEY_CTRL_F_8);
	ft_rl_addkey("<C-F-9>", KEY_CTRL_F_9);
	ft_rl_addkey("<C-F-10>", KEY_CTRL_F_10);
	ft_rl_addkey("<C-F-11>", KEY_CTRL_F_11);
	ft_rl_addkey("<C-F-12>", KEY_CTRL_F_12);
	ft_rl_addkey("<C-U-F-1>", KEY_CTRL_UP_F_1);
	ft_rl_addkey("<C-U-F-2>", KEY_CTRL_UP_F_2);
	ft_rl_addkey("<C-U-F-3>", KEY_CTRL_UP_F_3);
	ft_rl_addkey("<C-U-F-4>", KEY_CTRL_UP_F_4);
	ft_rl_addkey("<C-U-F-5>", KEY_CTRL_UP_F_5);
	ft_rl_addkey("<C-U-F-6>", KEY_CTRL_UP_F_6);
	ft_rl_addkey("<C-U-F-7>", KEY_CTRL_UP_F_7);
	ft_rl_addkey("<C-U-F-8>", KEY_CTRL_UP_F_8);
	ft_rl_addkey("<C-U-F-9>", KEY_CTRL_UP_F_9);
	ft_rl_addkey("<C-U-F-10>", KEY_CTRL_UP_F_10);
	ft_rl_addkey("<C-U-F-11>", KEY_CTRL_UP_F_11);
	ft_rl_addkey("<C-U-F-12>", KEY_CTRL_UP_F_12);
	ft_rl_addkey("<M-C-U-F-1>", KEY_ALT_CTRL_UP_F_1);
	ft_rl_addkey("<M-C-U-F-2>", KEY_ALT_CTRL_UP_F_2);
	ft_rl_addkey("<M-C-U-F-3>", KEY_ALT_CTRL_UP_F_3);
	ft_rl_addkey("<M-C-U-F-4>", KEY_ALT_CTRL_UP_F_4);
	ft_rl_addkey("<M-C-U-F-5>", KEY_ALT_CTRL_UP_F_5);
	ft_rl_addkey("<M-C-U-F-6>", KEY_ALT_CTRL_UP_F_6);
	ft_rl_addkey("<M-C-U-F-7>", KEY_ALT_CTRL_UP_F_7);
	ft_rl_addkey("<M-C-U-F-8>", KEY_ALT_CTRL_UP_F_8);
	ft_rl_addkey("<M-C-U-F-9>", KEY_ALT_CTRL_UP_F_9);
	ft_rl_addkey("<M-C-U-F-10>", KEY_ALT_CTRL_UP_F_10);
	ft_rl_addkey("<M-C-U-F-11>", KEY_ALT_CTRL_UP_F_11);
	ft_rl_addkey("<M-C-U-F-12>", KEY_ALT_CTRL_UP_F_12);
	ft_rl_addkey("<up>", KEY_UP);
	ft_rl_addkey("<down>", KEY_DOWN);
	ft_rl_addkey("<left>", KEY_LEFT);
	ft_rl_addkey("<right>", KEY_RIGHT);
	ft_rl_addkey("<U-up>", KEY_UP_UP);
	ft_rl_addkey("<U-down>", KEY_UP_DOWN);
	ft_rl_addkey("<U-left>", KEY_UP_LEFT);
	ft_rl_addkey("<U-right>", KEY_UP_RIGHT);
	ft_rl_addkey("<M-up>", KEY_ALT_UP);
	ft_rl_addkey("<M-down>", KEY_ALT_DOWN);
	ft_rl_addkey("<M-left>", KEY_ALT_LEFT);
	ft_rl_addkey("<M-right>", KEY_ALT_RIGHT);
	ft_rl_addkey("<M-U-up>", KEY_ALT_UP_UP);
	ft_rl_addkey("<M-U-down>", KEY_ALT_UP_DOWN);
	ft_rl_addkey("<M-U-left>", KEY_ALT_UP_LEFT);
	ft_rl_addkey("<M-U-right>", KEY_ALT_UP_RIGHT);
	ft_rl_addkey("<C-up>", KEY_CTRL_UP);
	ft_rl_addkey("<C-down>", KEY_CTRL_DOWN);
	ft_rl_addkey("<C-left>", KEY_CTRL_LEFT);
	ft_rl_addkey("<C-right>", KEY_CTRL_RIGHT);
	ft_rl_addkey("<C-U-up>", KEY_CTRL_UP_UP);
	ft_rl_addkey("<C-U-down>", KEY_CTRL_UP_DOWN);
	ft_rl_addkey("<C-U-left>", KEY_CTRL_UP_LEFT);
	ft_rl_addkey("<C-U-right>", KEY_CTRL_UP_RIGHT);
	ft_rl_addkey("<M-C-up>", KEY_ALT_CTRL_UP);
	ft_rl_addkey("<M-C-down>", KEY_ALT_CTRL_DOWN);
	ft_rl_addkey("<M-C-left>", KEY_ALT_CTRL_LEFT);
	ft_rl_addkey("<M-C-right>", KEY_ALT_CTRL_RIGHT);
	ft_rl_addkey("<M-C-U-up>", KEY_ALT_CTRL_UP_UP);
	ft_rl_addkey("<M-C-U-down>", KEY_ALT_CTRL_UP_DOWN);
	ft_rl_addkey("<M-C-U-left>", KEY_ALT_CTRL_UP_LEFT);
	ft_rl_addkey("<M-C-U-right>", KEY_ALT_CTRL_UP_RIGHT);
	ft_rl_addkey("<ESC>", KEY_ESC);
	ft_rl_addkey("<TAB>", KEY_TAB);
	ft_rl_addkey("<ENT>", KEY_ENTER);
	ft_rl_addkey("<RET>", KEY_RETURN);
	ft_rl_addkey("<INS>", KEY_INS);
	ft_rl_addkey("<HME>", KEY_HOME);
	ft_rl_addkey("<PGU>", KEY_PGUP);
	ft_rl_addkey("<DEL>", KEY_DEL);
	ft_rl_addkey("<END>", KEY_END);
	ft_rl_addkey("<PGD>", KEY_PGDN);
	ft_rl_addkey("<U-DEL>", KEY_UP_DEL);
	ft_rl_addkey("<M-TAB>", KEY_ALT_TAB);
	ft_rl_addkey("<M-RET>", KEY_ALT_RETURN);
	ft_rl_addkey("<M-INS>", KEY_ALT_INS);
	ft_rl_addkey("<M-HME>", KEY_ALT_HOME);
	ft_rl_addkey("<M-PGU>", KEY_ALT_PGUP);
	ft_rl_addkey("<M-DEL>", KEY_ALT_DEL);
	ft_rl_addkey("<M-END>", KEY_ALT_END);
	ft_rl_addkey("<M-PGD>", KEY_ALT_PGDN);
	ft_rl_addkey("<M-U-INS>", KEY_ALT_UP_INS);
	ft_rl_addkey("<M-U-HME>", KEY_ALT_UP_HOME);
	ft_rl_addkey("<M-U-PGU>", KEY_ALT_UP_PGUP);
	ft_rl_addkey("<M-U-DEL>", KEY_ALT_UP_DEL);
	ft_rl_addkey("<M-U-END>", KEY_ALT_UP_END);
	ft_rl_addkey("<M-U-PGD>", KEY_ALT_UP_PGDN);
	ft_rl_addkey("<C-U-INS>", KEY_CTRL_UP_INS);
	ft_rl_addkey("<C-U-HME>", KEY_CTRL_UP_HOME);
	ft_rl_addkey("<C-U-PGU>", KEY_CTRL_UP_PGUP);
	ft_rl_addkey("<C-U-DEL>", KEY_CTRL_UP_DEL);
	ft_rl_addkey("<C-U-END>", KEY_CTRL_UP_END);
	ft_rl_addkey("<C-U-PGD>", KEY_CTRL_UP_PGDN);
	ft_rl_addkey("<C-U-INS>", KEY_CTRL_UP_INS);
	ft_rl_addkey("<C-U-HME>", KEY_CTRL_UP_HOME);
	ft_rl_addkey("<C-U-PGU>", KEY_CTRL_UP_PGUP);
	ft_rl_addkey("<C-U-DEL>", KEY_CTRL_UP_DEL);
	ft_rl_addkey("<C-U-END>", KEY_CTRL_UP_END);
	ft_rl_addkey("<C-U-PGD>", KEY_CTRL_UP_PGDN);
	ft_rl_addkey("<M-C-INS>", KEY_ALT_CTRL_INS);
	ft_rl_addkey("<M-C-HME>", KEY_ALT_CTRL_HOME);
	ft_rl_addkey("<M-C-PGU>", KEY_ALT_CTRL_PGUP);
	ft_rl_addkey("<M-C-DEL>", KEY_ALT_CTRL_DEL);
	ft_rl_addkey("<M-C-END>", KEY_ALT_CTRL_END);
	ft_rl_addkey("<M-C-PGD>", KEY_ALT_CTRL_PGDN);
	ft_rl_addkey("<M-C-U-INS>", KEY_ALT_CTRL_UP_INS);
	ft_rl_addkey("<M-C-U-HME>", KEY_ALT_CTRL_UP_HOME);
	ft_rl_addkey("<M-C-U-PGU>", KEY_ALT_CTRL_UP_PGUP);
	ft_rl_addkey("<M-C-U-DEL>", KEY_ALT_CTRL_UP_DEL);
	ft_rl_addkey("<M-C-U-END>", KEY_ALT_CTRL_UP_END);
	ft_rl_addkey("<M-C-U-PGD>", KEY_ALT_CTRL_UP_PGDN);
}
