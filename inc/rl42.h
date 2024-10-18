/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 01:58:58 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:04:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H
# include "ft_rl_data.h"
# include "ft_rl_keys.h"
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

/**   keymap.c   **/
void		ft_rl_map(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_map_emacs(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_map_vi_ins(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_map_vi_cmd(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_map_hlcolor(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_addkey(const char *key, const uint64_t value);
void		ft_rl_addfunc(const char *func, const rl_fn_t f);

/**   utils.c   **/
uint8_t		ft_rl_ismapped(const uint64_t key);
rl_fn_t		ft_rl_getmap(const uint64_t key);
void		ft_rl_unmap(const char *key);
void		ft_rl_unmap_fn(const char *func);
void		ft_rl_unmap_all(void);

/**  keys  **/


/**  settings  **/

/**   utils.c   **/
void		ft_rl_set(const char *var, const uint64_t val);

/**  settings  **/

#endif
