/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 01:58:58 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 00:53:37 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H
# include <errno.h>
# include <fcntl.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include "libft.h"
# include "ft_stdio/ft_printf.h"
# include "ft_rl_data.h"
# include "ft_rl_keys.h"
# include "ft_rl_term.h"
# include "ft_rl_defines.h"

// ft_readline.c
char		*ft_readline(const char *prompt, uint8_t opts);

// ft_rl_init.c
void		ft_rl_init(void);

// ft_rl_color.c
void		ft_rl_sethlcolor(const char *s);

// ft_rl_history_file.c
void		ft_rl_hist_load(const char *fname);
void		ft_rl_hist_save(const char *fname);

// ft_rl_keymap.c
void		ft_rl_map(const char *key, const char *func, rl_mapmode_t mode);
void		ft_rl_addkey(const char *key, const uint64_t value);
void		ft_rl_addfunc(const char *func, const rl_fn_t f);

// ft_rl_keymap_utils.c
uint8_t		ft_rl_ismapped(uint64_t key);
rl_fn_t		ft_rl_getmap(uint64_t key);
void		ft_rl_unmap(const char *key);
void		ft_rl_unmap_fn(const char *func);

#endif
