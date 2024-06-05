/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_globals.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 18:48:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 21:28:06 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_GLOBALS_H
# define FT_RL_GLOBALS_H
# include "libft.h"
# include "ft_rl_data.h"

extern t_hmap	*g_keys;
extern t_hmap	*g_funcs;
extern t_hmap	*g_maps;

extern int16_t	g_rows;
extern int16_t	g_cols;

extern uint64_t	g_maxlen;
extern uint64_t	g_mark_s;
extern uint64_t	g_mark_e;

extern rl_hlc_t	g_hlcolor;

extern rl_input_t	g_input;

extern rl_keybuf_t	g_keybuf;

extern const t_list	*g_hist;
extern const t_list	*g_hist_cur;
extern const t_list	*g_blocks;

extern struct termios	g_oldsettings;
extern struct termios	g_newsettings;

#endif
