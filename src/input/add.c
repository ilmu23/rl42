/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:55:31 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 02:46:06 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_addchar(rl_input_t *input, const uint8_t c)
{
	char	*newline;

	newline = NULL;
	if (!input->line || __getblksize(input->line) <= input->len + 4)
	{
		newline = __push(__calloc(((input->len + 1) * 3) + 1, sizeof(*newline)));
		if (!newline)
			exit(ft_rl_perror());
	}
	if (newline)
	{
		__strlcpy(newline, input->line, input->i + 1);
		newline[input->i] = c;
		__strlcpy(&newline[input->i + 1], &input->line[input->i], input->len + 1);
		__popblk(input->line);
		input->line = newline;
	}
	else
	{
		memmove(&input->line[input->i + 1], &input->line[input->i], input->len - input->i + 1);
		input->line[input->i] = c;
	}
	input->len++;
	input->i++;
}
