/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:34:49 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 12:49:44 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_INTERNAL_H
# define FT_RL_INTERNAL_H
# include "ft_readline.h"

// ft_rl_input.c
uint8_t		ft_rl_getinput(t_rl_input *input);
void		ft_rl_addchar(t_rl_input *input);
void		ft_rl_rmchar(t_rl_input *input);

// ft_rl_color.c
char		*ft_rl_gethlcolor(void);

// ft_rl_word.c
t_rl_word	*ft_rl_splitword(t_rl_word *w1);
t_rl_word	*ft_rl_joinword(t_rl_word *w1, t_rl_word *w2);
void		ft_rl_insertword(t_rl_input *input, t_rl_word *newword);
void		ft_rl_addword(t_rl_input *input, uint8_t c);
void		ft_rl_rmword(t_rl_input *input, t_rl_word *dword, uint64_t key);

// ft_rl_word_replace.c
void		ft_rl_initreplace(t_rl_word *start, t_rl_word *end);
void		ft_rl_replaceword(t_rl_input *input, t_rl_word *w);
void		ft_rl_endreplace(void);

// ft_rl_cursor.c
void		ft_rl_shiftcursor(size_t n, uint64_t direction);

// ft_rl_history.c
t_rl_input	*ft_rl_hist_getnext(t_rl_input *input, uint8_t cpy);
t_rl_input	*ft_rl_hist_getprev(t_rl_input *input, uint8_t cpy);
void		ft_rl_hist_commit(t_rl_input *input);
void		ft_rl_hist_add(t_list **hist, t_rl_input *input);

// ft_rl_history_search.c
uint8_t		ft_rl_hist_search(t_rl_input *input, uint64_t direction);

// ft_rl_complete.c
uint8_t		ft_rl_complete(t_rl_input *input);

// ft_rl_complete2.c
t_list		*ft_rl_complete_wc(const char *pattern);
t_list		*ft_rl_complete_env(const char *pattern);
t_list		*ft_rl_complete_file(const char *pattern);

// ft_rl_complete3.c
void		ft_rl_complete_inc(t_list *completions, const char *pattern);

// ft_rl_fn.c
uint8_t		ft_rl_sol(t_rl_input *input);
uint8_t		ft_rl_eol(t_rl_input *input);
uint8_t		ft_rl_fwd(t_rl_input *input);
uint8_t		ft_rl_bck(t_rl_input *input);
uint8_t		ft_rl_clr(t_rl_input *input);

// ft_rl_fn2.c
uint8_t		ft_rl_fwd_w(t_rl_input *input);
uint8_t		ft_rl_bck_w(t_rl_input *input);
uint8_t		ft_rl_acl(t_rl_input *input);
uint8_t		ft_rl_eof(t_rl_input *input);
uint8_t		ft_rl_pvh(t_rl_input *input);

// ft_rl_fn3.c
uint8_t		ft_rl_nxh(t_rl_input *input);
uint8_t		ft_rl_soh(t_rl_input *input);
uint8_t		ft_rl_eoh(t_rl_input *input);
uint8_t		ft_rl_rsh(t_rl_input *input);
uint8_t		ft_rl_fsh(t_rl_input *input);

// ft_rl_fn4.c
uint8_t		ft_rl_upw(t_rl_input *input);
uint8_t		ft_rl_dnw(t_rl_input *input);
uint8_t		ft_rl_caw(t_rl_input *input);
uint8_t		ft_rl_cmp(t_rl_input *input);
uint8_t		ft_rl_mta(t_rl_input *input);

// ft_rl_fn5.c
uint8_t		ft_rl_hlc(t_rl_input *input);
uint8_t		ft_rl_dcl(t_rl_input *input);
uint8_t		ft_rl_ins(t_rl_input *input);
uint8_t		ft_rl_del(t_rl_input *input);

// ft_rl_exec.c
uint8_t		ft_rl_execmap(t_rl_input *input, uint64_t key);

// ft_rl_initkeys.c
t_rl_key	*ft_rl_newkey(const char *name, const uint64_t value);
void		ft_rl_initkeys(t_list **keys);

// ft_rl_initfuncs.c
t_rl_func	*ft_rl_newfunc(const char *name, t_rl_fn f);
void		ft_rl_initfuncs(t_list **funcs);

// ft_rl_keymap_lists.c
t_list		**ft_rl_getkeys(void);
t_list		**ft_rl_getfuncs(void);
t_list		**ft_rl_getkeymaps(void);

// ft_rl_utils.c
t_rl_input	*ft_rl_dupinput(t_rl_input *input);
t_rl_word	*ft_rl_dupwords(t_rl_word *words);
char		*ft_rl_inputstr(t_rl_input *input, uint8_t pop);
void		ft_rl_redisplay(t_rl_input *input, t_rl_rdmode mode);
void		ft_rl_popwords(t_rl_word *words);

// ft_rl_utils2.c
t_rl_input	*ft_rl_strinput(const char *s);
t_rl_word	*ft_rl_strwords(const char *s);
t_rl_word	*ft_rl_strword(const char *s);
uint16_t	ft_rl_isdir(const char *path);

// ft_rl_utils3.c
size_t		ft_rl_getinputlen(t_rl_input *input);
size_t		ft_rl_input_getmaxlen(void);

// ft_rl_keymap_utils.c
void		ft_rl_map_err(const char *k, const char *fn, void *ks, void *fns);

// ft_rl_input_utils.c
void		ft_rl_updateinput(t_rl_input *input, t_rl_input *newinput);
void		ft_rl_setcurinput(const t_rl_input *input);

// ft_rl_history_utils.c
t_list		**ft_rl_hist_gethead(void);
t_list		**ft_rl_hist_getcurrent(void);
t_list		*ft_rl_hist_duphist(t_list *hist);
void		ft_rl_hist_setcurrent(t_list *node);
void		ft_rl_hist_pop(t_list *hist);

// ft_rl_complete_utils.c
size_t		ft_rl_complete_getlongest(t_list *completions);
char		*ft_rl_complete_basename(const char *s);
void		ft_rl_putcompletion(t_list *st, t_list *c, const char *s, size_t m);

// ft_rl_term_utils.c
t_rl_cursor	*ft_rl_getcursor(t_rl_input *input);
void		ft_rl_inputcursor(t_rl_input *input);
void		ft_rl_resetcursor(t_rl_input *input);
void		ft_rl_updatecursor(t_rl_cursor *cursor);

// ft_rl_wildcard_utils.c
uint8_t		ft_rl_wc_checkalloc(t_rl_wc *cur, t_rl_wc *prv, char **patterns);
void		ft_rl_wc_rmdot(t_list *matches);

// ft_rl_debug_utils.c
uint8_t		ft_rl_dbg_printinput(t_rl_input *input);
void		ft_rl_dbg_info(t_rl_input *input, uint64_t key);

#endif
