/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_defines.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 00:19:33 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 05:21:19 by ivalimak         ###   ########.fr       */
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

// History modes
# define FT_RL_HIST_ON 0x1U
# define FT_RL_HIST_OFF 0x2U
# define FT_RL_HIST_NOUP 0x4U

#endif
