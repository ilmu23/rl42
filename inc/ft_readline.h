/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 01:58:58 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 10:45:44 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_READLINE_H
# define FT_READLINE_H
# include "libft.h"
# include "ft_rl_keys.h"
# include "ft_rl_term.h"
# include "ft_rl_data.h"
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <sys/stat.h>
# include <sys/ioctl.h>

# ifndef RL_DEBUG_MSG
#  define RL_DEBUG_MSG 0
# endif

# ifndef RL_HFNAME
#  define RL_HFNAME ".rl_history"
# endif

# ifndef RL_HISTORY_SIZE
#  define RL_HISTORY_SIZE 100
# endif

// ft_readline.c
char		*ft_readline(const char *prompt, t_rl_histmode mode);

// ft_rl_init.c
void		ft_rl_init(void);

// ft_rl_history_file.c
void		ft_rl_hist_load(const char *fname);
void		ft_rl_hist_save(const char *fname);

// ft_rl_wildcard.c
t_rl_wc		*ft_rl_wildcard_expand(const char *pattern);

// ft_rl_keymap.c
void		ft_rl_map(const char *key, const char *func, t_rl_mapmode mode);

// ft_rl_utils3.c
void		ft_rl_sethlcolor(const char *s);

// ft_rl_input_utils.c
t_rl_input	*ft_rl_getcurinput(void);

// ft_rl_keymap_utils.c
uint8_t		ft_rl_ismapped(uint64_t key);
t_rl_fn		ft_rl_getmap(uint64_t key);
void		ft_rl_unmap(const char *key);
void		ft_rl_unmap_fn(const char *func);

#endif
