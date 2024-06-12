/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_defines.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 00:19:33 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 04:04:29 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_DEFINES_H
# define FT_RL_DEFINES_H

// Version
# define _FT_RL_VERSION "2.0.0"

// Path to history file
# ifndef _FT_RL_HFILE
#  define _FT_RL_HFILE "~/.rl_history"
# endif /* _FT_RL_HFILE */

// History size
# ifndef _FT_RL_HSIZE
#  define _FT_RL_HSIZE 100
# endif /* _FT_RL_HSIZE */

// Keystr buffer length
# define _KEYSTR_LEN 16

// Keyshift mask
# define _KEYSHIFT_MASK 0x00000000000000FFULL

// Mark types
# define _MARK_START 0x1U
# define _MARK_END 0x2U
# define _MARK_USR 0x4U

// Kill directions
# define _KILL_FWD 0x1U
# define _KILL_BCK 0x2U

// History search direction
# define _SEARCH_BCK 0x0U
# define _SEARCH_FWD 0x1U
# define _I_SEARCH_BCK 0x2U
# define _I_SEARCH_FWD 0x3U

// History search prompts
# define _SEARCH_BCK_PROMPT "bck-search: "
# define _SEARCH_FWD_PROMPT "fwd-search: "
# define _I_SEARCH_BCK_PROMPT "failing-bck-i-search: "
# define _I_SEARCH_FWD_PROMPT "failing-fwd-i-search: "

// History modes
# define FT_RL_HIST_ON 0x1U
# define FT_RL_HIST_OFF 0x2U
# define FT_RL_HIST_NOUP 0x4U

// Highlight fg/bg
# define FT_RL_HL_FG 0x0U
# define FT_RL_HL_BG 0x1U

#endif
