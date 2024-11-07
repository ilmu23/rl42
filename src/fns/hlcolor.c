/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hlcolor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:31:04 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/02 07:03:02 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define set(x)			((x != 0) ? g_status = ((g_status & x) ? g_status : g_status ^ x) : (void)x)
#define unset(x)		((x != 0) ? g_status = ((g_status & x) ? g_status ^ x : g_status) : (void)x)
#define getfn(x, y)		(set(x), f = ft_rl_getinput(y), unset(x))
#define iscolor(x)		(x == ft_rl_hl_sgr || x == ft_rl_hl_clr || x == ft_rl_hl_rgb)
#define istoggle(x)		(x == ft_rl_hl_bld || x == ft_rl_hl_uln || x == ft_rl_hl_fbg)
#define isvalidfn(x)	(iscolor(x) || istoggle(x))

static inline void	_preview(void);

uint8_t	ft_rl_hlc(rl_input_t *input)
{
	uint8_t		emode;
	rl_fn_t		f;

	emode = ft_rl_geteditmode();
	ft_rl_seteditmode(_MD_HLCOLOR);
	_preview();
	for (getfn(_HIDE_CURSOR, NULL); f != ft_rl_hl_acc; getfn(_HIDE_CURSOR, NULL))
		if (isvalidfn (f) && !f(input))
			break ;
	ft_rl_seteditmode(emode);
	ft_rl_redisplay(input, PROMPT);
	return (1);
}

uint8_t	ft_rl_hl_bld(__UNUSED rl_input_t *input)
{
	ft_rl_sethlcolor_mode((g_hlcolor.mode & 0x0F) ^ FT_RL_HL_BOLD);
	_preview();
	return (1);
}

uint8_t	ft_rl_hl_uln(__UNUSED rl_input_t *input)
{
	ft_rl_sethlcolor_mode((g_hlcolor.mode & 0x0F) ^ FT_RL_HL_ULINE);
	_preview();
	return (1);
}

uint8_t	ft_rl_hl_fbg(__UNUSED rl_input_t *input)
{
	ft_rl_sethlcolor_mode((g_hlcolor.mode & 0x0F) ^ (FT_RL_HL_FG | FT_RL_HL_BG));
	_preview();
	return (1);
}

uint8_t	ft_rl_hl_sgr(__UNUSED rl_input_t *input)
{
	ft_rl_sethlcolor_sgr(g_hlcolor.sgr);
	_preview();
	return (1);
}

uint8_t	ft_rl_hl_clr(__UNUSED rl_input_t *input)
{
	char		color[4] = "\0\0\0\0";
	const char	*keyseq;
	rl_fn_t		f;
	size_t		i;

	__printf(" Enter color: %s", color);
	for (i = 0, getfn(0, &keyseq); f != ft_rl_hl_acc; getfn(0, &keyseq))
	{
		if (f == ft_rl_ins)
		{
			if (i > 2)
				break ;
			color[i++] = (uint8_t)*keyseq;
			if (atoi(color) > UINT8_MAX)
				__strlcpy(color, "255", 4);
		}
		else if (f == ft_rl_bdl)
			color[(i > 0) ? --i : i] = '\0';
		else if (istoggle(f) || f == ft_rl_pst)
			f(input);
		ft_rl_sethlcolor_clr(atoi(color));
		_preview();
		__printf(" Enter color: %s", color);
	}
	_preview();
	return (1);
}

uint8_t	ft_rl_hl_rgb(__UNUSED rl_input_t *input)
{
	char		red[4] = "\0\0\0\0";
	char		green[4] = "\0\0\0\0";
	char		blue[4] = "\0\0\0\0";
	const char	*keyseq;
	rl_rgb_t	color = {.r = 0, .g = 0, .b = 0};
	rl_fn_t		f;
	size_t		i;

	__printf(" Enter red value: %s", red);
	for (i = 0, getfn(0, &keyseq); f != ft_rl_hl_acc; getfn(0, &keyseq))
	{
		if (f == ft_rl_ins)
		{
			if (i > 2)
				break ;
			red[i++] = (uint8_t)*keyseq;
			if (atoi(red) > UINT8_MAX)
				__strlcpy(red, "255", 4);
			color.r = atoi(red);
		}
		else if (f == ft_rl_bdl)
			red[(i > 0) ? --i : i] = '\0';
		else if (istoggle(f) || f == ft_rl_pst)
			f(input);
		ft_rl_sethlcolor_rgb(color.r, color.g, color.b);
		_preview();
		__printf(" Enter red value: %s", red);
	}
	_preview();
	__printf(" Enter green value: %s", green);
	for (i = 0, getfn(0, &keyseq); f != ft_rl_hl_acc; getfn(0, &keyseq))
	{
		if (f == ft_rl_ins)
		{
			if (i > 2)
				break ;
			green[i++] = (uint8_t)*keyseq;
			if (atoi(green) > UINT8_MAX)
				__strlcpy(green, "255", 4);
			color.g = atoi(green);
		}
		else if (f == ft_rl_bdl)
			green[(i > 0) ? --i : i] = '\0';
		else if (istoggle(f) || f == ft_rl_pst)
			f(input);
		ft_rl_sethlcolor_rgb(color.r, color.g, color.b);
		_preview();
		__printf(" Enter green value: %s", green);
	}
	_preview();
	__printf(" Enter blue value: %s", blue);
	for (i = 0, getfn(0, &keyseq); f != ft_rl_hl_acc; getfn(0, &keyseq))
	{
		if (f == ft_rl_ins)
		{
			if (i > 2)
				break ;
			blue[i++] = (uint8_t)*keyseq;
			if (atoi(blue) > UINT8_MAX)
				__strlcpy(blue, "255", 4);
			color.b = atoi(blue);
		}
		else if (f == ft_rl_bdl)
			blue[(i > 0) ? --i : i] = '\0';
		else if (istoggle(f) || f == ft_rl_pst)
			f(input);
		ft_rl_sethlcolor_rgb(color.r, color.g, color.b);
		_preview();
		__printf(" Enter blue value: %s", blue);
	}
	_preview();
	return (1);
}

uint8_t	ft_rl_hl_acc(__UNUSED rl_input_t *input)
{
	return (1);
}

static inline void	_preview(void)
{
	ft_rl_cursor_move(g_input.cursor->p_row, g_input.cursor->p_col);
	ft_ti_tputs(g_escapes.ed, 1, ft_rl_putc);
	ft_ti_tputs(ft_rl_hlcolor(), 1, ft_rl_putc);
	ft_ti_tputs("example", 1, ft_rl_putc);
	ft_ti_tputs(g_escapes.sgr0, 1, ft_rl_putc);
}
