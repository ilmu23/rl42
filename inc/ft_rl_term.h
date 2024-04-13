/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_term.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 19:48:28 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 15:51:49 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_TERM_H
# define FT_RL_TERM_H
# include "libft.h"
# include "ft_rl_data.h"

// misc
# define TERM_STATUS "\e[6n"
# define TERM_CUR_SOL "\e[1G"
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
# define TERM_SCROLL_UP "\e[1S"
# define TERM_SCROLL_DOWN "\e[1T"

// sgr

/// misc
# define SGR_RESET "\e[0m"
# define SGR_BOLDON "\e[1m"
# define SGR_BOLDOFF "\e[22m"
# define SGR_ULINEON "\e[4m"
# define SGR_ULINEOFF "\e[24m"

/// colors

//// foreground
# define SGR_FG0 "\e[30m" // BLACK
# define SGR_FG1 "\e[31m" // RED
# define SGR_FG2 "\e[32m" // GREEN
# define SGR_FG3 "\e[33m" // YELLOW
# define SGR_FG4 "\e[34m" // BLUE
# define SGR_FG5 "\e[35m" // MAGENTA
# define SGR_FG6 "\e[36m" // CYAN
# define SGR_FG7 "\e[37m" // WHITE
# define SGR_FGR "\e[39m" // RESET

//// background
# define SGR_BG0 "\e[40m" // BLACK
# define SGR_BG1 "\e[41m" // RED
# define SGR_BG2 "\e[42m" // GREEN
# define SGR_BG3 "\e[43m" // YELLOW
# define SGR_BG4 "\e[44m" // BLUE
# define SGR_BG5 "\e[45m" // MAGENTA
# define SGR_BG6 "\e[46m" // CYAN
# define SGR_BG7 "\e[47m" // WHITE
# define SGR_BGR "\e[49m" // RESET

#endif
