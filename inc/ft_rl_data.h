/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 15:51:46 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/30 20:11:52 by ivalimak         ###   ########.fr       */
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
typedef struct s_rl_keytree		rl_keytree_t;
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

typedef uint8_t		(*rl_fn_t)(rl_input_t *);
typedef __t_list	*(*rl_cmp_fn_t)(const char *, const rl_input_t *);

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
	int64_t	keyseq_timeout;
};

struct s_rl_histnode
{
	const char	*line;
	const char	*edit;
};

struct s_rl_keytree
{
	void	*next[256];
	rl_fn_t	fn;
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
	const char	*kf1;	/* key sequence for f1 */
	const char	*kf2;	/* key sequence for f2 */
	const char	*kf3;	/* key sequence for f3 */
	const char	*kf4;	/* key sequence for f4 */
	const char	*kf5;	/* key sequence for f5 */
	const char	*kf6;	/* key sequence for f6 */
	const char	*kf7;	/* key sequence for f7 */
	const char	*kf8;	/* key sequence for f8 */
	const char	*kf9;	/* key sequence for f9 */
	const char	*kf10;	/* key sequence for f10 */
	const char	*kf11;	/* key sequence for f11 */
	const char	*kf12;	/* key sequence for f12 */
	const char	*kcuu1;	/* key sequence for up-arrow */
	const char	*kcud1; /* key sequence for down-arrow */
	const char	*kcub1; /* key sequence for left-arrow */
	const char	*kcuf1; /* key sequence for right-arrow */
	const char	*kLFT;	/* key sequence for shift + left-arrow */
	const char	*kRIT;	/* key sequence for shift + right-arrow */
	const char	*ht;	/* key sequence for tab */
	const char	*kent;	/* key sequnece for enter */
	const char	*cr;	/* key sequence for return */
	const char	*kich1;	/* key sequence for insert */
	const char	*khome;	/* key sequence for home */
	const char	*knp;	/* key sequence for pgup */
	const char	*kdch1;	/* key sequence for delete */
	const char	*kend;	/* key sequence for end */
	const char	*kpp;	/* key sequence for pgdn */
	const char	*kHOM;	/* key sequence for shift + home */
	const char	*kDC;	/* key sequence for shift + del */
	const char	*kEND;	/* key sequence for shift + end */
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
	const char		*keyseq;
	const char		*prompt;
	const char		*sprompt;
	rl_etype_t		exittype;
	rl_cursor_t		*cursor;
	const uint64_t	plen;
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
