/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:14:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/27 00:47:36 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint8_t	ft_rl_eof(rl_input_t *input)
{
	ft_printf("ft_rl_eof!\n");
	exit (0);
	return (1);
	(void)input;
}

uint8_t	ft_rl_del(rl_input_t *input)
{
	ft_printf("ft_rl_del!\n");
	return (1);
	(void)input;
}

uint8_t	ft_rl_ins(rl_input_t *input)
{
	ft_printf("ft_rl_ins!\n");
	return (1);
	(void)input;
}

uint8_t	ft_rl_upw(rl_input_t *input)
{
	ft_printf("ft_rl_upw!\n");
	return (1);
	(void)input;
}

uint8_t	ft_rl_dnw(rl_input_t *input)
{
	ft_printf("ft_rl_dnw!\n");
	return (1);
	(void)input;
}

uint8_t	ft_rl_caw(rl_input_t *input)
{
	ft_printf("ft_rl_caw!\n");
	return (1);
	(void)input;
}
