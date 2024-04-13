/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 12:46:51 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 12:46:57 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static inline char	**gethlp(void);

char	*ft_rl_gethlcolor(void)
{
	return (*gethlp());
}

void	ft_rl_sethlcolor(const char *s)
{
	*gethlp() = (char *)s;
}

static inline char	**gethlp(void)
{
	static char	*hlcolor = NULL;

	return (&hlcolor);
}

