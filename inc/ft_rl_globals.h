/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_globals.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 18:48:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_GLOBALS_H
# define FT_RL_GLOBALS_H
# include "__lft.h"
# include "ft_rl_data.h"

extern __t_hmap	*g_keys;
extern __t_hmap	*g_funcs;
extern __t_hmap	*g_map_emacs;
extern __t_hmap	*g_map_vi_ins;
extern __t_hmap	*g_map_vi_cmd;
extern __t_hmap	*g_map_hlcolor;

extern int16_t	g_rows;
extern int16_t	g_cols;

extern uint64_t	g_maxlen;
extern uint64_t	g_status;

extern rl_hlc_t	g_hlcolor;

extern rl_arg_t	g_argument;

extern rl_mark_t	g_mark_s;
extern rl_mark_t	g_mark_e;
extern rl_mark_t	g_mark_u;

extern rl_input_t	g_input;

extern rl_keybuf_t	g_keybuf;

extern rl_cmp_fn_t	g_cmp_fn;

extern rl_escapes_t	g_escapes;

extern const __t_list	*g_hist;
extern const __t_list	*g_hist_cur;
extern const __t_list	*g_kill_ring;
extern const __t_list	*g_blocks;

extern rl_settings_t	g_settings;

extern struct termios	g_oldsettings;
extern struct termios	g_newsettings;

#endif
