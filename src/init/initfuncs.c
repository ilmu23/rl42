/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initfuncs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:59:29 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/02 05:42:35 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_initfuncs(void)
{
	ft_rl_addfunc("beginning-of-line", ft_rl_sol);
	ft_rl_addfunc("end-of-line", ft_rl_eol);
	ft_rl_addfunc("beginning-of-word", ft_rl_sow);
	ft_rl_addfunc("end-of-word", ft_rl_eow);
	ft_rl_addfunc("forward-char", ft_rl_fwd);
	ft_rl_addfunc("backward-char", ft_rl_bck);
	ft_rl_addfunc("forward-word", ft_rl_fwd_w);
	ft_rl_addfunc("backward-word", ft_rl_bck_w);
	ft_rl_addfunc("clear-screen", ft_rl_clr);
	ft_rl_addfunc("forward-find-character", ft_rl_ffc);
	ft_rl_addfunc("backward-find-character", ft_rl_bfc);
	ft_rl_addfunc("accept-line", ft_rl_acl);
	ft_rl_addfunc("previous-history", ft_rl_pvh);
	ft_rl_addfunc("next-history", ft_rl_nxh);
	ft_rl_addfunc("beginning-of-history", ft_rl_soh);
	ft_rl_addfunc("end-of-history", ft_rl_eoh);
	ft_rl_addfunc("reverse-search-history", ft_rl_rsh);
	ft_rl_addfunc("forward-search-history", ft_rl_fsh);
	ft_rl_addfunc("inc-reverse-search-history", ft_rl_rsh_i);
	ft_rl_addfunc("inc-forward-search-history", ft_rl_fsh_i);
	ft_rl_addfunc("yank-nth-arg", ft_rl_yna);
	ft_rl_addfunc("yank-last-arg", ft_rl_yla);
	ft_rl_addfunc("end-of-file", ft_rl_eof);
	ft_rl_addfunc("backward-remove-char", ft_rl_bdl);
	ft_rl_addfunc("remove-char", ft_rl_del);
	ft_rl_addfunc("self-insert", ft_rl_ins);
	ft_rl_addfunc("__paste", ft_rl_pst);
	ft_rl_addfunc("upcase-word", ft_rl_upw);
	ft_rl_addfunc("downcase-word", ft_rl_dnw);
	ft_rl_addfunc("capitalize-word", ft_rl_caw);
	ft_rl_addfunc("forward-kill-line", ft_rl_fkl);
	ft_rl_addfunc("backward-kill-line", ft_rl_bkl);
	ft_rl_addfunc("kill-whole-line", ft_rl_kln);
	ft_rl_addfunc("forward-kill-word", ft_rl_fkw);
	ft_rl_addfunc("backward-kill-word", ft_rl_bkw);
	ft_rl_addfunc("kill-region", ft_rl_krg);
	ft_rl_addfunc("delete-horizontal-space", ft_rl_kws);
	ft_rl_addfunc("copy-region-as-kill", ft_rl_crg);
	ft_rl_addfunc("copy-backward-word", ft_rl_cbw);
	ft_rl_addfunc("copy-forward-word", ft_rl_cfw);
	ft_rl_addfunc("yank", ft_rl_ynk);
	ft_rl_addfunc("yank-pop", ft_rl_ynp);
	ft_rl_addfunc("transpose-characters", ft_rl_tpc);
	ft_rl_addfunc("transpose-words", ft_rl_tpw);
	ft_rl_addfunc("complete", ft_rl_cmp);
	ft_rl_addfunc("discard-line", ft_rl_dcl);
	ft_rl_addfunc("digit-argument", ft_rl_arg);
	ft_rl_addfunc("negative-digit-argument", ft_rl_arg_n);
	ft_rl_addfunc("abort", ft_rl_abt);
	ft_rl_addfunc("set-mark", ft_rl_smk);
	ft_rl_addfunc("unset-mark", ft_rl_umk);
	ft_rl_addfunc("exchange-point-and-mark", ft_rl_xmk);
	ft_rl_addfunc("emacs-editing-mode", ft_rl_md_em);
	ft_rl_addfunc("vi-editing-mode", ft_rl_md_vi);
	ft_rl_addfunc("vi-command-mode", ft_rl_md_vc);
	ft_rl_addfunc("vi-insert-mode-i", ft_rl_md_vi);
	ft_rl_addfunc("vi-insert-mode-a", ft_rl_md_va);
	ft_rl_addfunc("vi-insert-mode-I", ft_rl_md_vI);
	ft_rl_addfunc("vi-insert-mode-A", ft_rl_md_vA);
	ft_rl_addfunc("vi-delete", ft_rl_vi_del);
	ft_rl_addfunc("vi-subst", ft_rl_vi_sub);
	ft_rl_addfunc("vi-subst-line", ft_rl_vi_sbl);
	ft_rl_addfunc("vi-replace", ft_rl_vi_rep);
	ft_rl_addfunc("set-highlight-color", ft_rl_hlc);
	ft_rl_addfunc("hlcolor-toggle-bold", ft_rl_hl_bld);
	ft_rl_addfunc("hlcolor-toggle-underline", ft_rl_hl_uln);
	ft_rl_addfunc("hlcolor-toggle-fg/bg", ft_rl_hl_fbg);
	ft_rl_addfunc("hlcolor-set-sgr", ft_rl_hl_sgr);
	ft_rl_addfunc("hlcolor-set-color", ft_rl_hl_clr);
	ft_rl_addfunc("hlcolor-set-rgb", ft_rl_hl_rgb);
	ft_rl_addfunc("hlcolor-accept", ft_rl_hl_acc);
}
