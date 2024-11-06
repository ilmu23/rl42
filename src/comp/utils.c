/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 13:55:57 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/06 14:38:32 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline uint16_t	_dwidth(void);
static inline uint8_t	_query(rl_input_t *input, const size_t completions);

uint64_t	ft_rl_comp_getlongest(const __t_list *completions)
{
	uint64_t	longest;
	uint64_t	len;

	longest = 0;
	while (completions)
	{
		len = strlen(completions->blk);
		if (len > longest)
			longest = len;
		completions = completions->next;
	}
	return (longest);
}

// disgusting
uint8_t	ft_rl_comp_display(rl_input_t *input, const __t_list *completions, const void *cur, const void *prv)
{
	static uint16_t		cols;
	const __t_list		*blocks;
	const rl_block_t	*block;
	int16_t				pos[2];
	uint64_t			llen;
	uint16_t			rows;
	uint16_t			cpr;
	uint16_t			i;

	if (!cur && *completions->size >= ft_rl_get(_CMP_QITEMS_HASH))
	{
		if (!_query(input, *completions->size))
			return (0);
	}
	llen = ft_rl_comp_getlongest(completions);
	cpr = MAX(_dwidth() / (llen + 1), 1);
	rows = *completions->size / cpr + 1;
	while (input->cursor->i_row + (int16_t)(input->len / g_cols) + rows > g_rows)
	{
		input->cursor->p_row--;
		input->cursor->i_row--;
		rows--;
	}
	if (!cur || cols != g_cols)
	{
		i = 0;
		cols = g_cols;
		ft_rl_clearblocks();
		ft_rl_redisplay(input, PROMPT);
		ft_ti_tputs(TERM_CRNL, 1, ft_rl_putc);
		ft_rl_cursor_getpos(&pos[0], &pos[1]);
		while (completions)
		{
			__lstadd_back(&g_blocks, __lstnew(ft_rl_newblock(completions->blk, pos)));
			__printf("%-*s", (int)llen, (char *)completions->blk);
			if (++i == cpr)
			{
				ft_ti_tputs(TERM_CRNL, 1, ft_rl_putc);
				pos[0]++;
				pos[1] = 1;
				i = 0;
			}
			else
			{
				__putchar_fd(' ', 1);
				pos[1] += llen + 1;
			}
			completions = completions->next;
		}
	}
	if (cur)
	{
		i = 2;
		blocks = g_blocks;
		while (i && blocks)
		{
			block = blocks->blk;
			if (block->str == cur)
			{
				ft_rl_cursor_move(block->pos[0], block->pos[1]);
				ft_ti_tputs(ft_rl_hlcolor(), 1, ft_rl_putc);
				ft_ti_tputs(block->str, 1, ft_rl_putc);
				ft_ti_tputs(g_escapes.sgr0, 1, ft_rl_putc);
				i--;
			}
			else if (block->str == prv)
			{
				ft_rl_cursor_move(block->pos[0], block->pos[1]);
				ft_ti_tputs(block->str, 1, ft_rl_putc);
				i--;
			}
			blocks = blocks->next;
		}
	}
	ft_rl_cursor_reset(input);
	return (1);
}

static inline uint16_t	_dwidth(void)
{
	int16_t	dwidth;

	dwidth = ft_rl_get(_CMP_DWIDTH_HASH);
	if (dwidth < 0)
		return (g_cols);
	return (MIN(g_cols, dwidth));
}

static inline uint8_t	_query(rl_input_t *input, const size_t completions)
{
	uint16_t	emode;

	emode = ft_rl_geteditmode();
	ft_rl_seteditmode(_MD_SSI);
	input->cursor->row = input->cursor->i_row;
	input->cursor->col = input->cursor->i_col + input->len;
	ft_rl_cursor_setpos(input->cursor);
	__printf("\nDisplay all %zu completions? [Y/n]", completions);
	ft_rl_getinput(&input->keyseq);
	ft_rl_redisplay(input, CLEAR);
	ft_rl_seteditmode(emode);
	return (input->keyseq && tolower(*input->keyseq) == 'y');
}
