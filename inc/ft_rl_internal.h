/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:34:49 by ivalimak          #+#    #+#             */
/*   Updated: 2024/06/12 01:54:37 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_INTERNAL_H
# define FT_RL_INTERNAL_H
# include "rl42.h"
# include "ft_rl_fns.h"
# include "ft_rl_term.h"
# include "ft_rl_globals.h"

// error.c
int32_t			ft_rl_perror(void);

// exec.c
uint8_t			ft_rl_execmap(rl_input_t *input);

// color.c
char			*ft_rl_hlcolor(void);

// utils.c
rl_block_t		*ft_rl_newblock(const char *str, const int16_t pos[2]);
uint64_t		ft_rl_getkey(void);
uint8_t			ft_rl_isdir(const char *path);
void			ft_rl_clearblocks(void);

/**  comp  **/

/**   completion.c   **/
void			ft_rl_complete(rl_input_t *input);

/**   utils.c   **/
uint64_t		ft_rl_comp_getlongest(const t_list *completions);
void			ft_rl_comp_display(rl_input_t *input, const t_list *completions, const void *cur, const void *prv);

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
char			*ft_rl_hist_get_line(const t_list *node);
void			ft_rl_hist_recycle(void);
void			ft_rl_hist_restore(void);
void			ft_rl_hist_newnode(void);
void			ft_rl_hist_rmnode(void);

/**  hist  **/


/**  init  **/

/**   init.c   **/
void			ft_rl_init_input(const char *p, const uint64_t plen);

/**   initkeys.c   **/
void			ft_rl_initkeys(void);

/**   initfuncs.c   **/
void			ft_rl_initfuncs(void);

/**  init  **/


/**  input  **/

/**   add.c   **/
void			ft_rl_addchar(rl_input_t *input, const uint8_t c);

/**   rm.c   **/
void			ft_rl_rmchar(rl_input_t *input);
void			ft_rl_rmchar_back(rl_input_t *input);

/**   utils.c   **/
void			ft_rl_redisplay(const rl_input_t *input, const rl_rdmode_t mode);
void			ft_rl_unsetmark(uint8_t type);
void			ft_rl_setmark(uint8_t type);
void			ft_rl_word_start(void);
void			ft_rl_word_end(void);

/**  input  **/


/**  keys  **/

/**   utils.c   **/
uint8_t			ft_rl_iskey(const uint64_t key);
char			*ft_rl_keystr(const uint64_t key);

/**  keys  **/

#endif
