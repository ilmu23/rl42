/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:54:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:28:52 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_rmchar(rl_input_t *input)
{
	__strlcpy(&input->line[input->i - 1], &input->line[input->i], input->len - input->i + 1);
	input->len--;
	input->i--;
}

void	ft_rl_rmchar_back(rl_input_t *input)
{
	__strlcpy(&input->line[input->i], &input->line[input->i + 1], input->len - input->i);
	input->len--;
}
