/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:57:08 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/30 09:54:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

static inline char	*_getsgr(void);

char	*ft_rl_hlcolor(void)
{
	static const char	f[2][17] = {"\e[38;2;%u;%u;%um", "\e[48;2;%u;%u;%um"};
	static char			seq[23];

	if (g_hlcolor.sgr)
		return (_getsgr());
	ft_snprintf(seq, 20, f[g_hlcolor.mode],
		g_hlcolor.rgbval.r, g_hlcolor.rgbval.g, g_hlcolor.rgbval.b);
	return (seq);
}

void	ft_rl_sethlcolor_mode(const uint8_t mode)
{
	if (mode == FT_RL_HL_FG || mode == FT_RL_HL_BG)
		g_hlcolor.mode = mode;
	else
		ft_dprintf(2, "rl42: sethlcolor_mode: unknown mode\n");
}

void	ft_rl_sethlcolor_sgr(const char *s)
{
	g_hlcolor.sgr = s;
}

void	ft_rl_sethlcolor_rgb(const uint8_t r, const uint8_t g, const uint8_t b)
{
	ft_memcpy(&g_hlcolor.rgbval, &(rl_rgb_t){.r = r, .g = g, .b = b},
		sizeof(g_hlcolor.rgbval));
}

static inline char	*_getsgr(void)
{
	static char	out[64];

	if (g_hlcolor.mode == FT_RL_HL_FG)
	{
		if (ft_strnstr(g_hlcolor.sgr, "\e[3", 3))
			return ((char *)g_hlcolor.sgr);
		ft_strlcpy(out, g_hlcolor.sgr, 64);
		out[2] = '3';
		return (out);
	}
	if (ft_strnstr(g_hlcolor.sgr, "\e[4", 3))
		return ((char *)g_hlcolor.sgr);
	ft_strlcpy(out, g_hlcolor.sgr, 64);
	out[2] = '4';
	return (out);
}
