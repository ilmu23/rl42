/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initkeys.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:44:17 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 18:59:43 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_ft_rl_initkeys2(void);
static inline void	_ft_rl_initkeys3(void);

void	ft_rl_initkeys(void)
{
	char	key[6];
	uint8_t	val;

	ft_rl_addkey("<SPC>", KEY_SPACE);
	ft_strlcpy(key, "!", 2);
	val = KEY_BANG;
	while (val <= KEY_TILDE)
	{
		ft_rl_addkey(key, val++);
		*key = val;
	}
	ft_rl_addkey("<BCK>", KEY_BACKSPACE);
	ft_strlcpy(key, "<C-a>", 6);
	val = KEY_CTRL_A;
	while (val <= KEY_CTRL_Z)
	{
		ft_rl_addkey(key, val++);
		key[3] = val + 0x60U;
	}
	ft_rl_addkey("<C-1>", KEY_CTRL_1);
	ft_rl_addkey("<C-2>", KEY_CTRL_2);
	ft_rl_addkey("<C-3>", KEY_CTRL_3);
	ft_rl_addkey("<C-4>", KEY_CTRL_4);
	ft_rl_addkey("<C-5>", KEY_CTRL_5);
	_ft_rl_initkeys2();
}

static inline void	_ft_rl_initkeys2(void)
{
	char		key[6];
	uint64_t	val;

	ft_rl_addkey("<C-6>", KEY_CTRL_6);
	ft_rl_addkey("<C-7>", KEY_CTRL_7);
	ft_rl_addkey("<C-8>", KEY_CTRL_8);
	ft_rl_addkey("<C-9>", KEY_CTRL_9);
	ft_rl_addkey("<M-SPC>", KEY_ALT_SPACE);
	ft_strlcpy(key, "<M-!>", 6);
	val = KEY_ALT_BANG;
	while (val <= KEY_ALT_TILDE)
	{
		ft_rl_addkey(key, val);
		val += 0x1U << 8;
		key[3] = val >> 8;
	}
	ft_rl_addkey("<M-BCK>", KEY_ALT_BACKSPACE);
	ft_rl_addkey("<up>", KEY_UP);
	ft_rl_addkey("<down>", KEY_DOWN);
	ft_rl_addkey("<left>", KEY_LEFT);
	ft_rl_addkey("<right>", KEY_RIGHT);
	ft_rl_addkey("<ESC>", KEY_ESC);
	ft_rl_addkey("<TAB>", KEY_TAB);
	ft_rl_addkey("<RET>", KEY_RETURN);
	_ft_rl_initkeys3();
}

static inline void	_ft_rl_initkeys3(void)
{
	ft_rl_addkey("<M-TAB>", KEY_ALT_TAB);
	ft_rl_addkey("<M-RET>", KEY_ALT_RETURN);
	ft_rl_addkey("<PRT>", KEY_PRINT);
	ft_rl_addkey("<SCR>", KEY_SCRLL);
	ft_rl_addkey("<PSE>", KEY_PAUSE);
	ft_rl_addkey("<INS>", KEY_INS);
	ft_rl_addkey("<HME>", KEY_HOME);
	ft_rl_addkey("<PGU>", KEY_PGUP);
	ft_rl_addkey("<DEL>", KEY_DEL);
	ft_rl_addkey("<END>", KEY_END);
	ft_rl_addkey("<PGD>", KEY_PGDN);
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
}
