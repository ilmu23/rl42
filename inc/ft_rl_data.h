/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 15:51:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:02:27 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RL_DATA_H
# define FT_RL_DATA_H
# include "__lft.h"
# include <stdint.h>

typedef enum e_rl_etype			rl_etype_t;
typedef enum e_rl_rdmode		rl_rdmode_t;
typedef enum e_rl_mapmode		rl_mapmode_t;


typedef struct s_rl_settings	rl_settings_t;
typedef struct s_rl_histnode	rl_histnode_t;
typedef struct s_rl_escapes		rl_escapes_t;
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
typedef t_list	*(*rl_cmp_fn_t)(const char *, const rl_input_t *);

enum e_rl_etype
{
	E_ACL,
	E_EOF
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

struct s_rl_escapes
{
	const char	*cup;	/* move cursor to row #1 and col #2 */
	const char	*bel;	/* ring the bell */
	const char	*flash;	/* flash the screen / visual bell */
	const char	*civis;	/* hide cursor */
	const char	*cnorm; /* show cursor */
	const char	*el1;	/* clear to start of line */
	const char	*el;	/* clear to end of line */
	const char	*ed1;	/* clear to start of screen */
	const char	*ed;	/* clear to end of screen */
	const char	*indn;	/* scroll up #1 lines */
	const char	*rin;	/* scroll down #1 lines */
	const char	*sgr0;	/* turn off all attributes */
	const char	*bold;	/* turn on bold mode */
	const char	*smul;	/* turn on underline mode */
	const char	*rmul;	/* tun off undersline mode */
	const char	*setaf;	/* set foreground color to #1 */
	const char	*setab;	/* set background color to #1 */
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
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
};

struct s_rl_hlc
{
	const char	*seq;
	const char	*sgr;
	rl_rgb_t	rgb;
	uint8_t		clr;
	uint8_t		mode;
};

struct s_rl_arg
{
	uint8_t	set;
	int32_t	arg;
};

#endif
