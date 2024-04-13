/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_initkeys.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:44:17 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/28 15:47:17 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	ft_rl_initkeys2(t_list **keys);
static inline void	ft_rl_initkeys3(t_list **keys);

t_rl_key	*ft_rl_newkey(const char *name, const uint64_t value)
{
	t_rl_key	*key;

	key = ft_alloc(sizeof(*key));
	if (!key)
		return (NULL);
	ft_memcpy(key, &(t_rl_key){.name = name, .value = value}, sizeof(*key));
	return (key);
}

void	ft_rl_initkeys(t_list **keys)
{
	static char		key[6] = "!";
	static uint8_t	val = KEY_BANG;

	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<SPC>", KEY_SPACE)));
	while (val <= KEY_TILDE)
	{
		ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey(ft_push(ft_strdup(key)),
					val++)));
		*key = val;
	}
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<BCK>", KEY_BACKSPACE)));
	ft_strlcpy(key, "<C-a>", 6);
	val = KEY_CTRL_A;
	while (val <= KEY_CTRL_Z)
	{
		ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey(ft_push(ft_strdup(key)),
					val++)));
		key[3] = val + 0x60U;
	}
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-1>", KEY_CTRL_1)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-2>", KEY_CTRL_2)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-3>", KEY_CTRL_3)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-4>", KEY_CTRL_4)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-5>", KEY_CTRL_5)));
	ft_rl_initkeys2(keys);
}

static inline void	ft_rl_initkeys2(t_list **keys)
{
	static char		key[6] = "<M-!>";
	static uint64_t	val = KEY_ALT_BANG;

	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-6>", KEY_CTRL_6)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-7>", KEY_CTRL_7)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-8>", KEY_CTRL_8)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<C-9>", KEY_CTRL_9)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<M-SPC>", KEY_ALT_SPACE)));
	while (val <= KEY_ALT_TILDE)
	{
		ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey(ft_push(ft_strdup(key)),
					val)));
		val += 0x1U << 8;
		key[3] = val >> 8;
	}
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<M-BCK>",
				KEY_ALT_BACKSPACE)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<up>", KEY_UP)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<down>", KEY_DOWN)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<left>", KEY_LEFT)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<right>", KEY_RIGHT)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<ESC>", KEY_ESC)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<TAB>", KEY_TAB)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<RET>", KEY_RETURN)));
	ft_rl_initkeys3(keys);
}

static inline void	ft_rl_initkeys3(t_list **keys)
{
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<M-TAB>", KEY_ALT_TAB)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<M-RET>", KEY_ALT_RETURN)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<PRT>", KEY_PRINT)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<SCR>", KEY_SCRLL)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<PSE>", KEY_PAUSE)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<INS>", KEY_INS)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<HME>", KEY_HOME)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<PGU>", KEY_PGUP)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<DEL>", KEY_DEL)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<END>", KEY_END)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<PGD>", KEY_PGDN)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-1>", KEY_F_1)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-2>", KEY_F_2)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-3>", KEY_F_3)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-4>", KEY_F_4)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-5>", KEY_F_5)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-6>", KEY_F_6)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-7>", KEY_F_7)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-8>", KEY_F_8)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-9>", KEY_F_9)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-10>", KEY_F_10)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-11>", KEY_F_11)));
	ft_lstadd_front(keys, ft_lstnew(ft_rl_newkey("<F-12>", KEY_F_12)));
}
