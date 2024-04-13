/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:37:42 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 12:52:01 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline size_t	*_getmaxlen(void);

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

size_t	ft_rl_input_getmaxlen(void)
{
	return (*_getmaxlen());
}

void	ft_rl_input_setmaxlen(size_t maxlen)
{
	*_getmaxlen() = maxlen;
}

static inline size_t	*_getmaxlen(void)
{
	static size_t	maxlen;

	return (&maxlen);
}
