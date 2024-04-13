/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_initfuncs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:59:29 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 11:10:34 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	addfunc(t_list **funcs, t_rl_func *func);

t_rl_func	*ft_rl_newfunc(const char *name, t_rl_fn f)
{
	t_rl_func	*func;

	func = ft_alloc(sizeof(*func));
	if (!func)
		return (NULL);
	ft_memcpy(func, &(t_rl_func){.name = name, .f = f}, sizeof(*func));
	return (func);
}

void	ft_rl_initfuncs(t_list **funcs)
{
	addfunc(funcs, ft_rl_newfunc("self-insert", ft_rl_ins));
	addfunc(funcs, ft_rl_newfunc("remove-char", ft_rl_del));
	addfunc(funcs, ft_rl_newfunc("beginning-of-line", ft_rl_sol));
	addfunc(funcs, ft_rl_newfunc("end-of-line", ft_rl_eol));
	addfunc(funcs, ft_rl_newfunc("forward-char", ft_rl_fwd));
	addfunc(funcs, ft_rl_newfunc("backward-char", ft_rl_bck));
	addfunc(funcs, ft_rl_newfunc("forward-word", ft_rl_fwd_w));
	addfunc(funcs, ft_rl_newfunc("backward-word", ft_rl_bck_w));
	addfunc(funcs, ft_rl_newfunc("clear-screen", ft_rl_clr));
	addfunc(funcs, ft_rl_newfunc("accept-line", ft_rl_acl));
	addfunc(funcs, ft_rl_newfunc("end-of-file", ft_rl_eof));
	addfunc(funcs, ft_rl_newfunc("previous-history", ft_rl_pvh));
	addfunc(funcs, ft_rl_newfunc("next-history", ft_rl_nxh));
	addfunc(funcs, ft_rl_newfunc("beginning-of-history", ft_rl_soh));
	addfunc(funcs, ft_rl_newfunc("end-of-history", ft_rl_eoh));
	addfunc(funcs, ft_rl_newfunc("reverse-search-history", ft_rl_rsh));
	addfunc(funcs, ft_rl_newfunc("forward-search-history", ft_rl_fsh));
	addfunc(funcs, ft_rl_newfunc("upcase-word", ft_rl_upw));
	addfunc(funcs, ft_rl_newfunc("downcase-word", ft_rl_dnw));
	addfunc(funcs, ft_rl_newfunc("capitalize-word", ft_rl_caw));
	addfunc(funcs, ft_rl_newfunc("complete", ft_rl_cmp));
	addfunc(funcs, ft_rl_newfunc("prefix-meta", ft_rl_mta));
	addfunc(funcs, ft_rl_newfunc("discard-line", ft_rl_dcl));
	addfunc(funcs, ft_rl_newfunc("set-highlight-color", ft_rl_hlc));
	addfunc(funcs, ft_rl_newfunc("rl-dbg-print-input", ft_rl_dbg_printinput));
}

static inline void	addfunc(t_list **funcs, t_rl_func *func)
{
	ft_lstadd_front(funcs, ft_lstnew(func));
}
