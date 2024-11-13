/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 02:14:13 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/13 17:44:21 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define __KILLFN_COUNT 9

static inline uint8_t	_iskill(const rl_fn_t f);
static inline uint8_t	_repeat(rl_input_t *input);
static inline void		_insertstr(rl_input_t *input, const char *s);
static inline void		_copy(const rl_input_t *input, const uint64_t start, const uint64_t end, const uint8_t direction);

uint8_t	ft_rl_eof(rl_input_t *input)
{
	if (input->len != 0)
		return (1);
	__printf("%s%s", TERM_CLEAR_END, TERM_CRNL);
	input->exittype = E_EOF;
	return (0);
}

uint8_t	ft_rl_del(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->len == 0 || (input->i == input->len && count > 0))
		return (2);
	while (count > 0)
	{
		ft_rl_rmchar_back(input);
		count--;
	}
	while (count < 0)
	{
		if (ft_rl_bdl(input) == 2)
			break ;
		count++;
	}
	if (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len && input->len)
		input->i--;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_bdl(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (input->len == 0 || (input->i == 0 && count > 0))
		return (2);
	while (count > 0)
	{
		ft_rl_rmchar(input);
		count--;
	}
	while (count < 0)
	{
		if (ft_rl_del(input) == 2)
			break ;
		count++;
	}
	if (ft_rl_geteditmode() == _MD_VI_CMD && input->i == input->len && input->len)
		input->i--;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_ins(rl_input_t *input)
{
	int32_t	count;

	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	while (count-- > 0)
		ft_rl_addchar(input, *input->keyseq);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_pst(rl_input_t *input)
{
	size_t				i;
	ssize_t				rv;
	char				*paste;
	char				buf[7] = "\0\0\0\0\0\0\0";
	static const char	*end = "\e[201~";

	for (i = 0, paste = NULL, rv = read(0, &buf[i], 1); rv == 1; rv = read(0, &buf[i], 1)) {
		if (buf[i] == end[i]) {
			if (++i == 6)
				break ;
		} else {
			__popblk(paste);
			paste = __push(__strjoin(paste, buf));
			memset(buf, 0, i + 1);
			i = 0;
		}
	}
	_insertstr(input, paste);
	return 1;
}

uint8_t	ft_rl_upw(rl_input_t *input)
{
	int32_t		count;
	uint64_t	i;
	uint64_t	j;

	if (input->len == 0)
		return (1);
	i = 1;
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count < 0)
		i = input->i;
	while (count != 0)
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		j = g_mark_s.pos;
		while (j <= g_mark_e.pos)
		{
			input->line[j] = toupper(input->line[j]);
			j++;
		}
		ft_rl_unsetmark(_MARK_START | _MARK_END);
		if (count > 0)
		{
			if (--count > 0 && ft_rl_fwd_w(input) == 2)
				break ;
		}
		else if (count < 0)
		{
			ft_rl_word_start();
			if (++count < 0 && ft_rl_bck_w(input) == 2)
			{
				input->i = i;
				break ;
			}
			if (count == 0)
				input->i = i;
		}
	}
	input->i = MIN(input->i + 1, input->len);
	ft_rl_word_end();
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_dnw(rl_input_t *input)
{
	int32_t		count;
	uint64_t	i;
	uint64_t	j;

	if (input->len == 0)
		return (1);
	i = 1;
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count < 0)
		i = input->i;
	while (count != 0)
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		j = g_mark_s.pos;
		while (j <= g_mark_e.pos)
		{
			input->line[j] = tolower(input->line[j]);
			j++;
		}
		ft_rl_unsetmark(_MARK_START | _MARK_END);
		if (count > 0)
		{
			if (--count > 0 && ft_rl_fwd_w(input) == 2)
				break ;
		}
		else if (count < 0)
		{
			ft_rl_word_start();
			if (++count < 0 && ft_rl_bck_w(input) == 2)
			{
				input->i = i;
				break ;
			}
			if (count == 0)
				input->i = i;
		}
	}
	input->i = MIN(input->i + 1, input->len);
	ft_rl_word_end();
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_caw(rl_input_t *input)
{
	int32_t		count;
	uint64_t	i;
	uint64_t	j;

	if (input->len == 0)
		return (1);
	i = 1;
	count = 1;
	if (g_argument.set)
		count = ft_rl_getarg();
	if (count < 0)
		i = input->i;
	while (count != 0)
	{
		ft_rl_word_start();
		ft_rl_setmark(_MARK_START);
		ft_rl_word_end();
		ft_rl_setmark(_MARK_END);
		j = g_mark_s.pos;
		input->line[j] =  toupper(input->line[j]);
		while (++j <= g_mark_e.pos)
			input->line[j] = tolower(input->line[j]);
		ft_rl_unsetmark(_MARK_START | _MARK_END);
		if (count > 0)
		{
			if (--count > 0 && ft_rl_fwd_w(input) == 2)
				break ;
		}
		else if (count < 0)
		{
			ft_rl_word_start();
			if (++count < 0 && ft_rl_bck_w(input) == 2)
			{
				input->i = i;
				break ;
			}
			if (count == 0)
				input->i = i;
		}
	}
	input->i = MIN(input->i + 1, input->len);
	ft_rl_word_end();
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_fkl(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	ft_rl_kill_line(input, _KILL_FWD);
	ft_rl_redisplay(input, INPUT);
	return (_repeat(input));
}

uint8_t	ft_rl_bkl(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	ft_rl_kill_line(input, _KILL_BCK);
	ft_rl_redisplay(input, INPUT);
	return (_repeat(input));
}

uint8_t	ft_rl_kln(rl_input_t *input)
{
	if (input->len == 0)
		return (1);
	ft_rl_kill_line(input, _KILL_FWD | _KILL_BCK);
	ft_rl_redisplay(input, INPUT);
	return (_repeat(input));
}

uint8_t	ft_rl_fkw(rl_input_t *input)
{
	if (input->i == input->len)
		return (1);
	ft_rl_kill_word(input, _KILL_FWD);
	ft_rl_redisplay(input, INPUT);
	return (_repeat(input));
}

uint8_t	ft_rl_bkw(rl_input_t *input)
{
	if (input->i == 0)
		return (1);
	ft_rl_kill_word(input, _KILL_BCK);
	ft_rl_redisplay(input, INPUT);
	return (_repeat(input));
}

uint8_t	ft_rl_krg(rl_input_t *input)
{
	uint64_t	i;

	if (!g_mark_u.set)
		return (1);
	i = input->i;
	input->i = MIN(i + 1, g_mark_u.pos + 1);
	ft_rl_setmark(_MARK_START);
	input->i = MAX(i, g_mark_u.pos);
	ft_rl_setmark(_MARK_END);
	ft_rl_kill_region(input, _KILL_REG);
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	input->i = MIN(input->i, input->len);
	ft_rl_redisplay(input, INPUT);
	return (_repeat(input));
}

uint8_t	ft_rl_kws(rl_input_t *input)
{
	if (!isspace(input->line[input->i])
		&& (input->i == 0 || !isspace(input->line[input->i - 1])))
		return (1);
	if (!isspace(input->line[input->i]))
		input->i--;
	ft_rl_setmark(_MARK_START | _MARK_END);
	while (input->i > 0 && isspace(input->line[input->i]))
		input->i--;
	if (!isspace(input->line[input->i]))
		input->i++;
	ft_rl_setmark(_MARK_START);
	input->i = g_mark_e.pos;
	while (input->i < input->len && isspace(input->line[input->i]))
		input->i++;
	ft_rl_setmark(_MARK_END);
	ft_rl_kill_region(input, _KILL_REG);
	input->i = g_mark_s.pos;
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_tpc(rl_input_t *input)
{
	uint8_t	c;

	if (input->len < 2)
		return (1);
	if (input->i == 0)
		input->i++;
	else if (input->i == input->len)
		input->i--;
	c = input->line[input->i - 1];
	input->line[input->i - 1] = input->line[input->i];
	input->line[input->i++] = c;
	if (input->i == input->len && ft_rl_geteditmode() == _MD_VI_CMD)
		input->i--;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_tpw(rl_input_t *input)
{
	if (input->len < 3 || !strchr(input->line, ' ')
		|| input->i < __strclen(input->line, ' '))
		return (1);
	ft_rl_swap_words(input);
	if (input->i == input->len && ft_rl_geteditmode() == _MD_VI_CMD)
		input->i--;
	ft_rl_redisplay(input, INPUT);
	return (1);
}

uint8_t	ft_rl_crg(rl_input_t *input)
{
	uint64_t	start;
	uint64_t	end;

	if (!g_mark_u.set)
		return (1);
	start = (g_mark_u.pos > input->i) ? input->i : g_mark_u.pos;
	end = (g_mark_u.pos > input->i) ? g_mark_u.pos : input->i;
	_copy(input, start, end, _KILL_REG);
	return (_repeat(input));
}

uint8_t ft_rl_cbw(rl_input_t *input)
{
	uint64_t	i;

	if (input->len == 0)
		return (1);
	i = input->i;
	g_argument.set = 0;
	ft_rl_bck_w(input);
	ft_rl_setmark(_MARK_START);
	ft_rl_word_end();
	if (input->i < input->len)
		input->i++;
	ft_rl_setmark(_MARK_END);
	_copy(input, g_mark_s.pos, g_mark_e.pos, _KILL_BCK);
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	input->i = i;
	ft_rl_redisplay(input, LINE);
	return (_repeat(input));
}

uint8_t	ft_rl_cfw(rl_input_t *input)
{
	uint64_t	i;

	if (input->len == 0)
		return (1);
	i = input->i;
	g_argument.set = 0;
	if (isspace(input->line[input->i]))
		ft_rl_fwd_w(input);
	else
		ft_rl_word_start();
	ft_rl_setmark(_MARK_START);
	ft_rl_word_end();
	if (input->i < input->len)
		input->i++;
	ft_rl_setmark(_MARK_END);
	_copy(input, g_mark_s.pos, g_mark_e.pos, _KILL_FWD);
	ft_rl_unsetmark(_MARK_START | _MARK_END);
	input->i = i;
	ft_rl_redisplay(input, LINE);
	return (_repeat(input));
}

uint8_t	ft_rl_ynk(rl_input_t *input)
{
	int32_t		count;
	size_t		slen;
	const char	*s;
	rl_fn_t		f;

	s = ft_rl_kring_yank();
	if (!s)
		return (1);
	count = (g_argument.set) ? ft_rl_getarg() : 1;
	slen = strlen(s);
	while (count-- > 0)
	{
		__popblk(input->line);
		if (input->i == 0)
			input->line = __push(__strjoin(s, input->line));
		else if (input->i == input->len)
			input->line = __push(__strjoin(input->line, s));
		else
			input->line = __push(__strnjoin(3, __substr(input->line, 0, input->i), s, __substr(input->line, input->i, input->len)));
		input->len += slen;
		input->i += slen;
	}
	ft_rl_redisplay(input, INPUT);
	f = ft_rl_getinput(&input->keyseq);
	if (f == ft_rl_ynp)
	{
		ft_rl_setmark(_MARK_END);
		input->i -= slen;
		ft_rl_setmark(_MARK_START);
	}
	return (f(input));
}

uint8_t	ft_rl_ynp(rl_input_t *input)
{
	const char	*subs[2];
	const char	*s;
	rl_fn_t		f;

	ft_rl_kring_rotate();
	s = ft_rl_kring_yank();
	if (!s)
		return (1);
	subs[0] = __push(__substr(input->line, 0, g_mark_s.pos));
	subs[1] = __push(__substr(input->line, g_mark_e.pos, input->len));
	__popblk(input->line);
	input->line = __push(__strnjoin(3, subs[0], s, subs[1]));
	__popblks(2, subs[0], subs[1]);
	g_mark_e.pos = g_mark_s.pos + strlen(s);
	input->len = strlen(input->line);
	input->i = g_mark_e.pos;
	ft_rl_redisplay(input, INPUT);
	f = ft_rl_getinput(&input->keyseq);
	if (f != ft_rl_ynp)
		ft_rl_unsetmark(_MARK_START | _MARK_END);
	return (f(input));
}

static inline uint8_t	_iskill(const rl_fn_t f)
{
	size_t			i;
	static rl_fn_t	fns[__KILLFN_COUNT] = {ft_rl_fkl, ft_rl_bkl, ft_rl_kln,
		ft_rl_fkw, ft_rl_bkw, ft_rl_krg, ft_rl_crg, ft_rl_cbw, ft_rl_cfw};

	i = 0;
	while (i < __KILLFN_COUNT && fns[i] != f)
		i++;
	return (i < __KILLFN_COUNT);
}

static inline uint8_t	_repeat(rl_input_t *input)
{
	rl_fn_t	f;
	uint8_t	rv;

	f = ft_rl_getinput(&input->keyseq);
	if (_iskill(f))
	{
		if (!(g_status & _KILL_APPEND))
			g_status ^= _KILL_APPEND;
		rv = f(input);
		if (g_status & _KILL_APPEND)
			g_status ^= _KILL_APPEND;
		return (rv);
	}
	return (f(input));
}

static inline void	_insertstr(rl_input_t *input, const char *s)
{
	size_t		plen;
	const char	*subs[2];

	subs[0] = (input->i > 0) ? __push(__substr(input->line, 0, input->i)) : "";
	subs[1] = (input->i < input->len) ? __push(__substr(input->line, input->i, input->len - input->i)) : "";
	__popblk(input->line);
	input->line = __push(__strnjoin(3, subs[0], s, subs[1]));
	if (!input->line)
		exit(ft_rl_perror());
	__popblks(2, subs[0], subs[1]);
	plen = strlen(s);
	input->len += plen;
	input->i += plen;
	ft_rl_redisplay(input, INPUT);
}

static inline void	_copy(const rl_input_t *input, const uint64_t start, const uint64_t end, const uint8_t direction)
{
	const char	*content;
	const char	*s;

	s = __substr(input->line, start, end - start);
	if (g_status & _KILL_APPEND)
	{
		content = __strdup(g_kill_ring->blk);
		__lstrmnode(&g_kill_ring, g_kill_ring);
		if (direction == _KILL_FWD)
			__lstadd_front(&g_kill_ring, __lstnew(__strjoin(content, s)));
		else
			__lstadd_front(&g_kill_ring, __lstnew(__strjoin(s, content)));
	}
	else
		__lstadd_front(&g_kill_ring, __lstnew(s));
}
