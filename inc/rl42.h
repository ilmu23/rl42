/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl42.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 01:58:58 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 19:15:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include "ft_rl_data.h"
# include "ft_rl_keys.h"
# include "ft_rl_defines.h"

// rl42.c
char		*ft_readline(const char *prompt, uint8_t opts);

// color.c
void		ft_rl_sethlcolor(const char *s);


/**  init  **/

/**   init.c   **/
void		ft_rl_init(void);

/**  init  **/


/**  hist  **/

/**   historyfile.c   **/
void		ft_rl_hist_load(const char *path);
void		ft_rl_hist_save(const char *path);

/**  hist  **/


/**  keys  **/

/**   keymap.c   **/
void		ft_rl_map(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_addkey(const char *key, const uint64_t value);
void		ft_rl_addfunc(const char *func, const rl_fn_t f);

/**   utils.c   **/
uint8_t		ft_rl_ismapped(uint64_t key);
rl_fn_t		ft_rl_getmap(uint64_t key);
void		ft_rl_unmap(const char *key);
void		ft_rl_unmap_fn(const char *func);

/**  keys  **/

#endif
