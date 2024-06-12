/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_fns.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 01:33:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 03:42:38 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_FUNCS_H
# define FT_RL_FUNCS_H
# include "rl42.h"

/**  fns  **/

/**   move.c   **/
uint8_t	ft_rl_sol(rl_input_t *input);
uint8_t	ft_rl_eol(rl_input_t *input);
uint8_t	ft_rl_sow(rl_input_t *input);
uint8_t	ft_rl_eow(rl_input_t *input);
uint8_t	ft_rl_fwd(rl_input_t *input);
uint8_t	ft_rl_bck(rl_input_t *input);
uint8_t	ft_rl_fwd_w(rl_input_t *input);
uint8_t	ft_rl_bck_w(rl_input_t *input);
uint8_t	ft_rl_clr(rl_input_t *input);
uint8_t	ft_rl_ffc(rl_input_t *input);
uint8_t	ft_rl_bfc(rl_input_t *input);

/**   hist.c   **/
uint8_t	ft_rl_acl(rl_input_t *input);
uint8_t	ft_rl_pvh(rl_input_t *input);
uint8_t	ft_rl_nxh(rl_input_t *input);
uint8_t	ft_rl_soh(rl_input_t *input);
uint8_t	ft_rl_eoh(rl_input_t *input);
uint8_t	ft_rl_rsh(rl_input_t *input);
uint8_t	ft_rl_fsh(rl_input_t *input);
uint8_t	ft_rl_rsh_i(rl_input_t *input);
uint8_t	ft_rl_fsh_i(rl_input_t *input);

/**   text.c   **/
uint8_t	ft_rl_eof(rl_input_t *input);
uint8_t	ft_rl_del(rl_input_t *input);
uint8_t	ft_rl_bdl(rl_input_t *input);
uint8_t	ft_rl_ins(rl_input_t *input);
uint8_t	ft_rl_upw(rl_input_t *input);
uint8_t	ft_rl_dnw(rl_input_t *input);
uint8_t	ft_rl_caw(rl_input_t *input);

/**   comp.c   **/
uint8_t	ft_rl_cmp(rl_input_t *input);

/**   misc.c   **/
uint8_t	ft_rl_mta(rl_input_t *input);
uint8_t	ft_rl_dcl(rl_input_t *input);
uint8_t	ft_rl_hlc(rl_input_t *input);
uint8_t	ft_rl_hlm(rl_input_t *input);

/**  fns  **/

#endif
