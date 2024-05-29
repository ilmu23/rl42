/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:55:31 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 13:55:32 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_addchar(rl_input_t *input, const uint8_t c)
{
	char	*newline;

	newline = NULL;
	if (ft_getblksize(input->line) == input->len + 1)
	{
		newline = ft_push(ft_calloc((input->len * 3) + 1, sizeof(*newline)));
		if (!newline)
			exit(ft_rl_perror());
	}
	if (newline)
	{
		ft_strlcpy(newline, input->line, input->i + 1);
		newline[input->i] = c;
		ft_strlcpy(&newline[input->i + 1], &input->line[input->i], input->len);
		ft_popblk(input->line);
		input->line = newline;
	}
	else
	{
		ft_memmove(&input->line[input->i + 1], &input->line[input->i], input->len - input->i + 1);
		input->line[input->i] = c;
	}
	input->len++;
	input->i++;
}
