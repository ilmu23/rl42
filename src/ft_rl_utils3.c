/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:37:42 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 16:31:33 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline char	**gethlp(void);

size_t	ft_rl_getinputlen(t_rl_input *input)
{
	size_t		len;
	t_rl_word	*w;

	len = 0;
	w = input->head;
	while (w)
	{
		len += w->len;
		w = w->next;
	}
	return (len);
}

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
