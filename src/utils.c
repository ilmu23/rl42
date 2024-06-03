/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:40:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/03 15:26:45 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint64_t	ft_rl_getkey(void)
{
	uint64_t	key;
	uint64_t	keymask;
	uint8_t		shift;

	if (g_keybuf.size == 0)
	{
		g_keybuf.size = read(0, &g_keybuf.key, sizeof(g_keybuf.key));
		if (g_keybuf.size == -1)
			exit(ft_rl_perror());
	}
	keymask = _KEYSHIFT_MASK;
	shift = g_keybuf.size;
	while (--shift > 0)
		keymask |= keymask << 8;
	while (!ft_rl_iskey(g_keybuf.key & keymask) && shift++ < g_keybuf.size)
		keymask >>= 8;
	if (shift == g_keybuf.size)
	{
		g_keybuf.key = 0;
		g_keybuf.size = 0;
		return (0);
	}
	key = g_keybuf.key & keymask;
	g_keybuf.key >>= ((g_keybuf.size - shift) * 8);
	g_keybuf.size -= g_keybuf.size - shift;
	return (key);
}

uint8_t	ft_rl_isdir(const char *path)
{
	struct stat	file;

	stat(path, &file);
	return (file.st_mode & S_IFDIR);
}
