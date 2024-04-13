/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_debug_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 20:45:44 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 15:37:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

uint8_t	ft_rl_dbg_printinput(t_rl_input *input)
{
	size_t		node;
	t_rl_word	*w;

	node = 0;
	w = input->head;
	while (w)
	{
		ft_dprintf(2, "node %3u:\t'%-50s'", node++, w->word);
		ft_dprintf(2, "\t%u[%u]", w->len, w->i);
		if (w->wtype == SPACE)
			ft_dprintf(2, "\t SPACE\n");
		else
			ft_dprintf(2, "\tNORMAL\n");
		w = w->next;
	}
	return (1);
}


void	ft_rl_dbg_info(t_rl_input *input, uint64_t key)
{
	if (!RL_DEBUG_MSG)
		return ;
	ft_dprintf(2, "curpos: %d;%d\n", input->cursor->row, input->cursor->col);
	ft_dprintf(2, "curnode: %p\n", input->current);
	if (input->current)
		ft_dprintf(2, "curnode->word: [%s][%u]\n", input->current->word,
				input->current->i);
	if (ft_rl_ismapped(key))
		ft_dprintf(2, "key %X mapped to %p\n", key, ft_rl_getmap(key));
	else
		ft_dprintf(2, "key %X not mapped\n", key);
}
