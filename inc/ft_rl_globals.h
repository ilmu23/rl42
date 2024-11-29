/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_globals.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 18:48:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 16:36:56 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_GLOBALS_H
# define FT_RL_GLOBALS_H
# include "__lft.h"
# include "ft_rl_data.h"

extern int16_t	g_rows;
extern int16_t	g_cols;

extern uint64_t	g_maxlen;
extern uint64_t	g_status;

extern rl_hlc_t	g_hlcolor;

extern rl_arg_t	g_argument;

extern __t_hmap	*g_funcs;
extern __t_hmap	*g_binds;
extern __t_hmap	*g_macrodata[3];

extern rl_mark_t	g_mark_s;
extern rl_mark_t	g_mark_e;
extern rl_mark_t	g_mark_u;

extern rl_input_t	g_input;

extern rl_fninfo_t	g_macro;

extern rl_cmp_fn_t	g_cmp_fn;

extern rl_escapes_t	g_escapes;

extern const __t_list	*g_hist;
extern const __t_list	*g_hist_cur;
extern const __t_list	*g_hist_init;
extern const __t_list	*g_kill_ring;
extern const __t_list	*g_blocks;

extern rl_settings_t	g_settings;

extern struct termios	g_oldsettings;
extern struct termios	g_newsettings;

extern const rl_keytree_t	g_emptynode;

#endif
