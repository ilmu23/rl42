/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_fnames.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 19:59:27 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/29 16:55:32 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_FNAMES_H
# define FT_RL_FNAMES_H

extern const char	*g_fn_names[];

# define self_insert g_fn_names[0]
# define remove_char g_fn_names[1]
# define backward_remove_char g_fn_names[2]
# define beginning_of_line g_fn_names[3]
# define end_of_line g_fn_names[4]
# define forward_char g_fn_names[5]
# define backward_char g_fn_names[6]
# define end_of_word g_fn_names[7]
# define beginning_of_word g_fn_names[8]
# define forward_word g_fn_names[9]
# define backward_word g_fn_names[10]
# define clear_screen g_fn_names[11]
# define clear_display g_fn_names[12]
# define forward_find_character g_fn_names[13]
# define backward_find_character g_fn_names[14]
# define accept_line g_fn_names[15]
# define end_of_file g_fn_names[16]
# define next_history g_fn_names[17]
# define previous_history g_fn_names[18]
# define end_of_history g_fn_names[19]
# define beginning_of_history g_fn_names[20]
# define forward_search_history g_fn_names[21]
# define reverse_search_history g_fn_names[22]
# define inc_forward_search_history g_fn_names[23]
# define inc_reverse_search_history g_fn_names[24]
# define yank_nth_arg g_fn_names[25]
# define yank_last_arg g_fn_names[26]
# define upcase_word g_fn_names[27]
# define downcase_word g_fn_names[28]
# define capitalize_word g_fn_names[29]
# define forward_kill_line g_fn_names[30]
# define backward_kill_line g_fn_names[31]
# define kill_whole_line g_fn_names[32]
# define forward_kill_word g_fn_names[33]
# define backward_kill_word g_fn_names[34]
# define kill_region g_fn_names[35]
# define delete_horizontal_space g_fn_names[36]
# define copy_region_as_kill g_fn_names[37]
# define copy_forward_word g_fn_names[38]
# define copy_backward_word g_fn_names[39]
# define yank g_fn_names[40]
# define yank_pop g_fn_names[41]
# define transpose_characters g_fn_names[42]
# define transpose_words g_fn_names[43]
# define complete g_fn_names[44]
# define re_read_init_file g_fn_names[45]
# define discard_line g_fn_names[46]
# define digit_argument g_fn_names[47]
# define negative_digit_argument g_fn_names[48]
# define rl_abort g_fn_names[49]
# define set_mark g_fn_names[50]
# define unset_mark g_fn_names[51]
# define exchange_point_and_mark g_fn_names[52]
# define emacs_editing_mode g_fn_names[53]
# define vi_editing_mode g_fn_names[54]
# define vi_command_mode g_fn_names[55]
# define vi_insert_mode_a g_fn_names[56]
# define vi_insert_mode_A g_fn_names[57]
# define vi_insert_mode_i g_fn_names[58]
# define vi_insert_mode_I g_fn_names[59]
# define vi_subst g_fn_names[60]
# define vi_subst_line g_fn_names[61]
# define vi_delete g_fn_names[62]
# define vi_replace g_fn_names[63]
# define set_highlight_color g_fn_names[64]
# define hlcolor_toggle_bold g_fn_names[65]
# define hlcolor_toggle_underline g_fn_names[66]
# define hlcolor_toggle_fg_bg g_fn_names[67]
# define hlcolor_set_sgr g_fn_names[68]
# define hlcolor_set_color g_fn_names[69]
# define hlcolor_set_rgb g_fn_names[70]
# define hlcolor_accept g_fn_names[71]
# define dump_functions g_fn_names[72]
# define dump_variables g_fn_names[73]
# define dump_macros g_fn_names[74]
# define operate_and_get_next g_fn_names[75]

#endif
