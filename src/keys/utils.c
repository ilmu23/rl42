/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 05:31:24 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_ascii(char *keystr, const uint64_t key);
static inline void	_ctrl(char *keystr, const uint64_t key);

uint8_t	ft_rl_ismapped(const uint64_t key)
{
	rl_map_t	*mapping;

	mapping = ft_mapget(g_maps, ft_rl_keystr(key));
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

	mapping = ft_mapget(g_maps, ft_rl_keystr(key));
	if (mapping)
		return (*mapping->f);
	return (NULL);
}

char	*ft_rl_keystr(const uint64_t key)
{
	static char	keystr[_KEYSTR_LEN];

	ft_memset(keystr, 0, _KEYSTR_LEN);
	if (key >= KEY_BANG && key <= KEY_TILDE)
		_ascii(keystr, key);
	else if (key >= KEY_CTRL_A && key <= KEY_CTRL_Z)
		_ctrl(keystr, key);
	else switch (key)
	{
		case KEY_SPACE:
			ft_strlcpy(keystr, "<SPC>", 6);
			break ;
		case KEY_BACKSPACE:
			ft_strlcpy(keystr, "<BCK>", 6);
			break ;
		case KEY_ALT_SPACE:
			ft_strlcpy(keystr, "<M-SPC>", 8);
			break ;
		case KEY_ALT_BANG:
			ft_strlcpy(keystr, "<M-!>", 6);
			break ;
		case KEY_ALT_DQUOTE:
			ft_strlcpy(keystr, "<M-\">", 6);
			break ;
		case KEY_ALT_POUND:
			ft_strlcpy(keystr, "<M-#>", 6);
			break ;
		case KEY_ALT_DOLLAR:
			ft_strlcpy(keystr, "<M-$>", 6);
			break ;
		case KEY_ALT_PERCENT:
			ft_strlcpy(keystr, "<M-%>", 6);
			break ;
		case KEY_ALT_AMPERSAND:
			ft_strlcpy(keystr, "<M-&>", 6);
			break ;
		case KEY_ALT_SQUOTE:
			ft_strlcpy(keystr, "<M-'>", 6);
			break ;
		case KEY_ALT_OPARENT:
			ft_strlcpy(keystr, "<M-(>", 6);
			break ;
		case KEY_ALT_CPARENT:
			ft_strlcpy(keystr, "<M-)>", 6);
			break ;
		case KEY_ALT_ASTERISK:
			ft_strlcpy(keystr, "<M-*>", 6);
			break ;
		case KEY_ALT_PLUS:
			ft_strlcpy(keystr, "<M-+>", 6);
			break ;
		case KEY_ALT_COMMA:
			ft_strlcpy(keystr, "<M-,>", 6);
			break ;
		case KEY_ALT_MINUS:
			ft_strlcpy(keystr, "<M-->", 6);
			break ;
		case KEY_ALT_PERIOD:
			ft_strlcpy(keystr, "<M-.>", 6);
			break ;
		case KEY_ALT_SLASH:
			ft_strlcpy(keystr, "<M-/>", 6);
			break ;
		case KEY_ALT_NUM_0:
			ft_strlcpy(keystr, "<M-0>", 6);
			break ;
		case KEY_ALT_NUM_1:
			ft_strlcpy(keystr, "<M-1>", 6);
			break ;
		case KEY_ALT_NUM_2:
			ft_strlcpy(keystr, "<M-2>", 6);
			break ;
		case KEY_ALT_NUM_3:
			ft_strlcpy(keystr, "<M-3>", 6);
			break ;
		case KEY_ALT_NUM_4:
			ft_strlcpy(keystr, "<M-4>", 6);
			break ;
		case KEY_ALT_NUM_5:
			ft_strlcpy(keystr, "<M-5>", 6);
			break ;
		case KEY_ALT_NUM_6:
			ft_strlcpy(keystr, "<M-6>", 6);
			break ;
		case KEY_ALT_NUM_7:
			ft_strlcpy(keystr, "<M-7>", 6);
			break ;
		case KEY_ALT_NUM_8:
			ft_strlcpy(keystr, "<M-8>", 6);
			break ;
		case KEY_ALT_NUM_9:
			ft_strlcpy(keystr, "<M-9>", 6);
			break ;
		case KEY_ALT_COLON:
			ft_strlcpy(keystr, "<M-:>", 6);
			break ;
		case KEY_ALT_SCOLON:
			ft_strlcpy(keystr, "<M-;>", 6);
			break ;
		case KEY_ALT_LTHAN:
			ft_strlcpy(keystr, "<M-<>", 6);
			break ;
		case KEY_ALT_EQUAL:
			ft_strlcpy(keystr, "<M-=>", 6);
			break ;
		case KEY_ALT_MTHAN:
			ft_strlcpy(keystr, "<M->>", 6);
			break ;
		case KEY_ALT_QUESTION:
			ft_strlcpy(keystr, "<M-?>", 6);
			break ;
		case KEY_ALT_AT:
			ft_strlcpy(keystr, "<M-@>", 6);
			break ;
		case KEY_ALT_UP_A:
			ft_strlcpy(keystr, "<M-A>", 6);
			break ;
		case KEY_ALT_UP_B:
			ft_strlcpy(keystr, "<M-B>", 6);
			break ;
		case KEY_ALT_UP_C:
			ft_strlcpy(keystr, "<M-C>", 6);
			break ;
		case KEY_ALT_UP_D:
			ft_strlcpy(keystr, "<M-D>", 6);
			break ;
		case KEY_ALT_UP_E:
			ft_strlcpy(keystr, "<M-E>", 6);
			break ;
		case KEY_ALT_UP_F:
			ft_strlcpy(keystr, "<M-F>", 6);
			break ;
		case KEY_ALT_UP_G:
			ft_strlcpy(keystr, "<M-G>", 6);
			break ;
		case KEY_ALT_UP_H:
			ft_strlcpy(keystr, "<M-H>", 6);
			break ;
		case KEY_ALT_UP_I:
			ft_strlcpy(keystr, "<M-I>", 6);
			break ;
		case KEY_ALT_UP_J:
			ft_strlcpy(keystr, "<M-J>", 6);
			break ;
		case KEY_ALT_UP_K:
			ft_strlcpy(keystr, "<M-K>", 6);
			break ;
		case KEY_ALT_UP_L:
			ft_strlcpy(keystr, "<M-L>", 6);
			break ;
		case KEY_ALT_UP_M:
			ft_strlcpy(keystr, "<M-M>", 6);
			break ;
		case KEY_ALT_UP_N:
			ft_strlcpy(keystr, "<M-N>", 6);
			break ;
		case KEY_ALT_UP_O:
			ft_strlcpy(keystr, "<M-O>", 6);
			break ;
		case KEY_ALT_UP_P:
			ft_strlcpy(keystr, "<M-P>", 6);
			break ;
		case KEY_ALT_UP_Q:
			ft_strlcpy(keystr, "<M-Q>", 6);
			break ;
		case KEY_ALT_UP_R:
			ft_strlcpy(keystr, "<M-R>", 6);
			break ;
		case KEY_ALT_UP_S:
			ft_strlcpy(keystr, "<M-S>", 6);
			break ;
		case KEY_ALT_UP_T:
			ft_strlcpy(keystr, "<M-T>", 6);
			break ;
		case KEY_ALT_UP_U:
			ft_strlcpy(keystr, "<M-U>", 6);
			break ;
		case KEY_ALT_UP_V:
			ft_strlcpy(keystr, "<M-V>", 6);
			break ;
		case KEY_ALT_UP_W:
			ft_strlcpy(keystr, "<M-W>", 6);
			break ;
		case KEY_ALT_UP_X:
			ft_strlcpy(keystr, "<M-X>", 6);
			break ;
		case KEY_ALT_UP_Y:
			ft_strlcpy(keystr, "<M-Y>", 6);
			break ;
		case KEY_ALT_UP_Z:
			ft_strlcpy(keystr, "<M-Z>", 6);
			break ;
		case KEY_ALT_OSBRACKET:
			ft_strlcpy(keystr, "<M-[>", 6);
			break ;
		case KEY_ALT_BACKSLASH:
			ft_strlcpy(keystr, "<M-\\>", 6);
			break ;
		case KEY_ALT_CSBRACKET:
			ft_strlcpy(keystr, "<M-]>", 6);
			break ;
		case KEY_ALT_CARET:
			ft_strlcpy(keystr, "<M-^>", 6);
			break ;
		case KEY_ALT_UNDERSCORE:
			ft_strlcpy(keystr, "<M-_>", 6);
			break ;
		case KEY_ALT_BACKTICK:
			ft_strlcpy(keystr, "<M-`>", 6);
			break ;
		case KEY_ALT_DN_A:
			ft_strlcpy(keystr, "<M-a>", 6);
			break ;
		case KEY_ALT_DN_B:
			ft_strlcpy(keystr, "<M-b>", 6);
			break ;
		case KEY_ALT_DN_C:
			ft_strlcpy(keystr, "<M-c>", 6);
			break ;
		case KEY_ALT_DN_D:
			ft_strlcpy(keystr, "<M-d>", 6);
			break ;
		case KEY_ALT_DN_E:
			ft_strlcpy(keystr, "<M-e>", 6);
			break ;
		case KEY_ALT_DN_F:
			ft_strlcpy(keystr, "<M-f>", 6);
			break ;
		case KEY_ALT_DN_G:
			ft_strlcpy(keystr, "<M-g>", 6);
			break ;
		case KEY_ALT_DN_H:
			ft_strlcpy(keystr, "<M-h>", 6);
			break ;
		case KEY_ALT_DN_I:
			ft_strlcpy(keystr, "<M-i>", 6);
			break ;
		case KEY_ALT_DN_J:
			ft_strlcpy(keystr, "<M-j>", 6);
			break ;
		case KEY_ALT_DN_K:
			ft_strlcpy(keystr, "<M-k>", 6);
			break ;
		case KEY_ALT_DN_L:
			ft_strlcpy(keystr, "<M-l>", 6);
			break ;
		case KEY_ALT_DN_M:
			ft_strlcpy(keystr, "<M-m>", 6);
			break ;
		case KEY_ALT_DN_N:
			ft_strlcpy(keystr, "<M-n>", 6);
			break ;
		case KEY_ALT_DN_O:
			ft_strlcpy(keystr, "<M-o>", 6);
			break ;
		case KEY_ALT_DN_P:
			ft_strlcpy(keystr, "<M-p>", 6);
			break ;
		case KEY_ALT_DN_Q:
			ft_strlcpy(keystr, "<M-q>", 6);
			break ;
		case KEY_ALT_DN_R:
			ft_strlcpy(keystr, "<M-r>", 6);
			break ;
		case KEY_ALT_DN_S:
			ft_strlcpy(keystr, "<M-s>", 6);
			break ;
		case KEY_ALT_DN_T:
			ft_strlcpy(keystr, "<M-t>", 6);
			break ;
		case KEY_ALT_DN_U:
			ft_strlcpy(keystr, "<M-u>", 6);
			break ;
		case KEY_ALT_DN_V:
			ft_strlcpy(keystr, "<M-v>", 6);
			break ;
		case KEY_ALT_DN_W:
			ft_strlcpy(keystr, "<M-w>", 6);
			break ;
		case KEY_ALT_DN_X:
			ft_strlcpy(keystr, "<M-x>", 6);
			break ;
		case KEY_ALT_DN_Y:
			ft_strlcpy(keystr, "<M-y>", 6);
			break ;
		case KEY_ALT_DN_Z:
			ft_strlcpy(keystr, "<M-z>", 6);
			break ;
		case KEY_ALT_OCBRACKET:
			ft_strlcpy(keystr, "<M-{>", 6);
			break ;
		case KEY_ALT_PIPE:
			ft_strlcpy(keystr, "<M-|>", 6);
			break ;
		case KEY_ALT_CCBRACKET:
			ft_strlcpy(keystr, "<M-}>", 6);
			break ;
		case KEY_ALT_TILDE:
			ft_strlcpy(keystr, "<M-~>", 6);
			break ;
		case KEY_ALT_BACKSPACE:
			ft_strlcpy(keystr, "<M-BCK>", 8);
			break ;
		case KEY_CTRL_1:
			ft_strlcpy(keystr, "<C-1>", 6);
			break ;
		case KEY_CTRL_2:
			ft_strlcpy(keystr, "<C-2>", 6);
			break ;
		case KEY_CTRL_4:
			ft_strlcpy(keystr, "<C-4>", 6);
			break ;
		case KEY_CTRL_5:
			ft_strlcpy(keystr, "<C-5>", 6);
			break ;
		case KEY_CTRL_6:
			ft_strlcpy(keystr, "<C-6>", 6);
			break ;
		case KEY_CTRL_7:
			ft_strlcpy(keystr, "<C-7>", 6);
			break ;
		case KEY_CTRL_9:
			ft_strlcpy(keystr, "<C-9>", 6);
			break ;
		case KEY_F_1:
			ft_strlcpy(keystr, "<F-1>", 6);
			break ;
		case KEY_F_2:
			ft_strlcpy(keystr, "<F-2>", 6);
			break ;
		case KEY_F_3:
			ft_strlcpy(keystr, "<F-3>", 6);
			break ;
		case KEY_F_4:
			ft_strlcpy(keystr, "<F-4>", 6);
			break ;
		case KEY_F_5:
			ft_strlcpy(keystr, "<F-5>", 6);
			break ;
		case KEY_F_6:
			ft_strlcpy(keystr, "<F-6>", 6);
			break ;
		case KEY_F_7:
			ft_strlcpy(keystr, "<F-7>", 6);
			break ;
		case KEY_F_8:
			ft_strlcpy(keystr, "<F-8>", 6);
			break ;
		case KEY_F_9:
			ft_strlcpy(keystr, "<F-9>", 6);
			break ;
		case KEY_F_10:
			ft_strlcpy(keystr, "<F-10>", 7);
			break ;
		case KEY_F_11:
			ft_strlcpy(keystr, "<F-11>", 7);
			break ;
		case KEY_F_12:
			ft_strlcpy(keystr, "<F-12>", 7);
			break ;
		case KEY_UP_F_1:
			ft_strlcpy(keystr, "<U-F-1>", 8);
			break ;
		case KEY_UP_F_2:
			ft_strlcpy(keystr, "<U-F-2>", 8);
			break ;
		case KEY_UP_F_3:
			ft_strlcpy(keystr, "<U-F-3>", 8);
			break ;
		case KEY_UP_F_4:
			ft_strlcpy(keystr, "<U-F-4>", 8);
			break ;
		case KEY_UP_F_5:
			ft_strlcpy(keystr, "<U-F-5>", 8);
			break ;
		case KEY_UP_F_6:
			ft_strlcpy(keystr, "<U-F-6>", 8);
			break ;
		case KEY_UP_F_7:
			ft_strlcpy(keystr, "<U-F-7>", 8);
			break ;
		case KEY_UP_F_8:
			ft_strlcpy(keystr, "<U-F-8>", 8);
			break ;
		case KEY_UP_F_9:
			ft_strlcpy(keystr, "<U-F-9>", 8);
			break ;
		case KEY_UP_F_10:
			ft_strlcpy(keystr, "<U-F-10>", 9);
			break ;
		case KEY_UP_F_11:
			ft_strlcpy(keystr, "<U-F-11>", 9);
			break ;
		case KEY_UP_F_12:
			ft_strlcpy(keystr, "<U-F-12>", 9);
			break ;
		case KEY_ALT_F_1:
			ft_strlcpy(keystr, "<M-F-1>", 8);
			break ;
		case KEY_ALT_F_2:
			ft_strlcpy(keystr, "<M-F-2>", 8);
			break ;
		case KEY_ALT_F_3:
			ft_strlcpy(keystr, "<M-F-3>", 8);
			break ;
		case KEY_ALT_F_4:
			ft_strlcpy(keystr, "<M-F-4>", 8);
			break ;
		case KEY_ALT_F_5:
			ft_strlcpy(keystr, "<M-F-5>", 8);
			break ;
		case KEY_ALT_F_6:
			ft_strlcpy(keystr, "<M-F-6>", 8);
			break ;
		case KEY_ALT_F_7:
			ft_strlcpy(keystr, "<M-F-7>", 8);
			break ;
		case KEY_ALT_F_8:
			ft_strlcpy(keystr, "<M-F-8>", 8);
			break ;
		case KEY_ALT_F_9:
			ft_strlcpy(keystr, "<M-F-9>", 8);
			break ;
		case KEY_ALT_F_10:
			ft_strlcpy(keystr, "<M-F-10>", 9);
			break ;
		case KEY_ALT_F_11:
			ft_strlcpy(keystr, "<M-F-11>", 9);
			break ;
		case KEY_ALT_F_12:
			ft_strlcpy(keystr, "<M-F-12>", 9);
			break ;
		case KEY_ALT_UP_F_1:
			ft_strlcpy(keystr, "<M-U-F-1>", 10);
			break ;
		case KEY_ALT_UP_F_2:
			ft_strlcpy(keystr, "<M-U-F-2>", 10);
			break ;
		case KEY_ALT_UP_F_3:
			ft_strlcpy(keystr, "<M-U-F-3>", 10);
			break ;
		case KEY_ALT_UP_F_4:
			ft_strlcpy(keystr, "<M-U-F-4>", 10);
			break ;
		case KEY_ALT_UP_F_5:
			ft_strlcpy(keystr, "<M-U-F-5>", 10);
			break ;
		case KEY_ALT_UP_F_6:
			ft_strlcpy(keystr, "<M-U-F-6>", 10);
			break ;
		case KEY_ALT_UP_F_7:
			ft_strlcpy(keystr, "<M-U-F-7>", 10);
			break ;
		case KEY_ALT_UP_F_8:
			ft_strlcpy(keystr, "<M-U-F-8>", 10);
			break ;
		case KEY_ALT_UP_F_9:
			ft_strlcpy(keystr, "<M-U-F-9>", 10);
			break ;
		case KEY_ALT_UP_F_10:
			ft_strlcpy(keystr, "<M-U-F-10>", 11);
			break ;
		case KEY_ALT_UP_F_11:
			ft_strlcpy(keystr, "<M-U-F-11>", 11);
			break ;
		case KEY_ALT_UP_F_12:
			ft_strlcpy(keystr, "<M-U-F-12>", 11);
			break ;
		case KEY_CTRL_F_1:
			ft_strlcpy(keystr, "<C-F-1>", 8);
			break ;
		case KEY_CTRL_F_2:
			ft_strlcpy(keystr, "<C-F-2>", 8);
			break ;
		case KEY_CTRL_F_3:
			ft_strlcpy(keystr, "<C-F-3>", 8);
			break ;
		case KEY_CTRL_F_4:
			ft_strlcpy(keystr, "<C-F-4>", 8);
			break ;
		case KEY_CTRL_F_5:
			ft_strlcpy(keystr, "<C-F-5>", 8);
			break ;
		case KEY_CTRL_F_6:
			ft_strlcpy(keystr, "<C-F-6>", 8);
			break ;
		case KEY_CTRL_F_7:
			ft_strlcpy(keystr, "<C-F-7>", 8);
			break ;
		case KEY_CTRL_F_8:
			ft_strlcpy(keystr, "<C-F-8>", 8);
			break ;
		case KEY_CTRL_F_9:
			ft_strlcpy(keystr, "<C-F-9>", 8);
			break ;
		case KEY_CTRL_F_10:
			ft_strlcpy(keystr, "<C-F-10>", 9);
			break ;
		case KEY_CTRL_F_11:
			ft_strlcpy(keystr, "<C-F-11>", 9);
			break ;
		case KEY_CTRL_F_12:
			ft_strlcpy(keystr, "<C-F-12>", 9);
			break ;
		case KEY_CTRL_UP_F_1:
			ft_strlcpy(keystr, "<C-U-F-1>", 10);
			break ;
		case KEY_CTRL_UP_F_2:
			ft_strlcpy(keystr, "<C-U-F-2>", 10);
			break ;
		case KEY_CTRL_UP_F_3:
			ft_strlcpy(keystr, "<C-U-F-3>", 10);
			break ;
		case KEY_CTRL_UP_F_4:
			ft_strlcpy(keystr, "<C-U-F-4>", 10);
			break ;
		case KEY_CTRL_UP_F_5:
			ft_strlcpy(keystr, "<C-U-F-5>", 10);
			break ;
		case KEY_CTRL_UP_F_6:
			ft_strlcpy(keystr, "<C-U-F-6>", 10);
			break ;
		case KEY_CTRL_UP_F_7:
			ft_strlcpy(keystr, "<C-U-F-7>", 10);
			break ;
		case KEY_CTRL_UP_F_8:
			ft_strlcpy(keystr, "<C-U-F-8>", 10);
			break ;
		case KEY_CTRL_UP_F_9:
			ft_strlcpy(keystr, "<C-U-F-9>", 10);
			break ;
		case KEY_CTRL_UP_F_10:
			ft_strlcpy(keystr, "<C-U-F-10>", 11);
			break ;
		case KEY_CTRL_UP_F_11:
			ft_strlcpy(keystr, "<C-U-F-11>", 11);
			break ;
		case KEY_CTRL_UP_F_12:
			ft_strlcpy(keystr, "<C-U-F-12>", 11);
			break ;
		case KEY_ALT_CTRL_UP_F_1:
			ft_strlcpy(keystr, "<M-C-U-F-1>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_2:
			ft_strlcpy(keystr, "<M-C-U-F-2>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_3:
			ft_strlcpy(keystr, "<M-C-U-F-3>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_4:
			ft_strlcpy(keystr, "<M-C-U-F-4>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_5:
			ft_strlcpy(keystr, "<M-C-U-F-5>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_6:
			ft_strlcpy(keystr, "<M-C-U-F-6>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_7:
			ft_strlcpy(keystr, "<M-C-U-F-7>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_8:
			ft_strlcpy(keystr, "<M-C-U-F-8>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_9:
			ft_strlcpy(keystr, "<M-C-U-F-9>", 12);
			break ;
		case KEY_ALT_CTRL_UP_F_10:
			ft_strlcpy(keystr, "<M-C-U-F-10>", 13);
			break ;
		case KEY_ALT_CTRL_UP_F_11:
			ft_strlcpy(keystr, "<M-C-U-F-11>", 13);
			break ;
		case KEY_ALT_CTRL_UP_F_12:
			ft_strlcpy(keystr, "<M-C-U-F-12>", 13);
			break ;
		case KEY_UP:
			ft_strlcpy(keystr, "<up>", 5);
			break ;
		case KEY_DOWN:
			ft_strlcpy(keystr, "<down>", 7);
			break ;
		case KEY_RIGHT:
			ft_strlcpy(keystr, "<right>", 8);
			break ;
		case KEY_LEFT:
			ft_strlcpy(keystr, "<left>", 7);
			break ;
		case KEY_UP_UP:
			ft_strlcpy(keystr, "<U-up>", 7);
			break ;
		case KEY_UP_DOWN:
			ft_strlcpy(keystr, "<U-down>", 9);
			break ;
		case KEY_UP_RIGHT:
			ft_strlcpy(keystr, "<U-right>", 10);
			break ;
		case KEY_UP_LEFT:
			ft_strlcpy(keystr, "<U-left>", 9);
			break ;
		case KEY_ALT_UP:
			ft_strlcpy(keystr, "<M-up>", 7);
			break ;
		case KEY_ALT_DOWN:
			ft_strlcpy(keystr, "<M-down>", 9);
			break ;
		case KEY_ALT_RIGHT:
			ft_strlcpy(keystr, "<M-right>", 10);
			break ;
		case KEY_ALT_LEFT:
			ft_strlcpy(keystr, "<M-left>", 9);
			break ;
		case KEY_ALT_UP_UP:
			ft_strlcpy(keystr, "<M-U-up>", 9);
			break ;
		case KEY_ALT_UP_DOWN:
			ft_strlcpy(keystr, "<M-U-down>", 11);
			break ;
		case KEY_ALT_UP_RIGHT:
			ft_strlcpy(keystr, "<M-U-right>", 12);
			break ;
		case KEY_ALT_UP_LEFT:
			ft_strlcpy(keystr, "<M-U-left>", 11);
			break ;
		case KEY_CTRL_UP:
			ft_strlcpy(keystr, "<C-up>", 7);
			break ;
		case KEY_CTRL_DOWN:
			ft_strlcpy(keystr, "<C-down>", 9);
			break ;
		case KEY_CTRL_RIGHT:
			ft_strlcpy(keystr, "<C-right>", 10);
			break ;
		case KEY_CTRL_LEFT:
			ft_strlcpy(keystr, "<C-left>", 9);
			break ;
		case KEY_CTRL_UP_UP:
			ft_strlcpy(keystr, "<C-U-up>", 9);
			break ;
		case KEY_CTRL_UP_DOWN:
			ft_strlcpy(keystr, "<C-U-down>", 11);
			break ;
		case KEY_CTRL_UP_RIGHT:
			ft_strlcpy(keystr, "<C-U-right>", 12);
			break ;
		case KEY_CTRL_UP_LEFT:
			ft_strlcpy(keystr, "<C-U-left>", 11);
			break ;
		case KEY_ALT_CTRL_UP:
			ft_strlcpy(keystr, "<M-C-up>", 9);
			break ;
		case KEY_ALT_CTRL_DOWN:
			ft_strlcpy(keystr, "<M-C-down>", 11);
			break ;
		case KEY_ALT_CTRL_RIGHT:
			ft_strlcpy(keystr, "<M-C-right>", 12);
			break ;
		case KEY_ALT_CTRL_LEFT:
			ft_strlcpy(keystr, "<M-C-left>", 11);
			break ;
		case KEY_ALT_CTRL_UP_UP:
			ft_strlcpy(keystr, "<M-C-U-up>", 11);
			break ;
		case KEY_ALT_CTRL_UP_DOWN:
			ft_strlcpy(keystr, "<M-C-U-down>", 13);
			break ;
		case KEY_ALT_CTRL_UP_RIGHT:
			ft_strlcpy(keystr, "<M-C-U-right>", 14);
			break ;
		case KEY_ALT_CTRL_UP_LEFT:
			ft_strlcpy(keystr, "<M-C-U-left>", 13);
			break ;
		case KEY_ESC:
			ft_strlcpy(keystr, "<ESC>", 6);
			break ;
		case KEY_ALT_TAB:
			ft_strlcpy(keystr, "<M-TAB>", 9);
			break ;
		case KEY_ALT_RETURN:
			ft_strlcpy(keystr, "<M-RET>", 9);
			break ;
		case KEY_INS:
			ft_strlcpy(keystr, "<INS>", 6);
			break ;
		case KEY_HOME:
			ft_strlcpy(keystr, "<HME>", 6);
			break ;
		case KEY_PGUP:
			ft_strlcpy(keystr, "<PGU>", 6);
			break ;
		case KEY_DEL:
			ft_strlcpy(keystr, "<DEL>", 6);
			break ;
		case KEY_END:
			ft_strlcpy(keystr, "<END>", 6);
			break ;
		case KEY_PGDN:
			ft_strlcpy(keystr, "<PGD>", 6);
			break ;
		case KEY_UP_DEL:
			ft_strlcpy(keystr, "<U-DEL>", 6);
			break ;
		case KEY_ALT_INS:
			ft_strlcpy(keystr, "<M-INS>", 8);
			break ;
		case KEY_ALT_HOME:
			ft_strlcpy(keystr, "<M-HME>", 8);
			break ;
		case KEY_ALT_PGUP:
			ft_strlcpy(keystr, "<M-PGU>", 8);
			break ;
		case KEY_ALT_DEL:
			ft_strlcpy(keystr, "<M-DEL>", 8);
			break ;
		case KEY_ALT_END:
			ft_strlcpy(keystr, "<M-END>", 8);
			break ;
		case KEY_ALT_PGDN:
			ft_strlcpy(keystr, "<M-PGD>", 8);
			break ;
		case KEY_ALT_UP_INS:
			ft_strlcpy(keystr, "<M-U-INS>", 10);
			break ;
		case KEY_ALT_UP_HOME:
			ft_strlcpy(keystr, "<M-U-HME>", 10);
			break ;
		case KEY_ALT_UP_PGUP:
			ft_strlcpy(keystr, "<M-U-PGU>", 10);
			break ;
		case KEY_ALT_UP_DEL:
			ft_strlcpy(keystr, "<M-U-DEL>", 10);
			break ;
		case KEY_ALT_UP_END:
			ft_strlcpy(keystr, "<M-U-END>", 10);
			break ;
		case KEY_ALT_UP_PGDN:
			ft_strlcpy(keystr, "<M-U-PGD>", 10);
			break ;
		case KEY_CTRL_INS:
			ft_strlcpy(keystr, "<C-INS>", 8);
			break ;
		case KEY_CTRL_HOME:
			ft_strlcpy(keystr, "<C-HME>", 8);
			break ;
		case KEY_CTRL_PGUP:
			ft_strlcpy(keystr, "<C-PGU>", 8);
			break ;
		case KEY_CTRL_DEL:
			ft_strlcpy(keystr, "<C-DEL>", 8);
			break ;
		case KEY_CTRL_END:
			ft_strlcpy(keystr, "<C-END>", 8);
			break ;
		case KEY_CTRL_PGDN:
			ft_strlcpy(keystr, "<C-PGD>", 8);
			break ;
		case KEY_CTRL_UP_INS:
			ft_strlcpy(keystr, "<C-U-INS>", 10);
			break ;
		case KEY_CTRL_UP_HOME:
			ft_strlcpy(keystr, "<C-U-HME>", 10);
			break ;
		case KEY_CTRL_UP_PGUP:
			ft_strlcpy(keystr, "<C-U-PGU>", 10);
			break ;
		case KEY_CTRL_UP_DEL:
			ft_strlcpy(keystr, "<C-U-DEL>", 10);
			break ;
		case KEY_CTRL_UP_END:
			ft_strlcpy(keystr, "<C-U-END>", 10);
			break ;
		case KEY_CTRL_UP_PGDN:
			ft_strlcpy(keystr, "<C-U-PGD>", 10);
			break ;
		case KEY_ALT_CTRL_INS:
			ft_strlcpy(keystr, "<M-C-INS>", 10);
			break ;
		case KEY_ALT_CTRL_HOME:
			ft_strlcpy(keystr, "<M-C-HME>", 10);
			break ;
		case KEY_ALT_CTRL_PGUP:
			ft_strlcpy(keystr, "<M-C-PGU>", 10);
			break ;
		case KEY_ALT_CTRL_DEL:
			ft_strlcpy(keystr, "<M-C-DEL>", 10);
			break ;
		case KEY_ALT_CTRL_END:
			ft_strlcpy(keystr, "<M-C-END>", 10);
			break ;
		case KEY_ALT_CTRL_PGDN:
			ft_strlcpy(keystr, "<M-C-PGD>", 10);
			break ;
		case KEY_ALT_CTRL_UP_INS:
			ft_strlcpy(keystr, "<M-C-U-INS>", 12);
			break ;
		case KEY_ALT_CTRL_UP_HOME:
			ft_strlcpy(keystr, "<M-C-U-HME>", 12);
			break ;
		case KEY_ALT_CTRL_UP_PGUP:
			ft_strlcpy(keystr, "<M-C-U-PGU>", 12);
			break ;
		case KEY_ALT_CTRL_UP_DEL:
			ft_strlcpy(keystr, "<M-C-U-DEL>", 12);
			break ;
		case KEY_ALT_CTRL_UP_END:
			ft_strlcpy(keystr, "<M-C-U-END>", 12);
			break ;
		case KEY_ALT_CTRL_UP_PGDN:
			ft_strlcpy(keystr, "<M-C-U-PGD>", 12);
			break ;
	}
	return (keystr);
}

void	ft_rl_unmap(const char *key)
{
	ft_maprm(g_maps, key);
}

void	ft_rl_unmap_fn(const char *func)
{
	t_hmap_pair	**items;
	rl_fn_t		*f;
	size_t		i;

	f = ft_mapget(g_funcs, func);
	if (!f)
		return ;
	i = 0;
	items = g_maps->items;
	while (i < g_maps->size)
	{
		if (items[i] && items[i] != (void *)&g_hmap_sentinel
			&& ((rl_map_t *)(items[i]->value))->f == *f)
			ft_maprm(g_maps, items[i]->key);
		i++;
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
	uint64_t	val;

	val = KEY_CTRL_A;
	while (val < KEY_CTRL_Z && val != key)
		val++;
	if (val == KEY_TAB)
	{
		ft_strlcpy(keystr, "<TAB>", 6);
		return ;
	}
	if (val == KEY_ENTER)
	{
		ft_strlcpy(keystr, "<ENT>", 6);
		return ;
	}
	if (val == KEY_RETURN)
	{
		ft_strlcpy(keystr, "<RET>", 6);
		return ;
	}
	ft_strlcpy(keystr, "<C-a>", 6);
	keystr[3] = val + 0x60U;
}
