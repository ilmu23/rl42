/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:28:40 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 19:07:29 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_execmap(rl_input_t *input)
{
	rl_map_t	*mapping;

	mapping = ft_mapget(g_maps, input->keystr);
	return ((*mapping->f)(input));
}
