/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initfuncs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:59:29 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 16:56:18 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

void	ft_rl_initfuncs(void)
{
	ft_rl_addfunc(beginning_of_line, ft_rl_sol);
	ft_rl_addfunc(end_of_line, ft_rl_eol);
	ft_rl_addfunc(beginning_of_word, ft_rl_sow);
	ft_rl_addfunc(end_of_word, ft_rl_eow);
	ft_rl_addfunc(forward_char, ft_rl_fwd);
	ft_rl_addfunc(backward_char, ft_rl_bck);
	ft_rl_addfunc(forward_word, ft_rl_fwd_w);
	ft_rl_addfunc(backward_word, ft_rl_bck_w);
	ft_rl_addfunc(clear_screen, ft_rl_clr);
	ft_rl_addfunc(clear_display, ft_rl_cld);
	ft_rl_addfunc(forward_find_character, ft_rl_ffc);
	ft_rl_addfunc(backward_find_character, ft_rl_bfc);
	ft_rl_addfunc(accept_line, ft_rl_acl);
	ft_rl_addfunc(operate_and_get_next, ft_rl_opr);
	ft_rl_addfunc(previous_history, ft_rl_pvh);
	ft_rl_addfunc(next_history, ft_rl_nxh);
	ft_rl_addfunc(beginning_of_history, ft_rl_soh);
	ft_rl_addfunc(end_of_history, ft_rl_eoh);
	ft_rl_addfunc(reverse_search_history, ft_rl_rsh);
	ft_rl_addfunc(forward_search_history, ft_rl_fsh);
	ft_rl_addfunc(inc_reverse_search_history, ft_rl_rsh_i);
	ft_rl_addfunc(inc_forward_search_history, ft_rl_fsh_i);
	ft_rl_addfunc(yank_nth_arg, ft_rl_yla);
	ft_rl_addfunc(yank_last_arg, ft_rl_yna);
	ft_rl_addfunc(end_of_file, ft_rl_eof);
	ft_rl_addfunc(backward_remove_char, ft_rl_bdl);
	ft_rl_addfunc(remove_char, ft_rl_del);
	ft_rl_addfunc(self_insert, ft_rl_ins);
	ft_rl_addfunc("__paste", ft_rl_pst);
	ft_rl_addfunc(upcase_word, ft_rl_upw);
	ft_rl_addfunc(downcase_word, ft_rl_dnw);
	ft_rl_addfunc(capitalize_word, ft_rl_caw);
	ft_rl_addfunc(forward_kill_line, ft_rl_fkl);
	ft_rl_addfunc(backward_kill_line, ft_rl_bkl);
	ft_rl_addfunc(kill_whole_line, ft_rl_kln);
	ft_rl_addfunc(forward_kill_word, ft_rl_fkw);
	ft_rl_addfunc(backward_kill_word, ft_rl_bkw);
	ft_rl_addfunc(kill_region, ft_rl_krg);
	ft_rl_addfunc(delete_horizontal_space, ft_rl_kws);
	ft_rl_addfunc(transpose_characters, ft_rl_tpc);
	ft_rl_addfunc(transpose_words, ft_rl_tpw);
	ft_rl_addfunc(copy_region_as_kill, ft_rl_crg);
	ft_rl_addfunc(copy_forward_word, ft_rl_cfw);
	ft_rl_addfunc(copy_backward_word, ft_rl_cbw);
	ft_rl_addfunc(yank, ft_rl_ynk);
	ft_rl_addfunc(yank_pop, ft_rl_ynp);
	ft_rl_addfunc(complete, ft_rl_cmp);
	ft_rl_addfunc(re_read_init_file, ft_rl_rri);
	ft_rl_addfunc(discard_line, ft_rl_dcl);
	ft_rl_addfunc(digit_argument, ft_rl_arg);
	ft_rl_addfunc(negative_digit_argument, ft_rl_arg_n);
	ft_rl_addfunc(rl_abort, ft_rl_abt);
	ft_rl_addfunc(set_mark, ft_rl_smk);
	ft_rl_addfunc(unset_mark, ft_rl_umk);
	ft_rl_addfunc(exchange_point_and_mark, ft_rl_xmk);
	ft_rl_addfunc(emacs_editing_mode, ft_rl_md_em);
	ft_rl_addfunc(vi_editing_mode, ft_rl_md_vi);
	ft_rl_addfunc(vi_command_mode, ft_rl_md_vc);
	ft_rl_addfunc(vi_insert_mode_i, ft_rl_md_vi);
	ft_rl_addfunc(vi_insert_mode_a, ft_rl_md_va);
	ft_rl_addfunc(vi_insert_mode_I, ft_rl_md_vI);
	ft_rl_addfunc(vi_insert_mode_A, ft_rl_md_vA);
	ft_rl_addfunc(vi_delete, ft_rl_vi_del);
	ft_rl_addfunc(vi_subst, ft_rl_vi_sub);
	ft_rl_addfunc(vi_subst_line, ft_rl_vi_sbl);
	ft_rl_addfunc(vi_replace, ft_rl_vi_rep);
	ft_rl_addfunc(set_highlight_color, ft_rl_hlc);
	ft_rl_addfunc(hlcolor_toggle_bold, ft_rl_hl_bld);
	ft_rl_addfunc(hlcolor_toggle_underline, ft_rl_hl_uln);
	ft_rl_addfunc(hlcolor_toggle_fg_bg, ft_rl_hl_fbg);
	ft_rl_addfunc(hlcolor_set_sgr, ft_rl_hl_sgr);
	ft_rl_addfunc(hlcolor_set_color, ft_rl_hl_clr);
	ft_rl_addfunc(hlcolor_set_rgb, ft_rl_hl_rgb);
	ft_rl_addfunc(hlcolor_accept, ft_rl_hl_acc);
	ft_rl_addfunc(dump_functions, ft_rl_dfn);
	ft_rl_addfunc(dump_variables, ft_rl_dvr);
	ft_rl_addfunc(dump_macros, ft_rl_dmc);
}
