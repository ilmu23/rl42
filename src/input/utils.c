/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:59:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/29 11:46:04 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

void	ft_rl_rmchar(rl_input_t *input)
{
	ft_strlcpy(&input->line[input->i - 1], &input->line[input->i], input->len - input->i + 1);
	input->len--;
	input->i--;
}

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

void	ft_rl_redisplay(const rl_input_t *input)
{
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->i - (input->i != 0);
	ft_rl_cursor_setpos(input->cursor);
	ft_printf("%s%s", TERM_CLEAR_END, &input->line[input->i - (input->i != 0)]);
	input->cursor->col = input->cursor->i_col + input->i;
	ft_rl_cursor_setpos(input->cursor);
}
