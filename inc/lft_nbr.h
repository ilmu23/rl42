/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_nbr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:51:45 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 05:00:09 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_NBR_H
# define LFT_NBR_H
# include <stdlib.h>
# include <stdint.h>
# include "lft_data.h"
# include "lft_limits.h"

/*   no alloc   */
size_t	ft_intlen(int64_t n, t_base base);
size_t	ft_uintlen(uint64_t n, t_base base);
/*   no alloc   */

#endif
