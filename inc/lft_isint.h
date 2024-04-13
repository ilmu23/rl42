/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_isint.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 06:23:44 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 06:30:57 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_ISINT_H
# define LFT_ISINT_H
# include "lft_is.h"
# include "lft_to.h"
# include "lft_str.h"

const char	*getnbrs(t_base base);
char		*trimzeros(const char *n);

uint8_t		inbase(const char *nbrarr, const uint8_t c);

#endif
