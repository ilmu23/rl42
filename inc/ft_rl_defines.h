/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_defines.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 00:19:33 by ivalimak          #+#    #+#             */
/*   Updated: 2024/12/02 13:41:36 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_DEFINES_H
# define FT_RL_DEFINES_H
# include "ft_rl_fnames.h"

// Version
# define _FT_RL_VERSION "2.2.1"

// Unused identifier
# define __UNUSED __attribute__((unused))

// Path to history file
# ifndef _FT_RL_HFILE
#  define _FT_RL_HFILE "~/.rl_history"
# endif /* _FT_RL_HFILE */

// Default init files
# define _USER_INITFILE "~/.rl42rc"
# define _SYSTEM_INITFILE "/etc/rl42rc"

// Function count
# define _FNCOUNT 76

// Argument limits
# define _ARG_MAX 1000000
# define _ARG_MIN -1000000

// Mark types
# define _MARK_START 0x1U
# define _MARK_END 0x2U
# define _MARK_USR 0x4U

// Kill directions
# define _KILL_FWD 0x1U
# define _KILL_BCK 0x2U
# define _KILL_REG 0x3U

// History traversal direction
# define _HIST_BCK 0x0FU
# define _HIST_FWD 0xF0U

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

// Statuses
# define _YLA_RUNNING 0x1ULL
# define _READ_INITFILE 0x2ULL
# define _VI_ARG 0x20ULL
# define _KILL_APPEND 0x40ULL
# define _HIDE_CURSOR 0x100ULL
# define _KILL_NOUPDATE 0x400ULL

/// modes
# define _MD_EMACS 0x4ULL
# define _MD_VI_INS 0x8ULL
# define _MD_VI_CMD 0x10ULL
# define _MD_HLCOLOR 0x80ULL
# define _MD_SSI 0x200ULL
# define _MD_MASK 0x29CULL

// Settings

/// name hashes
# define _EMODE_HASH 0x486190232D1C68DCULL
# define _BSTYLE_HASH 0xC586B53AEF5ECCBBULL
# define _CMP_DWIDTH_HASH 0xF5AEE7FFC47A60C2ULL
# define _CMP_CASE_HASH 0x38D58D56383FE6DEULL
# define _CMP_MCASE_HASH 0x7A203DAB82820392ULL
# define _CMP_QITEMS_HASH 0xAADD4F348B5E2646ULL
# define _CMP_ENABLE_HASH 0x1E6A5CB74E84D7EULL
# define _HIST_SIZE_HASH 0xB855D24FC9911D4AULL
# define _CMP_MDIRS_HASH 0xF4094B976F01307FULL
# define _CMP_MLDIRS_HASH 0x4254D0CADBED6436ULL
# define _CMP_HFILES_HASH 0x27615217B8D36228ULL
# define _CMP_HLIGHT_HASH 0x4953FE167A6F1D05ULL
# define _KEY_TMEOUT_HASH 0x1F6B9E0F52408628ULL

/// completion
# define _CMP_OFF 0x0U
# define _CMP_ON 0x1U
# define _CMP_CASE 0x2U
# define _CMP_MCASE 0x4U
# define _CMP_MARK_DIRS 0x8U
# define _CMP_MARK_LINKDIRS 0x10U
# define _CMP_HIDDEN 0x20U
# define _CMP_HIGHLIGHT 0x40U

/// bell-style
# define BELL_NONE 0x0U
# define BELL_VISIBLE 0x1U
# define BELL_AUDIBLE 0x2U

/// on/off
# define SET_OFF 0x0U
# define SET_ON 0x1U

// History modes
# define FT_RL_HIST_ON 0x1U
# define FT_RL_HIST_OFF 0x2U
# define FT_RL_HIST_NOUP 0x4U

// Highlight settings
# define FT_RL_HL_FG 0x1U
# define FT_RL_HL_BG 0x2U
# define FT_RL_HL_BOLD 0x4U
# define FT_RL_HL_ULINE 0x8U

/// Currently used highlight type
# define _HL_SGR 0x10U
# define _HL_CLR 0x20U
# define _HL_RGB 0x40U

#endif
