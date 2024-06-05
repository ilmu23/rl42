/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:55:57 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/05 10:30:23 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

uint64_t	ft_rl_comp_getlongest(const t_list *completions)
{
	uint64_t	longest;
	uint64_t	len;

	longest = 0;
	while (completions)
	{
		len = ft_strlen(completions->blk);
		if (len > longest)
			longest = len;
		completions = completions->next;
	}
	return (longest);
}

// disgusting
void	ft_rl_comp_display(rl_input_t *input, const t_list *completions, const void *cur)
{
	uint16_t	i;
	uint16_t	cpr;
	uint16_t	rows;
	uint64_t	llen;
	uint64_t	asize;
	uint64_t	cslen;
	char		*cstr;

	llen = ft_rl_comp_getlongest(completions);
	cpr = g_cols / (llen + 2);
	rows = *completions->size / cpr + 1;
	while (input->cursor->i_row + (int16_t)(input->len / g_cols) + rows > g_rows)
	{
		input->cursor->p_row--;
		input->cursor->i_row--;
		rows--;
	}
	ft_rl_redisplay(input, PROMPT);
	i = 0;
	cslen = 0;
	asize = (*completions->size * llen) + rows + ft_strlen(ft_rl_hlcolor()) + ft_strlen(SGR_RESET) + 1;
	cstr = ft_push(ft_calloc(asize, sizeof(*cstr)));
	while (completions)
	{
		if (completions->blk == cur)
			cslen += ft_snprintf(&cstr[cslen], asize - cslen, "%s%-*s%s", ft_rl_hlcolor(), llen, completions->blk, SGR_RESET);
		else
			cslen += ft_snprintf(&cstr[cslen], asize - cslen, "%-*s", llen, completions->blk);
		if (++i == cpr)
		{
			cslen += ft_snprintf(&cstr[cslen], 1, "\n");
			i = 0;
		}
		else
			cslen += ft_snprintf(&cstr[cslen], 1, " ");
		completions = completions->next;
	}
	ft_printf("\n%s", cstr);
	ft_rl_cursor_reset(input);
}
