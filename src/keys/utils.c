/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/02 13:33:17 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline t_hmap	*_getcurmap(void);
static inline void		_altctrl(char *keystr, const uint64_t key);
static inline void		_ascii(char *keystr, const uint64_t key);
static inline void		_ctrl(char *keystr, const uint64_t key);

uint8_t	ft_rl_ismapped(const uint64_t key)
{
	rl_map_t	*mapping;

	mapping = __mapget(_getcurmap(), ft_rl_keystr(key));
	return (mapping != NULL);
}

uint8_t	ft_rl_iskey(const uint64_t key)
{
	const char	*keystr;

	keystr = ft_rl_keystr(key);
	return (*keystr != '\0');
}

rl_fn_t	ft_rl_getmap(const uint64_t key)
{
	rl_map_t	*mapping;

	mapping = __mapget(_getcurmap(), ft_rl_keystr(key));
	if (mapping)
		return (*mapping->f);
	return (NULL);
}

char	*ft_rl_keystr(const uint64_t key)
{
	static char	keystr[_KEYSTR_LEN];

	memset(keystr, 0, 2);
	if (key == KEY_ESC)
		__strlcpy(keystr, "<ESC>", 6);
	else if (key >= KEY_BANG && key <= KEY_TILDE)
		_ascii(keystr, key);
	else if (key == KEY_CTRL_AT || (key >= KEY_CTRL_A && key <= KEY_CTRL_Z))
		_ctrl(keystr, key);
	else if ((key & _KEYSHIFT_MASK) == 0x1BULL
			&& (key >> 8 == KEY_CTRL_AT || (key >> 8 >= KEY_CTRL_A && key >> 8 <= KEY_CTRL_Z)))
		_altctrl(keystr, key);
	else switch (key)
	{
		case KEY_SPACE:
			__strlcpy(keystr, "<SPC>", 6);
			break ;
		case KEY_BACKSPACE:
			__strlcpy(keystr, "<BCK>", 6);
			break ;
		case KEY_ALT_SPACE:
			__strlcpy(keystr, "<M-SPC>", 8);
			break ;
		case KEY_ALT_BANG:
			__strlcpy(keystr, "<M-!>", 6);
			break ;
		case KEY_ALT_DQUOTE:
			__strlcpy(keystr, "<M-\">", 6);
			break ;
		case KEY_ALT_POUND:
			__strlcpy(keystr, "<M-#>", 6);
			break ;
		case KEY_ALT_DOLLAR:
			__strlcpy(keystr, "<M-$>", 6);
			break ;
		case KEY_ALT_PERCENT:
			__strlcpy(keystr, "<M-%>", 6);
			break ;
		case KEY_ALT_AMPERSAND:
			__strlcpy(keystr, "<M-&>", 6);
			break ;
		case KEY_ALT_SQUOTE:
			__strlcpy(keystr, "<M-'>", 6);
			break ;
		case KEY_ALT_OPARENT:
			__strlcpy(keystr, "<M-(>", 6);
			break ;
		case KEY_ALT_CPARENT:
			__strlcpy(keystr, "<M-)>", 6);
			break ;
		case KEY_ALT_ASTERISK:
			__strlcpy(keystr, "<M-*>", 6);
			break ;
		case KEY_ALT_PLUS:
			__strlcpy(keystr, "<M-+>", 6);
			break ;
		case KEY_ALT_COMMA:
			__strlcpy(keystr, "<M-,>", 6);
			break ;
		case KEY_ALT_MINUS:
			__strlcpy(keystr, "<M-->", 6);
			break ;
		case KEY_ALT_PERIOD:
			__strlcpy(keystr, "<M-.>", 6);
			break ;
		case KEY_ALT_SLASH:
			__strlcpy(keystr, "<M-/>", 6);
			break ;
		case KEY_ALT_NUM_0:
			__strlcpy(keystr, "<M-0>", 6);
			break ;
		case KEY_ALT_NUM_1:
			__strlcpy(keystr, "<M-1>", 6);
			break ;
		case KEY_ALT_NUM_2:
			__strlcpy(keystr, "<M-2>", 6);
			break ;
		case KEY_ALT_NUM_3:
			__strlcpy(keystr, "<M-3>", 6);
			break ;
		case KEY_ALT_NUM_4:
			__strlcpy(keystr, "<M-4>", 6);
			break ;
		case KEY_ALT_NUM_5:
			__strlcpy(keystr, "<M-5>", 6);
			break ;
		case KEY_ALT_NUM_6:
			__strlcpy(keystr, "<M-6>", 6);
			break ;
		case KEY_ALT_NUM_7:
			__strlcpy(keystr, "<M-7>", 6);
			break ;
		case KEY_ALT_NUM_8:
			__strlcpy(keystr, "<M-8>", 6);
			break ;
		case KEY_ALT_NUM_9:
			__strlcpy(keystr, "<M-9>", 6);
			break ;
		case KEY_ALT_COLON:
			__strlcpy(keystr, "<M-:>", 6);
			break ;
		case KEY_ALT_SCOLON:
			__strlcpy(keystr, "<M-;>", 6);
			break ;
		case KEY_ALT_LTHAN:
			__strlcpy(keystr, "<M-<>", 6);
			break ;
		case KEY_ALT_EQUAL:
			__strlcpy(keystr, "<M-=>", 6);
			break ;
		case KEY_ALT_MTHAN:
			__strlcpy(keystr, "<M->>", 6);
			break ;
		case KEY_ALT_QUESTION:
			__strlcpy(keystr, "<M-?>", 6);
			break ;
		case KEY_ALT_AT:
			__strlcpy(keystr, "<M-@>", 6);
			break ;
		case KEY_ALT_UP_A:
			__strlcpy(keystr, "<M-A>", 6);
			break ;
		case KEY_ALT_UP_B:
			__strlcpy(keystr, "<M-B>", 6);
			break ;
		case KEY_ALT_UP_C:
			__strlcpy(keystr, "<M-C>", 6);
			break ;
		case KEY_ALT_UP_D:
			__strlcpy(keystr, "<M-D>", 6);
			break ;
		case KEY_ALT_UP_E:
			__strlcpy(keystr, "<M-E>", 6);
			break ;
		case KEY_ALT_UP_F:
			__strlcpy(keystr, "<M-F>", 6);
			break ;
		case KEY_ALT_UP_G:
			__strlcpy(keystr, "<M-G>", 6);
			break ;
		case KEY_ALT_UP_H:
			__strlcpy(keystr, "<M-H>", 6);
			break ;
		case KEY_ALT_UP_I:
			__strlcpy(keystr, "<M-I>", 6);
			break ;
		case KEY_ALT_UP_J:
			__strlcpy(keystr, "<M-J>", 6);
			break ;
		case KEY_ALT_UP_K:
			__strlcpy(keystr, "<M-K>", 6);
			break ;
		case KEY_ALT_UP_L:
			__strlcpy(keystr, "<M-L>", 6);
			break ;
		case KEY_ALT_UP_M:
			__strlcpy(keystr, "<M-M>", 6);
			break ;
		case KEY_ALT_UP_N:
			__strlcpy(keystr, "<M-N>", 6);
			break ;
		case KEY_ALT_UP_O:
			__strlcpy(keystr, "<M-O>", 6);
			break ;
		case KEY_ALT_UP_P:
			__strlcpy(keystr, "<M-P>", 6);
			break ;
		case KEY_ALT_UP_Q:
			__strlcpy(keystr, "<M-Q>", 6);
			break ;
		case KEY_ALT_UP_R:
			__strlcpy(keystr, "<M-R>", 6);
			break ;
		case KEY_ALT_UP_S:
			__strlcpy(keystr, "<M-S>", 6);
			break ;
		case KEY_ALT_UP_T:
			__strlcpy(keystr, "<M-T>", 6);
			break ;
		case KEY_ALT_UP_U:
			__strlcpy(keystr, "<M-U>", 6);
			break ;
		case KEY_ALT_UP_V:
			__strlcpy(keystr, "<M-V>", 6);
			break ;
		case KEY_ALT_UP_W:
			__strlcpy(keystr, "<M-W>", 6);
			break ;
		case KEY_ALT_UP_X:
			__strlcpy(keystr, "<M-X>", 6);
			break ;
		case KEY_ALT_UP_Y:
			__strlcpy(keystr, "<M-Y>", 6);
			break ;
		case KEY_ALT_UP_Z:
			__strlcpy(keystr, "<M-Z>", 6);
			break ;
		case KEY_ALT_OSBRACKET:
			__strlcpy(keystr, "<M-[>", 6);
			break ;
		case KEY_ALT_BACKSLASH:
			__strlcpy(keystr, "<M-\\>", 6);
			break ;
		case KEY_ALT_CSBRACKET:
			__strlcpy(keystr, "<M-]>", 6);
			break ;
		case KEY_ALT_CARET:
			__strlcpy(keystr, "<M-^>", 6);
			break ;
		case KEY_ALT_UNDERSCORE:
			__strlcpy(keystr, "<M-_>", 6);
			break ;
		case KEY_ALT_BACKTICK:
			__strlcpy(keystr, "<M-`>", 6);
			break ;
		case KEY_ALT_DN_A:
			__strlcpy(keystr, "<M-a>", 6);
			break ;
		case KEY_ALT_DN_B:
			__strlcpy(keystr, "<M-b>", 6);
			break ;
		case KEY_ALT_DN_C:
			__strlcpy(keystr, "<M-c>", 6);
			break ;
		case KEY_ALT_DN_D:
			__strlcpy(keystr, "<M-d>", 6);
			break ;
		case KEY_ALT_DN_E:
			__strlcpy(keystr, "<M-e>", 6);
			break ;
		case KEY_ALT_DN_F:
			__strlcpy(keystr, "<M-f>", 6);
			break ;
		case KEY_ALT_DN_G:
			__strlcpy(keystr, "<M-g>", 6);
			break ;
		case KEY_ALT_DN_H:
			__strlcpy(keystr, "<M-h>", 6);
			break ;
		case KEY_ALT_DN_I:
			__strlcpy(keystr, "<M-i>", 6);
			break ;
		case KEY_ALT_DN_J:
			__strlcpy(keystr, "<M-j>", 6);
			break ;
		case KEY_ALT_DN_K:
			__strlcpy(keystr, "<M-k>", 6);
			break ;
		case KEY_ALT_DN_L:
			__strlcpy(keystr, "<M-l>", 6);
			break ;
		case KEY_ALT_DN_M:
			__strlcpy(keystr, "<M-m>", 6);
			break ;
		case KEY_ALT_DN_N:
			__strlcpy(keystr, "<M-n>", 6);
			break ;
		case KEY_ALT_DN_O:
			__strlcpy(keystr, "<M-o>", 6);
			break ;
		case KEY_ALT_DN_P:
			__strlcpy(keystr, "<M-p>", 6);
			break ;
		case KEY_ALT_DN_Q:
			__strlcpy(keystr, "<M-q>", 6);
			break ;
		case KEY_ALT_DN_R:
			__strlcpy(keystr, "<M-r>", 6);
			break ;
		case KEY_ALT_DN_S:
			__strlcpy(keystr, "<M-s>", 6);
			break ;
		case KEY_ALT_DN_T:
			__strlcpy(keystr, "<M-t>", 6);
			break ;
		case KEY_ALT_DN_U:
			__strlcpy(keystr, "<M-u>", 6);
			break ;
		case KEY_ALT_DN_V:
			__strlcpy(keystr, "<M-v>", 6);
			break ;
		case KEY_ALT_DN_W:
			__strlcpy(keystr, "<M-w>", 6);
			break ;
		case KEY_ALT_DN_X:
			__strlcpy(keystr, "<M-x>", 6);
			break ;
		case KEY_ALT_DN_Y:
			__strlcpy(keystr, "<M-y>", 6);
			break ;
		case KEY_ALT_DN_Z:
			__strlcpy(keystr, "<M-z>", 6);
			break ;
		case KEY_ALT_OCBRACKET:
			__strlcpy(keystr, "<M-{>", 6);
			break ;
		case KEY_ALT_PIPE:
			__strlcpy(keystr, "<M-|>", 6);
			break ;
		case KEY_ALT_CCBRACKET:
			__strlcpy(keystr, "<M-}>", 6);
			break ;
		case KEY_ALT_TILDE:
			__strlcpy(keystr, "<M-~>", 6);
			break ;
		case KEY_ALT_BACKSPACE:
			__strlcpy(keystr, "<M-BCK>", 8);
			break ;
		case KEY_F_1:
			__strlcpy(keystr, "<F-1>", 6);
			break ;
		case KEY_F_2:
			__strlcpy(keystr, "<F-2>", 6);
			break ;
		case KEY_F_3:
			__strlcpy(keystr, "<F-3>", 6);
			break ;
		case KEY_F_4:
			__strlcpy(keystr, "<F-4>", 6);
			break ;
		case KEY_F_5:
			__strlcpy(keystr, "<F-5>", 6);
			break ;
		case KEY_F_6:
			__strlcpy(keystr, "<F-6>", 6);
			break ;
		case KEY_F_7:
			__strlcpy(keystr, "<F-7>", 6);
			break ;
		case KEY_F_8:
			__strlcpy(keystr, "<F-8>", 6);
			break ;
		case KEY_F_9:
			__strlcpy(keystr, "<F-9>", 6);
			break ;
		case KEY_F_10:
			__strlcpy(keystr, "<F-10>", 7);
			break ;
		case KEY_F_11:
			__strlcpy(keystr, "<F-11>", 7);
			break ;
		case KEY_F_12:
			__strlcpy(keystr, "<F-12>", 7);
			break ;
		case KEY_UP_F_1:
			__strlcpy(keystr, "<U-F-1>", 8);
			break ;
		case KEY_UP_F_2:
			__strlcpy(keystr, "<U-F-2>", 8);
			break ;
		case KEY_UP_F_3:
			__strlcpy(keystr, "<U-F-3>", 8);
			break ;
		case KEY_UP_F_4:
			__strlcpy(keystr, "<U-F-4>", 8);
			break ;
		case KEY_UP_F_5:
			__strlcpy(keystr, "<U-F-5>", 8);
			break ;
		case KEY_UP_F_6:
			__strlcpy(keystr, "<U-F-6>", 8);
			break ;
		case KEY_UP_F_7:
			__strlcpy(keystr, "<U-F-7>", 8);
			break ;
		case KEY_UP_F_8:
			__strlcpy(keystr, "<U-F-8>", 8);
			break ;
		case KEY_UP_F_9:
			__strlcpy(keystr, "<U-F-9>", 8);
			break ;
		case KEY_UP_F_10:
			__strlcpy(keystr, "<U-F-10>", 9);
			break ;
		case KEY_UP_F_11:
			__strlcpy(keystr, "<U-F-11>", 9);
			break ;
		case KEY_UP_F_12:
			__strlcpy(keystr, "<U-F-12>", 9);
			break ;
		case KEY_ALT_F_1:
			__strlcpy(keystr, "<M-F-1>", 8);
			break ;
		case KEY_ALT_F_2:
			__strlcpy(keystr, "<M-F-2>", 8);
			break ;
		case KEY_ALT_F_3:
			__strlcpy(keystr, "<M-F-3>", 8);
			break ;
		case KEY_ALT_F_4:
			__strlcpy(keystr, "<M-F-4>", 8);
			break ;
		case KEY_ALT_F_5:
			__strlcpy(keystr, "<M-F-5>", 8);
			break ;
		case KEY_ALT_F_6:
			__strlcpy(keystr, "<M-F-6>", 8);
			break ;
		case KEY_ALT_F_7:
			__strlcpy(keystr, "<M-F-7>", 8);
			break ;
		case KEY_ALT_F_8:
			__strlcpy(keystr, "<M-F-8>", 8);
			break ;
		case KEY_ALT_F_9:
			__strlcpy(keystr, "<M-F-9>", 8);
			break ;
		case KEY_ALT_F_10:
			__strlcpy(keystr, "<M-F-10>", 9);
			break ;
		case KEY_ALT_F_11:
			__strlcpy(keystr, "<M-F-11>", 9);
			break ;
		case KEY_ALT_F_12:
			__strlcpy(keystr, "<M-F-12>", 9);
			break ;
		case KEY_ALT_UP_F_1:
			__strlcpy(keystr, "<M-U-F-1>", 10);
			break ;
		case KEY_ALT_UP_F_2:
			__strlcpy(keystr, "<M-U-F-2>", 10);
			break ;
		case KEY_ALT_UP_F_3:
			__strlcpy(keystr, "<M-U-F-3>", 10);
			break ;
		case KEY_ALT_UP_F_4:
			__strlcpy(keystr, "<M-U-F-4>", 10);
			break ;
		case KEY_ALT_UP_F_5:
			__strlcpy(keystr, "<M-U-F-5>", 10);
			break ;
		case KEY_ALT_UP_F_6:
			__strlcpy(keystr, "<M-U-F-6>", 10);
			break ;
		case KEY_ALT_UP_F_7:
			__strlcpy(keystr, "<M-U-F-7>", 10);
			break ;
		case KEY_ALT_UP_F_8:
			__strlcpy(keystr, "<M-U-F-8>", 10);
			break ;
		case KEY_ALT_UP_F_9:
			__strlcpy(keystr, "<M-U-F-9>", 10);
			break ;
		case KEY_ALT_UP_F_10:
			__strlcpy(keystr, "<M-U-F-10>", 11);
			break ;
		case KEY_ALT_UP_F_11:
			__strlcpy(keystr, "<M-U-F-11>", 11);
			break ;
		case KEY_ALT_UP_F_12:
			__strlcpy(keystr, "<M-U-F-12>", 11);
			break ;
		case KEY_CTRL_F_1:
			__strlcpy(keystr, "<C-F-1>", 8);
			break ;
		case KEY_CTRL_F_2:
			__strlcpy(keystr, "<C-F-2>", 8);
			break ;
		case KEY_CTRL_F_3:
			__strlcpy(keystr, "<C-F-3>", 8);
			break ;
		case KEY_CTRL_F_4:
			__strlcpy(keystr, "<C-F-4>", 8);
			break ;
		case KEY_CTRL_F_5:
			__strlcpy(keystr, "<C-F-5>", 8);
			break ;
		case KEY_CTRL_F_6:
			__strlcpy(keystr, "<C-F-6>", 8);
			break ;
		case KEY_CTRL_F_7:
			__strlcpy(keystr, "<C-F-7>", 8);
			break ;
		case KEY_CTRL_F_8:
			__strlcpy(keystr, "<C-F-8>", 8);
			break ;
		case KEY_CTRL_F_9:
			__strlcpy(keystr, "<C-F-9>", 8);
			break ;
		case KEY_CTRL_F_10:
			__strlcpy(keystr, "<C-F-10>", 9);
			break ;
		case KEY_CTRL_F_11:
			__strlcpy(keystr, "<C-F-11>", 9);
			break ;
		case KEY_CTRL_F_12:
			__strlcpy(keystr, "<C-F-12>", 9);
			break ;
		case KEY_CTRL_UP_F_1:
			__strlcpy(keystr, "<C-U-F-1>", 10);
			break ;
		case KEY_CTRL_UP_F_2:
			__strlcpy(keystr, "<C-U-F-2>", 10);
			break ;
		case KEY_CTRL_UP_F_3:
			__strlcpy(keystr, "<C-U-F-3>", 10);
			break ;
		case KEY_CTRL_UP_F_4:
			__strlcpy(keystr, "<C-U-F-4>", 10);
			break ;
		case KEY_CTRL_UP_F_5:
			__strlcpy(keystr, "<C-U-F-5>", 10);
			break ;
		case KEY_CTRL_UP_F_6:
			__strlcpy(keystr, "<C-U-F-6>", 10);
			break ;
		case KEY_CTRL_UP_F_7:
			__strlcpy(keystr, "<C-U-F-7>", 10);
			break ;
		case KEY_CTRL_UP_F_8:
			__strlcpy(keystr, "<C-U-F-8>", 10);
			break ;
		case KEY_CTRL_UP_F_9:
			__strlcpy(keystr, "<C-U-F-9>", 10);
			break ;
		case KEY_CTRL_UP_F_10:
			__strlcpy(keystr, "<C-U-F-10>", 11);
			break ;
		case KEY_CTRL_UP_F_11:
			__strlcpy(keystr, "<C-U-F-11>", 11);
			break ;
		case KEY_CTRL_UP_F_12:
			__strlcpy(keystr, "<C-U-F-12>", 11);
			break ;
		case KEY_ALT_CTRL_UP_F_1:
			__strlcpy(keystr, "<M-C-U-F-1>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_2:
			__strlcpy(keystr, "<M-C-U-F-2>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_3:
			__strlcpy(keystr, "<M-C-U-F-3>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_4:
			__strlcpy(keystr, "<M-C-U-F-4>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_5:
			__strlcpy(keystr, "<M-C-U-F-5>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_6:
			__strlcpy(keystr, "<M-C-U-F-6>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_7:
			__strlcpy(keystr, "<M-C-U-F-7>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_8:
			__strlcpy(keystr, "<M-C-U-F-8>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_9:
			__strlcpy(keystr, "<M-C-U-F-9>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_10:
			__strlcpy(keystr, "<M-C-U-F-10>", 13);
			break ;
		case KEY_ALT_CTRL_UP_F_11:
			__strlcpy(keystr, "<M-C-U-F-11>", 13);
			break ;
		case KEY_ALT_CTRL_UP_F_12:
			__strlcpy(keystr, "<M-C-U-F-12>", 13);
			break ;
		case KEY_UP:
			__strlcpy(keystr, "<up>", 5);
			break ;
		case KEY_DOWN:
			__strlcpy(keystr, "<down>", 7);
			break ;
		case KEY_RIGHT:
			__strlcpy(keystr, "<right>", 8);
			break ;
		case KEY_LEFT:
			__strlcpy(keystr, "<left>", 7);
			break ;
		case KEY_UP_UP:
			__strlcpy(keystr, "<U-up>", 7);
			break ;
		case KEY_UP_DOWN:
			__strlcpy(keystr, "<U-down>", 9);
			break ;
		case KEY_UP_RIGHT:
			__strlcpy(keystr, "<U-right>", 10);
			break ;
		case KEY_UP_LEFT:
			__strlcpy(keystr, "<U-left>", 9);
			break ;
		case KEY_ALT_UP:
			__strlcpy(keystr, "<M-up>", 7);
			break ;
		case KEY_ALT_DOWN:
			__strlcpy(keystr, "<M-down>", 9);
			break ;
		case KEY_ALT_RIGHT:
			__strlcpy(keystr, "<M-right>", 10);
			break ;
		case KEY_ALT_LEFT:
			__strlcpy(keystr, "<M-left>", 9);
			break ;
		case KEY_ALT_UP_UP:
			__strlcpy(keystr, "<M-U-up>", 9);
			break ;
		case KEY_ALT_UP_DOWN:
			__strlcpy(keystr, "<M-U-down>", 11);
			break ;
		case KEY_ALT_UP_RIGHT:
			__strlcpy(keystr, "<M-U-right>", 12);
			break ;
		case KEY_ALT_UP_LEFT:
			__strlcpy(keystr, "<M-U-left>", 11);
			break ;
		case KEY_CTRL_UP:
			__strlcpy(keystr, "<C-up>", 7);
			break ;
		case KEY_CTRL_DOWN:
			__strlcpy(keystr, "<C-down>", 9);
			break ;
		case KEY_CTRL_RIGHT:
			__strlcpy(keystr, "<C-right>", 10);
			break ;
		case KEY_CTRL_LEFT:
			__strlcpy(keystr, "<C-left>", 9);
			break ;
		case KEY_CTRL_UP_UP:
			__strlcpy(keystr, "<C-U-up>", 9);
			break ;
		case KEY_CTRL_UP_DOWN:
			__strlcpy(keystr, "<C-U-down>", 11);
			break ;
		case KEY_CTRL_UP_RIGHT:
			__strlcpy(keystr, "<C-U-right>", 12);
			break ;
		case KEY_CTRL_UP_LEFT:
			__strlcpy(keystr, "<C-U-left>", 11);
			break ;
		case KEY_ALT_CTRL_UP:
			__strlcpy(keystr, "<M-C-up>", 9);
			break ;
		case KEY_ALT_CTRL_DOWN:
			__strlcpy(keystr, "<M-C-down>", 11);
			break ;
		case KEY_ALT_CTRL_RIGHT:
			__strlcpy(keystr, "<M-C-right>", 12);
			break ;
		case KEY_ALT_CTRL_LEFT:
			__strlcpy(keystr, "<M-C-left>", 11);
			break ;
		case KEY_ALT_CTRL_UP_UP:
			__strlcpy(keystr, "<M-C-U-up>", 11);
			break ;
		case KEY_ALT_CTRL_UP_DOWN:
			__strlcpy(keystr, "<M-C-U-down>", 13);
			break ;
		case KEY_ALT_CTRL_UP_RIGHT:
			__strlcpy(keystr, "<M-C-U-right>", 14);
			break ;
		case KEY_ALT_CTRL_UP_LEFT:
			__strlcpy(keystr, "<M-C-U-left>", 13);
			break ;
		case KEY_INS:
			__strlcpy(keystr, "<INS>", 6);
			break ;
		case KEY_HOME:
			__strlcpy(keystr, "<HME>", 6);
			break ;
		case KEY_PGUP:
			__strlcpy(keystr, "<PGU>", 6);
			break ;
		case KEY_DEL:
			__strlcpy(keystr, "<DEL>", 6);
			break ;
		case KEY_END:
			__strlcpy(keystr, "<END>", 6);
			break ;
		case KEY_PGDN:
			__strlcpy(keystr, "<PGD>", 6);
			break ;
		case KEY_UP_DEL:
			__strlcpy(keystr, "<U-DEL>", 6);
			break ;
		case KEY_ALT_INS:
			__strlcpy(keystr, "<M-INS>", 8);
			break ;
		case KEY_ALT_HOME:
			__strlcpy(keystr, "<M-HME>", 8);
			break ;
		case KEY_ALT_PGUP:
			__strlcpy(keystr, "<M-PGU>", 8);
			break ;
		case KEY_ALT_DEL:
			__strlcpy(keystr, "<M-DEL>", 8);
			break ;
		case KEY_ALT_END:
			__strlcpy(keystr, "<M-END>", 8);
			break ;
		case KEY_ALT_PGDN:
			__strlcpy(keystr, "<M-PGD>", 8);
			break ;
		case KEY_ALT_UP_INS:
			__strlcpy(keystr, "<M-U-INS>", 10);
			break ;
		case KEY_ALT_UP_HOME:
			__strlcpy(keystr, "<M-U-HME>", 10);
			break ;
		case KEY_ALT_UP_PGUP:
			__strlcpy(keystr, "<M-U-PGU>", 10);
			break ;
		case KEY_ALT_UP_DEL:
			__strlcpy(keystr, "<M-U-DEL>", 10);
			break ;
		case KEY_ALT_UP_END:
			__strlcpy(keystr, "<M-U-END>", 10);
			break ;
		case KEY_ALT_UP_PGDN:
			__strlcpy(keystr, "<M-U-PGD>", 10);
			break ;
		case KEY_CTRL_INS:
			__strlcpy(keystr, "<C-INS>", 8);
			break ;
		case KEY_CTRL_HOME:
			__strlcpy(keystr, "<C-HME>", 8);
			break ;
		case KEY_CTRL_PGUP:
			__strlcpy(keystr, "<C-PGU>", 8);
			break ;
		case KEY_CTRL_DEL:
			__strlcpy(keystr, "<C-DEL>", 8);
			break ;
		case KEY_CTRL_END:
			__strlcpy(keystr, "<C-END>", 8);
			break ;
		case KEY_CTRL_PGDN:
			__strlcpy(keystr, "<C-PGD>", 8);
			break ;
		case KEY_CTRL_UP_INS:
			__strlcpy(keystr, "<C-U-INS>", 10);
			break ;
		case KEY_CTRL_UP_HOME:
			__strlcpy(keystr, "<C-U-HME>", 10);
			break ;
		case KEY_CTRL_UP_PGUP:
			__strlcpy(keystr, "<C-U-PGU>", 10);
			break ;
		case KEY_CTRL_UP_DEL:
			__strlcpy(keystr, "<C-U-DEL>", 10);
			break ;
		case KEY_CTRL_UP_END:
			__strlcpy(keystr, "<C-U-END>", 10);
			break ;
		case KEY_CTRL_UP_PGDN:
			__strlcpy(keystr, "<C-U-PGD>", 10);
			break ;
		case KEY_ALT_CTRL_INS:
			__strlcpy(keystr, "<M-C-INS>", 10);
			break ;
		case KEY_ALT_CTRL_HOME:
			__strlcpy(keystr, "<M-C-HME>", 10);
			break ;
		case KEY_ALT_CTRL_PGUP:
			__strlcpy(keystr, "<M-C-PGU>", 10);
			break ;
		case KEY_ALT_CTRL_DEL:
			__strlcpy(keystr, "<M-C-DEL>", 10);
			break ;
		case KEY_ALT_CTRL_END:
			__strlcpy(keystr, "<M-C-END>", 10);
			break ;
		case KEY_ALT_CTRL_PGDN:
			__strlcpy(keystr, "<M-C-PGD>", 10);
			break ;
		case KEY_ALT_CTRL_UP_INS:
			__strlcpy(keystr, "<M-C-U-INS>", 12);
			break ;
		case KEY_ALT_CTRL_UP_HOME:
			__strlcpy(keystr, "<M-C-U-HME>", 12);
			break ;
		case KEY_ALT_CTRL_UP_PGUP:
			__strlcpy(keystr, "<M-C-U-PGU>", 12);
			break ;
		case KEY_ALT_CTRL_UP_DEL:
			__strlcpy(keystr, "<M-C-U-DEL>", 12);
			break ;
		case KEY_ALT_CTRL_UP_END:
			__strlcpy(keystr, "<M-C-U-END>", 12);
			break ;
		case KEY_ALT_CTRL_UP_PGDN:
			__strlcpy(keystr, "<M-C-U-PGD>", 12);
			break ;
	}
	return (keystr);
}

void	ft_rl_unmap(const char *key)
{
	__maprm(_getcurmap(), key);
}

void	ft_rl_unmap_fn(const char *func)
{
	t_hmap_pair	**items;
	rl_fn_t		*f;
	size_t		i;
	t_hmap		*map;

	map = _getcurmap();
	f = __mapget(g_funcs, func);
	if (!f)
		return ;
	i = 0;
	items = map->items;
	while (i < map->size)
	{
		if (items[i] && items[i] != (void *)&__hmap_sentinel
			&& ((rl_map_t *)(items[i]->value))->f == *f)
			__maprm(map, items[i]->key);
		i++;
	}
}

static inline t_hmap	*_getcurmap(void)
{
	switch(ft_rl_geteditmode())
	{
		case _MD_EMACS:
			return (g_map_emacs);
		case _MD_VI_INS:
			return (g_map_vi_ins);
		case _MD_VI_CMD:
			return (g_map_vi_cmd);
		case _MD_HLCOLOR:
			return (g_map_hlcolor);
	}
	return (NULL);
}

static inline void	_altctrl(char *keystr, const uint64_t key)
{
	switch (key)
	{
		case KEY_ALT_TAB:
			__strlcpy(keystr, "<M-TAB>", 6);
			break ;
		case KEY_ALT_RETURN:
			__strlcpy(keystr, "<M-RET>", 6);
			break ;
		case KEY_ALT_CTRL_AT:
			__strlcpy(keystr, "<M-C-@>", 8);
			break ;
		case KEY_ALT_CTRL_A:
			__strlcpy(keystr, "<M-C-a>", 8);
			break ;
		case KEY_ALT_CTRL_B:
			__strlcpy(keystr, "<M-C-b>", 8);
			break ;
		case KEY_ALT_CTRL_C:
			__strlcpy(keystr, "<M-C-c>", 8);
			break ;
		case KEY_ALT_CTRL_D:
			__strlcpy(keystr, "<M-C-d>", 8);
			break ;
		case KEY_ALT_CTRL_E:
			__strlcpy(keystr, "<M-C-e>", 8);
			break ;
		case KEY_ALT_CTRL_F:
			__strlcpy(keystr, "<M-C-f>", 8);
			break ;
		case KEY_ALT_CTRL_G:
			__strlcpy(keystr, "<M-C-g>", 8);
			break ;
		case KEY_ALT_CTRL_H:
			__strlcpy(keystr, "<M-C-h>", 8);
			break ;
		case KEY_ALT_CTRL_J:
			__strlcpy(keystr, "<M-C-j>", 8);
			break ;
		case KEY_ALT_CTRL_K:
			__strlcpy(keystr, "<M-C-k>", 8);
			break ;
		case KEY_ALT_CTRL_L:
			__strlcpy(keystr, "<M-C-l>", 8);
			break ;
		case KEY_ALT_CTRL_N:
			__strlcpy(keystr, "<M-C-n>", 8);
			break ;
		case KEY_ALT_CTRL_O:
			__strlcpy(keystr, "<M-C-o>", 8);
			break ;
		case KEY_ALT_CTRL_P:
			__strlcpy(keystr, "<M-C-p>", 8);
			break ;
		case KEY_ALT_CTRL_Q:
			__strlcpy(keystr, "<M-C-q>", 8);
			break ;
		case KEY_ALT_CTRL_R:
			__strlcpy(keystr, "<M-C-r>", 8);
			break ;
		case KEY_ALT_CTRL_S:
			__strlcpy(keystr, "<M-C-s>", 8);
			break ;
		case KEY_ALT_CTRL_T:
			__strlcpy(keystr, "<M-C-t>", 8);
			break ;
		case KEY_ALT_CTRL_U:
			__strlcpy(keystr, "<M-C-u>", 8);
			break ;
		case KEY_ALT_CTRL_V:
			__strlcpy(keystr, "<M-C-v>", 8);
			break ;
		case KEY_ALT_CTRL_W:
			__strlcpy(keystr, "<M-C-w>", 8);
			break ;
		case KEY_ALT_CTRL_X:
			__strlcpy(keystr, "<M-C-x>", 8);
			break ;
		case KEY_ALT_CTRL_Y:
			__strlcpy(keystr, "<M-C-y>", 8);
			break ;
		case KEY_ALT_CTRL_Z:
			__strlcpy(keystr, "<M-C-z>", 8);
			break ;
		case KEY_ALT_CTRL_OSBRACKET:
			__strlcpy(keystr, "<M-C-[>", 8);
			break ;
		case KEY_ALT_CTRL_BACKSLASH:
			__strlcpy(keystr, "<M-C-\\>", 8);
			break ;
		case KEY_ALT_CTRL_CSBRACKET:
			__strlcpy(keystr, "<M-C-]>", 8);
			break ;
		case KEY_ALT_CTRL_TILDE:
			__strlcpy(keystr, "<M-C-~>", 8);
			break ;
		case KEY_ALT_CTRL_UNDERSCORE:
			__strlcpy(keystr, "<M-C-_>", 8);
			break ;
	}
}

static inline void	_ascii(char *keystr, const uint64_t key)
{
	uint64_t	val;

	val = KEY_BANG;
	while (val < KEY_TILDE && val != key)
		val++;
	*keystr = val;
}

static inline void	_ctrl(char *keystr, const uint64_t key)
{
	switch (key)
	{
		case KEY_TAB:
			__strlcpy(keystr, "<TAB>", 6);
			break ;
		case KEY_ENTER:
			__strlcpy(keystr, "<ENT>", 6);
			break ;
		case KEY_RETURN:
			__strlcpy(keystr, "<RET>", 6);
			break ;
		case KEY_CTRL_AT:
			__strlcpy(keystr, "<C-@>", 6);
			break ;
		case KEY_CTRL_A:
			__strlcpy(keystr, "<C-a>", 6);
			break ;
		case KEY_CTRL_B:
			__strlcpy(keystr, "<C-b>", 6);
			break ;
		case KEY_CTRL_C:
			__strlcpy(keystr, "<C-c>", 6);
			break ;
		case KEY_CTRL_D:
			__strlcpy(keystr, "<C-d>", 6);
			break ;
		case KEY_CTRL_E:
			__strlcpy(keystr, "<C-e>", 6);
			break ;
		case KEY_CTRL_F:
			__strlcpy(keystr, "<C-f>", 6);
			break ;
		case KEY_CTRL_G:
			__strlcpy(keystr, "<C-g>", 6);
			break ;
		case KEY_CTRL_H:
			__strlcpy(keystr, "<C-h>", 6);
			break ;
		case KEY_CTRL_K:
			__strlcpy(keystr, "<C-k>", 6);
			break ;
		case KEY_CTRL_L:
			__strlcpy(keystr, "<C-l>", 6);
			break ;
		case KEY_CTRL_N:
			__strlcpy(keystr, "<C-n>", 6);
			break ;
		case KEY_CTRL_O:
			__strlcpy(keystr, "<C-o>", 6);
			break ;
		case KEY_CTRL_P:
			__strlcpy(keystr, "<C-o>", 6);
			break ;
		case KEY_CTRL_Q:
			__strlcpy(keystr, "<C-q>", 6);
			break ;
		case KEY_CTRL_R:
			__strlcpy(keystr, "<C-r>", 6);
			break ;
		case KEY_CTRL_S:
			__strlcpy(keystr, "<C-s>", 6);
			break ;
		case KEY_CTRL_T:
			__strlcpy(keystr, "<C-t>", 6);
			break ;
		case KEY_CTRL_U:
			__strlcpy(keystr, "<C-u>", 6);
			break ;
		case KEY_CTRL_V:
			__strlcpy(keystr, "<C-v>", 6);
			break ;
		case KEY_CTRL_W:
			__strlcpy(keystr, "<C-w>", 6);
			break ;
		case KEY_CTRL_X:
			__strlcpy(keystr, "<C-x>", 6);
			break ;
		case KEY_CTRL_Y:
			__strlcpy(keystr, "<C-y>", 6);
			break ;
		case KEY_CTRL_Z:
			__strlcpy(keystr, "<C-z>", 6);
			break ;
		case KEY_CTRL_OSBRACKET:
			__strlcpy(keystr, "<C-[>", 6);
			break ;
		case KEY_CTRL_BACKSLASH:
			__strlcpy(keystr, "<C-\\>", 6);
			break ;
		case KEY_CTRL_CSBRACKET:
			__strlcpy(keystr, "<C-]>", 6);
			break ;
		case KEY_CTRL_TILDE:
			__strlcpy(keystr, "<C-~>", 6);
			break ;
		case KEY_CTRL_UNDERSCORE:
			__strlcpy(keystr, "<C-_>", 6);
			break ;
	}
}
