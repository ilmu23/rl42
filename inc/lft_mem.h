/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_mem.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:21:52 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 05:00:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_MEM_H
# define LFT_MEM_H
# include <stdlib.h>
# include <stdint.h>
# include "lft_limits.h"

/*   no alloc   */
int8_t	ft_memcmp(const void *s1, const void *s2, size_t n);

void	*ft_memset(void *s, uint8_t c, size_t n);
void	*ft_memchr(const void *s, uint8_t c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t n);

void	ft_bzero(void *s, size_t n);
/*   no alloc   */

#endif
