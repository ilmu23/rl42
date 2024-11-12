/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 01:58:58 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/08 19:59:10 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H
# include "ft_rl_data.h"
# include "ft_rl_keys.h"
# include "ft_rl_fnames.h"
# include "ft_rl_defines.h"

// rl42.c
char		*ft_readline(const char *prompt, const uint8_t opts);

// color.c
void		ft_rl_sethlcolor_mode(const uint8_t mode);
void		ft_rl_sethlcolor_sgr(const char *s);
void		ft_rl_sethlcolor_clr(const uint8_t color);
void		ft_rl_sethlcolor_rgb(const uint8_t r, const uint8_t g, const uint8_t b);

// utils.c
rl_cmp_fn_t	ft_rl_get_completion_fn(void);
void		ft_rl_set_completion_fn(rl_cmp_fn_t f);


/**  init  **/

/**   init.c   **/
void		ft_rl_init(void);

/**   initfile.c   **/
void		ft_rl_read_initfile(void);

/**  init  **/


/**  keys  **/

/**   bind.c   **/
uint8_t		ft_rl_bind(const char *seq, const char *fn, const rl_mapmode_t mode);
uint8_t		ft_rl_bind_emacs(const char *seq, const char *fn, const rl_mapmode_t mode);
uint8_t		ft_rl_bind_vi_ins(const char *seq, const char *fn, const rl_mapmode_t mode);
uint8_t		ft_rl_bind_vi_cmd(const char *seq, const char *fn, const rl_mapmode_t mode);
uint8_t		ft_rl_bind_hlcolor(const char *seq, const char *fn, const rl_mapmode_t mode);

uint8_t		ft_rl_const_bind(const char *seq, const char *fn);
uint8_t		ft_rl_const_bind_emacs(const char *seq, const char *fn);
uint8_t		ft_rl_const_bind_vi_ins(const char *seq, const char *fn);
uint8_t		ft_rl_const_bind_vi_cmd(const char *seq, const char *fn);
uint8_t		ft_rl_const_bind_hlcolor(const char *seq, const char *fn);

/**   fn.c   **/
uint8_t		ft_rl_addfunc(const char *fn, const rl_fn_t f);

/**   utils.c   **/
uint8_t		ft_rl_isbound(const char *seq);
uint8_t		ft_rl_unmap(const char *seq);
rl_fn_t		ft_rl_getfn(const char *seq);

/**  keys  **/


/**  settings  **/

/**   utils.c   **/
void		ft_rl_set(const char *var, const uint64_t val);

/**  settings  **/

#endif
