/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:12:36 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 11:40:35 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_acl(rl_input_t *input)
{
	ft_putstr_fd(TERM_CRNL, 1);
	input->exittype = ACL;
	return (0);
	(void)input;
}

uint8_t	ft_rl_pvh(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_nxh(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_soh(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_eoh(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_rsh(rl_input_t *input)
{
	return (1);
	(void)input;
}

uint8_t	ft_rl_fsh(rl_input_t *input)
{
	return (1);
	(void)input;
}
