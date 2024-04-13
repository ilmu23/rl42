/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_to.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:43:40 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 04:47:13 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_TO_H
# define LFT_TO_H
# include <stdlib.h>
# include <stdint.h>
# include "lft_data.h"
# include "lft_limits.h"

/*   alloc   */
char		*ft_itoa(int64_t n);
char		*ft_itoa_base(int64_t n, t_base base);
char		*ft_utoa(uint64_t n);
char		*ft_utoa_base(uint64_t n, t_base base);
/*   alloc   */

/*   no alloc   */
int64_t		ft_atoi_base(const char *s, t_base base);
int64_t		ft_atoi64(const char *s);
int32_t		ft_atoi32(const char *s);
int16_t		ft_atoi16(const char *s);
int8_t		ft_atoi8(const char *s);
int			ft_atoi(const char *s);

uint64_t	ft_atou_base(const char *s, t_base base);
uint64_t	ft_atou64(const char *s);
uint32_t	ft_atou32(const char *s);
uint16_t	ft_atou16(const char *s);
uint8_t		ft_atou8(const char *s);

uint8_t		ft_toupper(uint8_t c);
uint8_t		ft_tolower(uint8_t c);
/*   no alloc   */

#endif
