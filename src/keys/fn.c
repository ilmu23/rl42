/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 22:58:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/30 18:20:15 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_addfunc(const char *fn, const rl_fn_t f)
{
	rl_fn_t	*fnp;

	ft_rl_init();
	fnp = __push(__alloc(sizeof(*fnp)));
	if (!fnp)
		goto err;
	*fnp = f;
	if (!__mapadd(g_funcs, fn, fnp))
		goto err;
	__popblk(fnp);
	return 1;
	err:
	__popblk(fnp);
	ft_rl_perror();
	return 0;
}
