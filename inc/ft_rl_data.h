/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 15:51:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 04:16:00 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_DATA_H
# define FT_RL_DATA_H
# include <stdint.h>

typedef enum e_rl_etype			rl_etype_t;
typedef enum e_rl_wtype			rl_wtype_t;
typedef enum e_rl_rdmode		rl_rdmode_t;
typedef enum e_rl_mapmode		rl_mapmode_t;


typedef struct s_rl_settings	rl_settings_t;
typedef struct s_rl_histnode	rl_histnode_t;
typedef struct s_rl_cursor		rl_cursor_t;
typedef struct s_rl_keybuf		rl_keybuf_t;
typedef struct s_rl_input		rl_input_t;
typedef struct s_rl_block		rl_block_t;
typedef struct s_rl_mark		rl_mark_t;
typedef struct s_rl_map			rl_map_t;
typedef struct s_rl_rgb			rl_rgb_t;
typedef struct s_rl_hlc			rl_hlc_t;
typedef struct s_rl_arg			rl_arg_t;

typedef uint8_t	(*rl_fn_t)(rl_input_t *);
typedef void	(*rl_cmp_fn_t)(rl_input_t *);

enum e_rl_etype
{
	E_ACL,
	E_EOF
};

enum e_rl_wtype
{
	SPACE,
	NORMAL
};

enum e_rl_rdmode
{
	LINE,
	INPUT,
	PROMPT,
	SPROMPT,
	CLEAR
};

enum e_rl_mapmode
{
	WARN,
	QUIET,
	REMAP,
	QREMAP
};

struct s_rl_settings
{
	uint8_t	bstyle;
	uint8_t	completion;
	int16_t	cmp_dwidth;
	int64_t	cmp_qitems;
	int64_t	hist_size;
};

struct s_rl_histnode
{
	const char	*line;
	const char	*edit;
};

struct s_rl_cursor
{
	int16_t	p_row;
	int16_t	p_col;
	int16_t	i_row;
	int16_t	i_col;
	int16_t	row;
	int16_t	col;
};

struct s_rl_keybuf
{
	uint64_t	key;
	int8_t		size;
};

struct s_rl_input
{
	char			*line;
	const char		*prompt;
	const char		*keystr;
	const char		*sprompt;
	rl_etype_t		exittype;
	rl_cursor_t		*cursor;
	const uint64_t	plen;
	uint64_t		key;
	uint64_t		len;
	uint64_t		i;
};

struct s_rl_block
{
	const char		*str;
	const uint64_t	length;
	const int16_t	pos[2];
	uint8_t			highlighted;
};

struct s_rl_mark
{
	uint8_t		set;
	uint64_t	pos;
};

struct s_rl_map
{
	const uint64_t	key;
	const rl_fn_t	f;
};

struct s_rl_rgb
{
	const uint8_t	r;
	const uint8_t	g;
	const uint8_t	b;
};

struct s_rl_hlc
{
	const char	*sgr;
	rl_rgb_t	rgbval;
	uint8_t		mode;
};

struct s_rl_arg
{
	uint8_t	set;
	int32_t	arg;
};

#endif
