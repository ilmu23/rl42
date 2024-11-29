/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_fns.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 01:33:06 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 16:54:38 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_FUNCS_H
# define FT_RL_FUNCS_H
# include "rl42.h"

/**  fns  **/

/**   move.c   **/

/* beginning-of-line */
uint8_t	ft_rl_sol(rl_input_t *input);
/* end-of-line */
uint8_t	ft_rl_eol(rl_input_t *input);
/* beginning-of-word */
uint8_t	ft_rl_sow(rl_input_t *input);
/* end-of-word */
uint8_t	ft_rl_eow(rl_input_t *input);
/* forward-char */
uint8_t	ft_rl_fwd(rl_input_t *input);
/* backward-char */
uint8_t	ft_rl_bck(rl_input_t *input);
/* forward-word */
uint8_t	ft_rl_fwd_w(rl_input_t *input);
/* backward-word */
uint8_t	ft_rl_bck_w(rl_input_t *input);
/* clear-screen */
uint8_t	ft_rl_clr(rl_input_t *input);
/* clear-display */
uint8_t	ft_rl_cld(rl_input_t *input);
/* forward-find-character */
uint8_t	ft_rl_ffc(rl_input_t *input);
/* backward-find-character */
uint8_t	ft_rl_bfc(rl_input_t *input);

/**   hist.c   **/

/* accept-line */
uint8_t	ft_rl_acl(rl_input_t *input);
/* operate-and-get-next */
uint8_t	ft_rl_opr(rl_input_t *input);
/* previous-history */
uint8_t	ft_rl_pvh(rl_input_t *input);
/* next-history */
uint8_t	ft_rl_nxh(rl_input_t *input);
/* beginning-of-history */
uint8_t	ft_rl_soh(rl_input_t *input);
/* end-of-history */
uint8_t	ft_rl_eoh(rl_input_t *input);
/* reverse-search-history */
uint8_t	ft_rl_rsh(rl_input_t *input);
/* forward-search-history */
uint8_t	ft_rl_fsh(rl_input_t *input);
/* inc-reverse-search-history */
uint8_t	ft_rl_rsh_i(rl_input_t *input);
/* inc-forward-search-history */
uint8_t	ft_rl_fsh_i(rl_input_t *input);
/* yank-nth-arg */
uint8_t	ft_rl_yna(rl_input_t *input);
/* yank-last-arg */
uint8_t	ft_rl_yla(rl_input_t *input);

/**   text.c   **/

/* end-of-file */
uint8_t	ft_rl_eof(rl_input_t *input);
/* backward-remove-char */
uint8_t	ft_rl_del(rl_input_t *input);
/* remove-char */
uint8_t	ft_rl_bdl(rl_input_t *input);
/* self-insert */
uint8_t	ft_rl_ins(rl_input_t *input);
/* __paste */
uint8_t	ft_rl_pst(rl_input_t *input);
/* upcase-word */
uint8_t	ft_rl_upw(rl_input_t *input);
/* downcase-word */
uint8_t	ft_rl_dnw(rl_input_t *input);
/* capitalize-word */
uint8_t	ft_rl_caw(rl_input_t *input);
/* forward-kill-line */
uint8_t	ft_rl_fkl(rl_input_t *input);
/* backward-kill-line */
uint8_t	ft_rl_bkl(rl_input_t *input);
/* kill-whole-line */
uint8_t	ft_rl_kln(rl_input_t *input);
/* forward-kill-word */
uint8_t	ft_rl_fkw(rl_input_t *input);
/* backward-kill-word */
uint8_t	ft_rl_bkw(rl_input_t *input);
/* kill-region */
uint8_t	ft_rl_krg(rl_input_t *input);
/* delete-horizontal-space */
uint8_t	ft_rl_kws(rl_input_t *input);
/* transpose-characters */
uint8_t	ft_rl_tpc(rl_input_t *input);
/* transpose-words */
uint8_t	ft_rl_tpw(rl_input_t *input);
/* copy-region-as-kill */
uint8_t	ft_rl_crg(rl_input_t *input);
/* copy-forward-word */
uint8_t	ft_rl_cfw(rl_input_t *input);
/* copy-backward-word */
uint8_t	ft_rl_cbw(rl_input_t *input);
/* yank */
uint8_t	ft_rl_ynk(rl_input_t *input);
/* yank-pop */
uint8_t	ft_rl_ynp(rl_input_t *input);

/**   comp.c   **/

/* complete */
uint8_t	ft_rl_cmp(rl_input_t *input);

/**   misc.c   **/

/* __macro */
uint8_t	ft_rl_mcr(rl_input_t *input);
/* re-read-init-file */
uint8_t	ft_rl_rri(rl_input_t *input);
/* discard-line */
uint8_t	ft_rl_dcl(rl_input_t *input);
/* digit-argument */
uint8_t	ft_rl_arg(rl_input_t *input);
/* negative-digit-argument */
uint8_t	ft_rl_arg_n(rl_input_t *input);
/* abort */
uint8_t	ft_rl_abt(rl_input_t *input);
/* set-mark */
uint8_t	ft_rl_smk(rl_input_t *input);
/* unset-mark */
uint8_t	ft_rl_umk(rl_input_t *input);
/* exchange-point-and-mark */
uint8_t	ft_rl_xmk(rl_input_t *input);
/* dump-functions */
uint8_t	ft_rl_dfn(rl_input_t *input);
/* dump-variables */
uint8_t	ft_rl_dvr(rl_input_t *input);
/* dump-macros */
uint8_t	ft_rl_dmc(rl_input_t *input);
/* emacs-editing-mode */
uint8_t	ft_rl_md_em(rl_input_t *input);
/* vi-command-mode */
uint8_t	ft_rl_md_vc(rl_input_t *input);
/* vi-insert-mode-i */
uint8_t	ft_rl_md_vi(rl_input_t *input);
/* vi-insert-mode-a */
uint8_t	ft_rl_md_va(rl_input_t *input);
/* vi-insert-mode-I */
uint8_t	ft_rl_md_vI(rl_input_t *input);
/* vi-insert-mode-A */
uint8_t	ft_rl_md_vA(rl_input_t *input);

/**   vi.c   **/

/* vi-delete */
uint8_t	ft_rl_vi_del(rl_input_t *input);
/* vi-subst */
uint8_t	ft_rl_vi_sub(rl_input_t *input);
/* vi-subst-line */
uint8_t	ft_rl_vi_sbl(rl_input_t *input);
/* vi-replace */
uint8_t	ft_rl_vi_rep(rl_input_t *input);

/**   hlcolor.c   **/

/* set-highlight-color */
uint8_t	ft_rl_hlc(rl_input_t *input);
/* hlcolor-toggle-bold */
uint8_t	ft_rl_hl_bld(rl_input_t *input);
/* hlcolor-toggle-underline */
uint8_t	ft_rl_hl_uln(rl_input_t *input);
/* hlcolor-toggle-fg/bg */
uint8_t	ft_rl_hl_fbg(rl_input_t *input);
/* hlcolor-set-sgr */
uint8_t	ft_rl_hl_sgr(rl_input_t *input);
/* hlcolor-set-color */
uint8_t	ft_rl_hl_clr(rl_input_t *input);
/* hlcolor-set-rgb */
uint8_t	ft_rl_hl_rgb(rl_input_t *input);
/* hlcolor-accept */
uint8_t	ft_rl_hl_acc(rl_input_t *input);

/**  fns  **/

#endif
