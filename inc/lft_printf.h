/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_printf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:36:57 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 05:00:15 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_PRINTF_H
# define LFT_PRINTF_H
# include <stdlib.h>
# include <stdint.h>
# include "lft_limits.h"

ssize_t	ft_printf(const char *f, ...);
ssize_t	ft_dprintf(int32_t fd, const char *f, ...);

#endif
