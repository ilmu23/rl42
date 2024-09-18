/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 05:59:26 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/18 16:09:57 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline uint8_t	_islastword(const rl_input_t *input);
static inline void		_mark(const rl_input_t *input);

void	ft_rl_redisplay(const rl_input_t *input, const rl_rdmode_t mode)
{
	uint16_t	rows;
	const char	*clr;

	switch (mode)
	{
		case LINE:
			input->cursor->row = input->cursor->i_row;
			input->cursor->col = input->cursor->i_col + input->i - (input->i != 0);
			break ;
		case INPUT:
			input->cursor->row = input->cursor->i_row;
			input->cursor->col = input->cursor->i_col;
			break ;
		case PROMPT:
			input->cursor->row = input->cursor->p_row;
			input->cursor->col = input->cursor->p_col;
			break ;
		case SPROMPT:
			input->cursor->row = input->cursor->p_row;
			input->cursor->col = input->cursor->p_col;
			break ;
		case CLEAR:
			input->cursor->row = input->cursor->p_row;
			input->cursor->col = input->cursor->p_col;
			ft_rl_cursor_setpos(input->cursor);
			__printf("%s%s%s", TERM_CLEAR_END, input->prompt, input->line);
			input->cursor->col = input->cursor->i_col + input->i;
			ft_rl_cursor_setpos(input->cursor);
			return ;
	}
	rows = (input->cursor->col + input->len) / g_cols + 1;
	clr = __strdup(TERM_CLEAR_LINE_END);
	while (--rows)
		clr = __strnjoin(3, clr, TERM_CRNL, TERM_CLEAR_LINE_END);
	__push(clr);
	ft_rl_cursor_setpos(input->cursor);
	ft_ti_tputs(clr, 1, ft_rl_putc);
	ft_rl_cursor_setpos(input->cursor);
	switch (mode)
	{
		case LINE:
			ft_ti_tputs(&input->line[input->i - (input->i != 0)], 1, ft_rl_putc);
			break ;
		case INPUT:
			ft_ti_tputs(input->line, 1, ft_rl_putc);
			break;
		case PROMPT:
			ft_ti_tputs(input->prompt, 1, ft_rl_putc);
			ft_ti_tputs(input->line, 1, ft_rl_putc);
			break ;
		case SPROMPT:
			__printf("%s %s%s", input->sprompt, input->prompt, input->line);
			break ;
		case CLEAR:
			break ;
	}
	if (g_mark_u.set && input == &g_input)
		_mark(input);
	__popblk(clr);
	input->cursor->col = input->cursor->i_col + input->i;
	if (mode == SPROMPT)
		input->cursor->col += strlen(input->sprompt) + 1;
	ft_rl_cursor_setpos(input->cursor);
}

void	ft_rl_swap_words(rl_input_t *input)
{
	uint64_t	tmp;
	uint64_t	marks[4];
	const char	*subs[2];

	if (input->i == input->len)
		input->i--;
	while (input->i > 0 && isspace(input->line[input->i]))
		input->i--;
	ft_rl_word_start();
	marks[0] = input->i;
	ft_rl_word_end();
	marks[1] = MIN(input->i + 1, input->len);
	if (!_islastword(input))
		ft_rl_fwd_w(input);
	else
	{
		ft_rl_bck_w(input);
		ft_rl_bck_w(input);
	}
	marks[2] = input->i;
	ft_rl_word_end();
	input->i += (input->i < input->len);
	marks[3] = input->i;
	if (marks[1] > marks[2])
	{
		tmp = marks[0];
		marks[0] = marks[2];
		marks[2] = tmp;
		tmp = marks[1];
		marks[1] = marks[3];
		marks[3] = tmp;
	}
	subs[0] = __push(__substr(input->line, marks[2], marks[3] - marks[2]));
	subs[1] = __push(__substr(input->line, marks[0], marks[1] - marks[0]));
	if (!subs[0] || !subs[1])
		exit(ft_rl_perror());
	__snprintf(&input->line[marks[0]], marks[3] - marks[0], "%-*s%s",
			(int)(marks[3] - marks[0] - strlen(subs[1])), subs[0], subs[1]);
	__popblks(2, subs[0], subs[1]);
	input->i = marks[3];
}

void	ft_rl_unsetmark(uint8_t type)
{
	switch (type)
	{
		case _MARK_START:
			g_mark_s.set = 0;
			break ;
		case _MARK_END:
			g_mark_e.set = 0;
			break ;
		case _MARK_START | _MARK_END:
			g_mark_s.set = 0;
			g_mark_e.set = 0;
			break ;
		case _MARK_USR:
			g_mark_u.set = 0;
			break ;
	}
}

void	ft_rl_setmark(uint8_t type)
{
	switch (type)
	{
		case _MARK_START:
			g_mark_s.set = 1;
			g_mark_s.pos = g_input.i;
			break ;
		case _MARK_END:
			g_mark_e.set = 1;
			g_mark_e.pos = g_input.i;
			break ;
		case _MARK_START | _MARK_END:
			g_mark_s.set = 1;
			g_mark_e.set = 1;
			g_mark_s.pos = g_input.i;
			g_mark_e.pos = g_input.i;
			break ;
		case _MARK_USR:
			g_mark_u.set = 1;
			g_mark_u.pos = g_input.i;
			break ;
	}
}

void	ft_rl_word_start(void)
{
	if (g_input.i == 0)
		return ;
	while (g_input.i > 0 && !isspace(g_input.line[g_input.i - 1]))
		g_input.i--;
}

void	ft_rl_word_end(void)
{
	if (g_input.i == g_input.len)
		return ;
	while (g_input.i < g_input.len && !isspace(g_input.line[g_input.i + 1]))
		g_input.i++;
}

static inline uint8_t	_islastword(const rl_input_t *input)
{
	uint64_t	i;

	i = input->i;
	while (++i < input->len)
	{
		if (!isspace(input->line[i]))
			return (0);
	}
	return (1);
}

static inline void	_mark(const rl_input_t *input)
{
	uint8_t	splen;

	splen = 0;
	if (input->sprompt)
		splen = strlen(input->sprompt) + 1;
	input->cursor->col = input->cursor->i_col + MIN(g_mark_u.pos + splen, input->len);
	ft_rl_cursor_setpos(input->cursor);
	if (g_mark_u.pos < input->len)
		__printf("%s%c%s", g_escapes.smul, input->line[g_mark_u.pos], g_escapes.rmul);
	else
		__printf("%s %s", g_escapes.smul, g_escapes.rmul);
}
