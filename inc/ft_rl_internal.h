/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:34:49 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 02:24:48 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_INTERNAL_H
# define FT_RL_INTERNAL_H
# include "ft_readline.h"
# include "ft_rl_funcs.h"

extern t_hmap	*g_keys;
extern t_hmap	*g_funcs;
extern t_hmap	*g_maps;

// ft_rl_error.c
int32_t	ft_rl_perror(void);

// ft_rl_exec.c
uint8_t	ft_rl_execmap(rl_input_t *input);

// ft_rl_initkeys.c
void	ft_rl_initkeys(void);

// ft_rl_initfuncs.c
void	ft_rl_initfuncs(void);

// ft_rl_keymap_utils.c
char	*ft_rl_keystr(const uint64_t key);

#endif
