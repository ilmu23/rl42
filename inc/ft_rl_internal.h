/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:34:49 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/06 15:31:08 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_INTERNAL_H
# define FT_RL_INTERNAL_H
# include "rl42.h"
# include "ti42.h"
# include "__lft.h"
# include "ft_rl_fns.h"
# include "ft_rl_term.h"
# include "ft_rl_globals.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <unistd.h>
# include <termios.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/param.h>
# include <sys/select.h>

// error.c
int32_t			ft_rl_perror(void);

// color.c
const char		*ft_rl_hlcolor(void);

// kring.c
char			*ft_rl_kring_yank(void);
void			ft_rl_kring_rotate(void);

// utils.c
rl_block_t		*ft_rl_newblock(const char *str, const int16_t pos[2]);
uint16_t		ft_rl_geteditmode(void);
rl_fn_t			ft_rl_getinput(const char **seqstore);
ssize_t			ft_rl_putc(const int8_t c);
int32_t			ft_rl_getarg(void);
uint8_t			ft_rl_isdir(const char *path);
void			ft_rl_seteditmode(const uint16_t node);
void			ft_rl_clearblocks(void);
void			ft_rl_bell(void);


/**  comp  **/

/**   completion.c   **/
__t_list		*ft_rl_complete_path(const char *pattern, const rl_input_t *context);
rl_fn_t			ft_rl_complete(rl_input_t *input);

/**   utils.c   **/
uint64_t		ft_rl_comp_getlongest(const __t_list *completions);
uint8_t			ft_rl_comp_display(rl_input_t *input, const __t_list *completions, const void *cur, const void *prv);

/**  comp  **/


/**  term  **/

/**   cursor.c   **/
rl_cursor_t		*ft_rl_cursor_init(void);
void			ft_rl_cursor_getpos(int16_t *row, int16_t *col);
void			ft_rl_cursor_setpos(rl_cursor_t *cursor);
void			ft_rl_cursor_reset(rl_input_t *input);
void			ft_rl_cursor_move(const int16_t row, const int16_t col);

/**   utils.c   **/
void			ft_rl_updatetermsize(void);

/**  term  **/


/**  hist  **/

/**   history.c   **/
uint8_t			ft_rl_hist_get_next(rl_input_t *input);
uint8_t			ft_rl_hist_get_prev(rl_input_t *input);
uint8_t			ft_rl_hist_get_last(rl_input_t *input);
uint8_t			ft_rl_hist_get_first(rl_input_t *input);

/**   historyfile.c   **/
void			ft_rl_hist_load(const char *path);
void			ft_rl_hist_save(const char *path);

/**   search.c   **/
uint8_t			ft_rl_hist_search(rl_input_t *input, const uint8_t direction);
uint8_t			ft_rl_hist_isearch(rl_input_t *input, const uint8_t direction);

/**   utils.c   **/
rl_histnode_t	*ft_rl_hist_mknode(const char *line);
char			*ft_rl_hist_get_line(const __t_list *node);
void			ft_rl_hist_yank_arg(rl_input_t *input, const rl_histnode_t *node, int32_t n);
void			ft_rl_hist_recycle(void);
void			ft_rl_hist_restore(void);
void			ft_rl_hist_newnode(void);
void			ft_rl_hist_rmnode(void);

/**  hist  **/


/**  init  **/

/**   init.c   **/
void			ft_rl_init_input(const char *p, const uint64_t plen);

/**   initfuncs.c   **/
void			ft_rl_initfuncs(void);

/**  init  **/


/**  input  **/

/**   add.c   **/
void			ft_rl_addchar(rl_input_t *input, const uint8_t c);

/**   rm.c   **/
void			ft_rl_rmchar(rl_input_t *input);
void			ft_rl_rmchar_back(rl_input_t *input);

/**   kill.c   **/
void			ft_rl_kill_line(rl_input_t *input, const uint8_t direction);
void			ft_rl_kill_word(rl_input_t *input, const uint8_t direction);
void			ft_rl_kill_region(rl_input_t *input, const uint8_t direction);
/**   kill.c   **/

/**   utils.c   **/
void			ft_rl_redisplay(const rl_input_t *input, const rl_rdmode_t mode);
void			ft_rl_swap_words(rl_input_t *input);
void			ft_rl_unsetmark(uint8_t type);
void			ft_rl_setmark(uint8_t type);
void			ft_rl_word_start(void);
void			ft_rl_word_end(void);

/**  input  **/


/**  keys  **/

/**   utils.c   **/
rl_keytree_t	*ft_rl_getcurtree(void);
const char		*ft_rl_parse_escape(const char *esc);
const char		*ft_rl_parse_sequence(const char *seq);
void			ft_rl_bind_error(const uint8_t err, const char *context);

/**  keys  **/


/**  settings  **/

/**   utils.c   **/
uint64_t		ft_rl_get(const uint64_t var);

/**  settings  **/

#endif
