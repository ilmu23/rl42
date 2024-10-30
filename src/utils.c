/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:40:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 15:33:49 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline uint8_t	_interrupt(void);

rl_cmp_fn_t	ft_rl_get_completion_fn(void)
{
	ft_rl_init();
	return (g_cmp_fn);
}

rl_block_t	*ft_rl_newblock(const char *str, const int16_t pos[2])
{
	rl_block_t	*out;

	out = __push(__alloc(sizeof(*out)));
	if (!out)
		exit(ft_rl_perror());
	memcpy(out, &(rl_block_t){.str = str, .length = strlen(str),
			.pos[0] = pos[0], .pos[1] = pos[1], .highlighted = 0}, sizeof(*out));
	return (out);
}

uint64_t	ft_rl_getkey(void)
{
	uint64_t	key;
	uint64_t	keymask;
	uint8_t		shift;

	if (g_keybuf.size == 0)
	{
		if (!(g_status & _HIDE_CURSOR))
			ft_ti_tputs(g_escapes.cnorm, 1, ft_rl_putc);
		g_keybuf.size = read(0, &g_keybuf.key, sizeof(g_keybuf.key));
		if (g_keybuf.size == -1)
		{
			if (_interrupt())
				return (ft_rl_getkey());
			exit(ft_rl_perror());
		}
		if (!(g_status & _HIDE_CURSOR))
			ft_ti_tputs(g_escapes.civis, 1, ft_rl_putc);
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
	if (g_input.sprompt)
	{
		__popblk(g_input.sprompt);
		g_input.sprompt = NULL;
		ft_rl_redisplay(&g_input, PROMPT);
	}
	return (key);
}

ssize_t	ft_rl_putc(const int8_t c)
{
	return (write(1, &c, sizeof(c)));
}

int32_t	ft_rl_getarg(void)
{
	g_argument.set = 0;
	return (g_argument.arg);
}

uint8_t	ft_rl_isdir(const char *path)
{
	struct stat	file;

	lstat(path, &file);
	if (S_ISLNK(file.st_mode) && ft_rl_get(_CMP_MLDIRS_HASH))
		stat(path, &file);
	return (S_ISDIR(file.st_mode));
}

uint8_t	ft_rl_geteditmode(void)
{
	return (g_status & _MD_MASK);
}

void	ft_rl_set_completion_fn(rl_cmp_fn_t f)
{
	ft_rl_init();
	g_cmp_fn = f;
}

void	ft_rl_seteditmode(const uint8_t mode)
{
	uint8_t	emode;
	
	emode = ft_rl_geteditmode();
	if (mode != emode)
		g_status ^= emode | mode;
}

void	ft_rl_clearblocks(void)
{
	while (g_blocks)
		__lstrmnode(&g_blocks, g_blocks);
}

void	ft_rl_bell(void)
{
	uint64_t	type;

	type = ft_rl_get(_BSTYLE_HASH);
	switch (type)
	{
		case BELL_AUDIBLE:
			ft_ti_tputs(g_escapes.bel, 1, ft_rl_putc);
			break ;
		case BELL_VISIBLE:
			ft_ti_tputs(g_escapes.flash, 1, ft_rl_putc);
	}
}

static inline uint8_t	_interrupt(void)
{
	if (errno != EINTR)
		return (0);
	g_keybuf.size = 0;
	errno = 0;
	return (1);
}
