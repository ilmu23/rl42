/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:40:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/11/13 18:10:35 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

#define CUR_SHOW	((!(g_status & _HIDE_CURSOR)) ? ft_ti_tputs(g_escapes.cnorm, 1, ft_rl_putc) : 0)
#define CUR_HIDE	((!(g_status & _HIDE_CURSOR)) ? ft_ti_tputs(g_escapes.civis, 1, ft_rl_putc) : 0)

static inline const struct timespec	*_gettimeout(void);
static inline uint8_t				_interrupt(void);

rl_cmp_fn_t	ft_rl_get_completion_fn(void)
{
	ft_rl_init();
	return (g_cmp_fn);
}

rl_block_t	*ft_rl_newblock(const char *str, const int16_t pos[2])
{
	rl_block_t	*out;

	out = __push(__alloc(sizeof(*out)));
	if (!out)
		exit(ft_rl_perror());
	memcpy(out, &(rl_block_t){.str = str, .length = strlen(str),
			.pos[0] = pos[0], .pos[1] = pos[1], .highlighted = 0}, sizeof(*out));
	return (out);
}

uint16_t	ft_rl_geteditmode(void)
{
	return (g_status & _MD_MASK);
}

rl_fn_t	ft_rl_getinput(const char **seqstore)
{
	char			*seq;
	char			c[2] = "\0\0";
	int32_t			slctrv;
	ssize_t			readrv;
	rl_keytree_t	*tree;
	fd_set			exc;
	fd_set			in;

	CUR_SHOW;
	for (seq = NULL, tree = ft_rl_getcurtree(), readrv = read(0, c, 1); tree; readrv = read(0, c, 1))
	{
		if (readrv == -1)
		{
			if (_interrupt())
				continue ;
			exit(ft_rl_perror());
		}
		if (tree->fn && !tree->next[(uint8_t)*c])
			break ;
		tree = tree->next[(uint8_t)*c];
		__popblk(seq);
		seq = __push(__strjoin(seq, c));
		if (!tree || memcmp(tree->next, g_emptynode.next, 256 * sizeof(void *)) == 0)
			break ;
		if (tree->fn)
		{
			slct:
			FD_ZERO(&in);
			FD_ZERO(&exc);
			FD_SET(0, &in);
			FD_SET(0, &exc);
			slctrv = pselect(1, &in, NULL, &exc, _gettimeout(), NULL);
			if (slctrv == -1 && _interrupt())
				goto slct;
			if (slctrv == 0)
				break ;
		}
	}
	CUR_HIDE;
	if (tree && seqstore)
		*seqstore = seq;
	return (tree) ? tree->fn : NULL;
}

ssize_t	ft_rl_putc(const int8_t c)
{
	return (write(1, &c, sizeof(c)));
}

int32_t	ft_rl_getarg(void)
{
	g_argument.set = 0;
	return (g_argument.arg);
}

uint8_t	ft_rl_isdir(const char *path)
{
	struct stat	file;

	lstat(path, &file);
	if (S_ISLNK(file.st_mode) && ft_rl_get(_CMP_MLDIRS_HASH))
		stat(path, &file);
	return (S_ISDIR(file.st_mode));
}

void	ft_rl_set_completion_fn(rl_cmp_fn_t f)
{
	ft_rl_init();
	g_cmp_fn = f;
}

void	ft_rl_seteditmode(const uint16_t mode)
{
	uint16_t	emode;
	
	emode = ft_rl_geteditmode();
	if (mode != emode)
		g_status ^= emode | mode;
}

void	ft_rl_clearblocks(void)
{
	while (g_blocks)
		__lstrmnode(&g_blocks, g_blocks);
}

void	ft_rl_bell(void)
{
	uint64_t	type;

	type = ft_rl_get(_BSTYLE_HASH);
	switch (type)
	{
		case BELL_AUDIBLE:
			ft_ti_tputs(g_escapes.bel, 1, ft_rl_putc);
			break ;
		case BELL_VISIBLE:
			ft_ti_tputs(g_escapes.flash, 1, ft_rl_putc);
	}
}

static inline const struct timespec	*_gettimeout(void)
{
	static struct timespec	ts;

	if (g_settings.keyseq_timeout <= 0)
		return NULL;
	ts = (struct timespec){
		.tv_sec = g_settings.keyseq_timeout / 1000,
		.tv_nsec = (g_settings.keyseq_timeout % 1000) * 1000000};
	return &ts;
}

static inline uint8_t	_interrupt(void)
{
	if (errno != EINTR)
		return (0);
	errno = 0;
	return (1);
}
