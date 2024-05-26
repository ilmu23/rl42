/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_keymap_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 05:18:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	_ascii(char *keystr, const uint64_t key);
static inline void	_ctrl(char *keystr, const uint64_t key);
static inline void	_alt(char *keystr, const uint64_t key);

uint8_t	ft_rl_ismapped(uint64_t key)
{
	rl_map_t	*mapping;

	mapping = ft_mapget(g_maps, ft_rl_keystr(key));
	return (mapping != NULL);
}

rl_fn_t	ft_rl_getmap(uint64_t key)
{
	rl_map_t	*mapping;

	mapping = ft_mapget(g_maps, ft_rl_keystr(key));
	if (mapping)
		return (*mapping->f);
	return (NULL);
}

char	*ft_rl_keystr(const uint64_t key)
{
	static char	keystr[8];

	ft_memset(keystr, 0, sizeof(keystr));
	if (key >= KEY_BANG && key <= KEY_TILDE)
		_ascii(keystr, key);
	else if (key >= KEY_CTRL_A && key <= KEY_CTRL_Z)
		_ctrl(keystr, key);
	else if (key >= KEY_ALT_BANG && key <= KEY_ALT_TILDE)
		_alt(keystr, key);
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
		case KEY_UP:
			ft_strlcpy(keystr, "<up>", 5);
			break ;
		case KEY_DOWN:
			ft_strlcpy(keystr, "<down>", 7);
			break ;
		case KEY_LEFT:
			ft_strlcpy(keystr, "<left>", 7);
			break ;
		case KEY_RIGHT:
			ft_strlcpy(keystr, "<right>", 8);
			break ;
		case KEY_ESC:
			ft_strlcpy(keystr, "<ESC>", 6);
			break ;
		case KEY_TAB:
			ft_strlcpy(keystr, "<TAB>", 6);
			break ;
		case KEY_RETURN:
			ft_strlcpy(keystr, "<RET>", 6);
			break ;
		case KEY_ALT_TAB:
			ft_strlcpy(keystr, "<M-TAB>", 8);
			break ;
		case KEY_ALT_RETURN:
			ft_strlcpy(keystr, "<M-RET>", 8);
			break ;
		case KEY_PRINT:
			ft_strlcpy(keystr, "<PRT>", 6);
			break ;
		case KEY_SCRLL:
			ft_strlcpy(keystr, "<SCR>", 6);
			break ;
		case KEY_PAUSE:
			ft_strlcpy(keystr, "<PSE>", 6);
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
	ft_strlcpy(keystr, "<C-a>", 6);
	keystr[3] = val + 0x60U;
}

static inline void	_alt(char *keystr, const uint64_t key)
{
	uint64_t	val;

	val = KEY_ALT_BANG;
	while (val < KEY_ALT_TILDE && val != key)
		val += 0x1U << 8;
	ft_strlcpy(keystr, "<M-!>", 6);
	keystr[3] = val >> 8;
}
