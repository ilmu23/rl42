/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_term.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 19:48:28 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/02 07:14:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_TERM_H
# define FT_RL_TERM_H
# include "ft_rl_data.h"

// misc
# define TERM_CRNL "\r\n"
# define TERM_STATUS "\e[6n"

// bell
# define TERM_RING_BELL "\a"
# define TERM_FLASH_SCREEN "\e[?5h$<100/>\e[?5l"

// cursor
# define TERM_CUR_SOL "\e[1G"
# define TERM_CUR_HIDE "\e[?25l"
# define TERM_CUR_SHOW "\e[?25h"
# define TERM_CUR_RESET "\e[1;1H"

// clear
# define TERM_CLEAR_END "\e[0J"
# define TERM_CLEAR_START "\e[1J"
# define TERM_CLEAR_SCREEN "\e[2J"
# define TERM_CLEAR_BUFFER "\e[3J"
# define TERM_CLEAR_LINE_END "\e[0K"
# define TERM_CLEAR_LINE_START "\e[1K"
# define TERM_CLEAR_LINE_WHOLE "\e[2K"

// scroll
# define TERM_SCROLL_UP_N "\e[%p1%dS"
# define TERM_SCROLL_DOWN_N "\e[%p1%dT"

// bracketed paste mode
# define TERM_BPM_ON "\e[?2004h"
# define TERM_BPM_OFF "\e[?2004l"

// sgr

# define SGR_RESET "\e[0m"
# define SGR_BOLDON "\e[1m"
# define SGR_ULINEON "\e[4m"
# define SGR_ULINEOFF "\e[24m"

/// colors
# define SGR_FG0 "\e[30m"
# define SGR_FG1 "\e[31m"
# define SGR_FG2 "\e[32m"
# define SGR_FG3 "\e[33m"
# define SGR_FG4 "\e[34m"
# define SGR_FG5 "\e[35m"
# define SGR_FG6 "\e[36m"
# define SGR_FG7 "\e[37m"

#endif
