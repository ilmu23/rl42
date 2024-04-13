/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_is.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:23:14 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/15 20:38:56 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_IS_H
# define LFT_IS_H
# include <stdint.h>
# include "lft_data.h"
# include "lft_limits.h"

/*   no alloc   */
uint8_t	ft_isalnum(uint8_t c);
uint8_t	ft_isalpha(uint8_t c);
uint8_t	ft_isascii(uint8_t c);
uint8_t	ft_isdigit(uint8_t c);
uint8_t	ft_islower(uint8_t c);
uint8_t	ft_isprint(uint8_t c);
uint8_t	ft_isspace(uint8_t c);
uint8_t	ft_isupper(uint8_t c);
uint8_t	ft_isint(const char *n, size_t tsize);
uint8_t	ft_isint_base(const char *n, size_t tsize, t_base base);
uint8_t	ft_isuint(const char *n, size_t tsize);
uint8_t	ft_isuint_base(const char *n, size_t tsize, t_base base);
/*   no alloc   */

#endif
