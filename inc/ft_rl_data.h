/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 15:51:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/05/26 01:56:37 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_DATA_H
# define FT_RL_DATA_H
# include "libft.h"

typedef enum e_rl_etype
{
	ACL,
	EOF
}	rl_etype_t;

typedef enum e_rl_wtype
{
	SPACE,
	NORMAL
}	rl_wtype_t;

typedef enum e_rl_rdmode
{
	ALL,
	LINE,
	PROMPT
}	rl_rdmode_t;

typedef enum e_rl_mapmode
{
	WARN,
	QUIET,
	REMAP,
	QREMAP
}	rl_mapmode_t;

typedef struct s_rl_word	rl_word_t;

struct s_rl_word
{
	size_t		i;
	size_t		len;
	char		*word;
	rl_wtype_t	wtype;
	rl_word_t	*next;
	rl_word_t	*prev;
};

typedef struct s_rl_cursor
{
	int16_t	t_rows;
	int16_t	t_cols;
	int16_t	i_row;
	int16_t	i_col;
	int16_t	row;
	int16_t	col;
}	rl_cursor_t;

typedef struct s_rl_input
{
	size_t		plen;
	size_t		maxlen;
	const char	*prompt;
	const char	*keystr;
	rl_etype_t	exittype;
	rl_cursor_t	*cursor;
	rl_word_t	*current;
	rl_word_t	*head;
	uint64_t	key;
}	rl_input_t;

typedef uint8_t	(*rl_fn_t)(rl_input_t *);

typedef struct s_rl_map
{
	const uint64_t	key;
	const rl_fn_t	f;
}	rl_map_t;

typedef struct s_rl_wc
{
	char	*pattern;
	t_list	*matches;
}	rl_wc_t;

#endif
