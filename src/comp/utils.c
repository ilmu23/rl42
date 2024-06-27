/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:55:57 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/27 17:50:21 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"
#include "ft_stdio/ft_printf.h"

static inline uint16_t	_dwidth(void);

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
void	ft_rl_comp_display(rl_input_t *input, const t_list *completions, const void *cur, const void *prv)
{
	const t_list		*blocks;
	const rl_block_t	*block;
	int16_t				pos[2];
	uint64_t			llen;
	uint16_t			rows;
	uint16_t			cpr;
	uint16_t			i;

	llen = ft_rl_comp_getlongest(completions);
	cpr = ft_max(_dwidth() / (llen + 1), 1);
	rows = *completions->size / cpr + 1;
	while (input->cursor->i_row + (int16_t)(input->len / g_cols) + rows > g_rows)
	{
		input->cursor->p_row--;
		input->cursor->i_row--;
		rows--;
	}
	ft_rl_redisplay(input, PROMPT);
	if (!cur)
	{
		i = 0;
		ft_putstr_fd(TERM_CRNL, 1);
		ft_rl_cursor_getpos(&pos[0], &pos[1]);
		while (completions)
		{
			ft_lstadd_back(&g_blocks, ft_lstnew(ft_rl_newblock(completions->blk, pos)));
			ft_printf("%-*s", llen, completions->blk);
			if (++i == cpr)
			{
				ft_putstr_fd(TERM_CRNL, 1);
				pos[0]++;
				pos[1] = 1;
				i = 0;
			}
			else
			{
				ft_putchar_fd(' ', 1);
				pos[1] += llen + 1;
			}
			completions = completions->next;
		}
	}
	else
	{
		i = 2;
		blocks = g_blocks;
		while (i && blocks)
		{
			block = blocks->blk;
			if (block->str == cur)
			{
				ft_rl_cursor_move(block->pos[0], block->pos[1]);
				ft_putstr_fd(ft_rl_hlcolor(), 1);
				ft_putstr_fd(block->str, 1);
				ft_putstr_fd(SGR_RESET, 1);
				i--;
			}
			else if (block->str == prv)
			{
				ft_rl_cursor_move(block->pos[0], block->pos[1]);
				ft_putstr_fd(block->str, 1);
				i--;
			}
			blocks = blocks->next;
		}
	}
	ft_rl_cursor_reset(input);
}

static inline uint16_t	_dwidth(void)
{
	int16_t	dwidth;

	dwidth = ft_rl_get(_CMP_DWIDTH_HASH);
	if (dwidth < 0)
		return (g_cols);
	return (ft_min(g_cols, dwidth));
}
