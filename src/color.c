/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:57:08 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 11:43:18 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define _UHALF 0xF0U
#define _LHALF 0x0FU

#define _bold	((g_hlcolor.mode & FT_RL_HL_BOLD) ? g_escapes.bold : "")
#define _uline	((g_hlcolor.mode & FT_RL_HL_ULINE) ? g_escapes.smul : "")

#define set_seq(x)		(__popblk(g_hlcolor.seq), g_hlcolor.seq = __push(__strnjoin(3, _bold, _uline, x)))
#define set_mode(x, y)	(g_hlcolor.mode = (g_hlcolor.mode & y) | (x))

static inline void	_refresh_seq(void);

const char	*ft_rl_hlcolor(void)
{
	return ((g_hlcolor.seq) ? g_hlcolor.seq : "");
}

void	ft_rl_sethlcolor_mode(const uint8_t mode)
{
	ft_rl_init();
	if (mode & FT_RL_HL_FG && mode & FT_RL_HL_BG)
		set_mode(mode & ~FT_RL_HL_BG, ~_LHALF);
	else
		set_mode(mode, ~_LHALF);
	_refresh_seq();
}

void	ft_rl_sethlcolor_sgr(const char *s)
{
	ft_rl_init();
	set_mode(_HL_SGR, ~_UHALF);
	set_seq(s);
	g_hlcolor.sgr = s;
}

void	ft_rl_sethlcolor_clr(const uint8_t color)
{
	const char	*seq;

	ft_rl_init();
	set_mode(_HL_CLR, ~_UHALF);
	seq = (g_hlcolor.mode & FT_RL_HL_BG) ? g_escapes.setab : g_escapes.setaf;
	set_seq(ft_ti_tparm(seq, color));
	g_hlcolor.clr = color;
}

void	ft_rl_sethlcolor_rgb(const uint8_t r, const uint8_t g, const uint8_t b)
{
	char		fmt[23] = "\e[38;2;%hhu;%hhu;%hhum";
	char		buf[23];

	ft_rl_init();
	set_mode(_HL_RGB, ~_UHALF);
	if (g_hlcolor.mode & FT_RL_HL_BG)
		fmt[2] = '4';
	__snprintf(buf, 23, fmt, r, g, b);
	set_seq(__push(__strdup(buf)));
	memcpy(&g_hlcolor.rgb, &(rl_rgb_t){.r = r, .g = g, .b = b}, sizeof(g_hlcolor.rgb));
}

static inline void	_refresh_seq(void)
{
	switch (g_hlcolor.mode & _UHALF)
	{
		case _HL_SGR:
			ft_rl_sethlcolor_sgr(g_hlcolor.sgr);
			break ;
		case _HL_CLR:
			ft_rl_sethlcolor_clr(g_hlcolor.clr);
			break ;
		case _HL_RGB:
			ft_rl_sethlcolor_rgb(g_hlcolor.rgb.r, g_hlcolor.rgb.g, g_hlcolor.rgb.b);
	}
}
