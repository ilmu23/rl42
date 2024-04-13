/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_printf_internal.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:39:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 05:19:59 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_PRINTF_INTERNAL_H
# define LFT_PRINTF_INTERNAL_H
# include <stdarg.h>
# include "lft_printf.h"

void	ft_pfsetfd(int fd);

int		ft_pfgetfd(void);
int		ft_printformat(const char **f, va_list args);
int		ft_putarg(const char **f, va_list argc);
int		ft_putc(va_list args, int *flags, char format);
int		ft_putd(va_list args, int *flags);
int		ft_putu(va_list args, int *flags);
int		ft_puts(va_list args, int *flags);
int		ft_putx(va_list args, int *flags, char format);
int		ft_putp(va_list args, int *flags);
int		putpadding(int spaces, char c, int *sign, int *flags);

#endif
