/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_lst.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:28:48 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/14 04:59:59 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_LST_H
# define LFT_LST_H
# include <stdlib.h>
# include "lft_data.h"
# include "lft_limits.h"

/*   alloc   */
t_list	*ft_lstnew(void *blk);
t_list	*ft_lstdup(t_list *node);
/*   alloc   */

/*   no alloc   */
t_list	*ft_lstpop(t_list *node);
t_list	*ft_lstpush(t_list *node);
t_list	*ft_lstlast(t_list *list);
t_list	*ft_lstfirst(t_list *list);

void	ft_lstadd_front(t_list **list, t_list *node);
void	ft_lstadd_back(t_list **list, t_list *node);
void	ft_lstrmnode(t_list **list, t_list *node);
void	ft_lstpopall(t_list *list);
void	ft_lstpushall(t_list *list);
/*   no alloc   */

#endif
