/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:17:01 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 16:34:37 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline size_t	getplen(const char *p);
static inline char	*getline(const char *p, t_rl_histmode mode, t_list *hist);

char	*ft_readline(const char *p, t_rl_histmode mode)
{
	struct termios	old;
	struct termios	new;
	char			*out;

	ft_rl_init();
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(0, TCSANOW, &new);
	if (mode != OFF)
		out = getline(p, mode, ft_rl_hist_duphist(*ft_rl_hist_gethead()));
	else
		out = getline(p, mode, NULL);
	tcsetattr(0, TCSANOW, &old);
	return (out);
}

static inline size_t	getplen(const char *p)
{
	size_t	len;

	len = 0;
	while (*p)
	{
		if (*p == '\e')
		{
			while (*p && *p != 'm')
				p++;
			p++;
		}
		if (*p == '\e')
			continue ;
		if (!*p)
			break ;
		p++;
		len++;
	}
	return (len);
}

static inline char	*getline(const char *p, t_rl_histmode mode, t_list *hist)
{
	t_rl_input	*input;

	input = ft_push(ft_calloc(1, sizeof(*input)));
	*input = (t_rl_input){.prompt = p, .plen = getplen(p)};
	input->cursor = ft_rl_getcursor(input);
	input->cursor->col += input->plen;
	if (mode != OFF)
	{
		ft_rl_hist_add(&hist, ft_rl_dupinput(input));
		ft_rl_hist_setcurrent(hist);
	}
	else
		ft_rl_hist_setcurrent(NULL);
	ft_rl_setcurinput(input);
	ft_putstr_fd(p, 1);
	while (ft_rl_getinput(input))
		;
	if (mode == ON)
		ft_rl_hist_commit(input);
	ft_rl_hist_pop(hist);
	ft_rl_setcurinput(NULL);
	return (ft_rl_inputstr(input, 1));
}
