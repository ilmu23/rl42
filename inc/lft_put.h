/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_put.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:26:52 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 05:00:27 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_PUT_H
# define LFT_PUT_H
# include <stdarg.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include "lft_limits.h"

# ifndef DEBUG_MSG
#  define DEBUG_MSG 0
# endif

# define DEBUGFD 2

/*   no alloc   */
ssize_t	ft_putchar_fd(uint8_t c, int32_t fd);
ssize_t	ft_putendl_fd(const char *s, int32_t fd);
ssize_t	ft_putstr_fd(const char *s, int32_t fd);
ssize_t	ft_putnbr_fd(int64_t n, int32_t fd, int *flags);
ssize_t	ft_putunbr_fd(uint64_t n, int32_t fd);
ssize_t	ft_putxnbr_fd(uint64_t n, int32_t fd, uint8_t upper);

void	ft_debugmsg(char *pfx, char *f, ...);
void	ft_debugnbr(int64_t n);
void	ft_debugunbr(uint64_t n);
void	ft_debugxnbr(uint64_t n, char f);
/*   no alloc   */

#endif
