/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 15:51:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/13 12:45:55 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_DATA_H
# define FT_RL_DATA_H
# include "libft.h"

typedef enum e_rl_etype
{
	ACL,
	EOF
}	t_rl_etype;

typedef enum e_rl_wtype
{
	SPACE,
	NORMAL
}	t_rl_wtype;

typedef enum e_rl_rdmode
{
	ALL,
	LINE,
	PROMPT
}	t_rl_rdmode;

typedef enum e_rl_mapmode
{
	WARN,
	QUIET,
	REMAP,
	QREMAP
}	t_rl_mapmode;

typedef enum e_rl_histmode
{
	ON,
	OFF,
	NOUPDATE
}	t_rl_histmode;

typedef struct s_rl_word
{
	size_t				i;
	size_t				len;
	char				*word;
	t_rl_wtype			wtype;
	struct s_rl_word	*next;
	struct s_rl_word	*prev;
}	t_rl_word;

typedef struct s_rl_cursor
{
	int16_t	t_rows;
	int16_t	t_cols;
	int16_t	i_row;
	int16_t	i_col;
	int16_t	row;
	int16_t	col;
}	t_rl_cursor;

typedef struct s_rl_input
{
	size_t		plen;
	size_t		maxlen;
	const char	*prompt;
	t_rl_etype	exittype;
	t_rl_cursor	*cursor;
	t_rl_word	*current;
	t_rl_word	*head;
	uint64_t	key;
}	t_rl_input;

typedef uint8_t	(*t_rl_fn)(t_rl_input *);

typedef struct s_rl_key
{
	const char		*name;
	const uint64_t	value;
}	t_rl_key;

typedef struct s_rl_func
{
	const char	*name;
	t_rl_fn		f;
}	t_rl_func;

typedef struct s_rl_map
{
	const uint64_t	key;
	t_rl_fn			f;
}	t_rl_map;

typedef struct s_rl_wc
{
	char	*pattern;
	t_list	*matches;
}	t_rl_wc;

#endif
