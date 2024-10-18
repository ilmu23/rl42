/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __misc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:34:52 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

#define BUFFER_SIZE 4096

static inline char	*_trimzeros(const char *n);
static inline char	*_bufcpy(char *buf, char **out);
static inline char	*_bufsft(char *dst, const char *src, const size_t n);

uint8_t	__isint(const char *n)
{
	int64_t	nbr;

	n = _trimzeros(n);
	if (!n || (int64_t)strlen(n) > 19 + (*n == '+' || *n == '-'))
		return (0);
	nbr = atol(n);
	if ((nbr == -1 && !__strequals(n, "-1"))
		|| (nbr == 0 && !__strequals(n, "0")))
		return (0);
	if (*n == '+' || *n == '-')
		n++;
	while (*n)
	{
		if (!isdigit(*n++))
			return (0);
	}
	return (1);
}

size_t	__uintlen_base(uint64_t n, const __t_base base)
{
	size_t	digits;
	uint8_t	div;

	digits = 1;
	switch (base)
	{
		case BINARY:
			div = 2;
			break ;
		case OCTAL:
			div = 8;
			break ;
		case DECIMAL:
			div = 10;
			break ;
		case HEX:
			div = 16;
			break ;
		default:
			div = 10;
	}
	while (n > (uint8_t)(div - 1))
	{
		n /= div;
		digits++;
	}
	return (digits);
}

size_t	__intlen_base(int64_t n, const __t_base base)
{
	size_t	digits;
	int8_t	div;

	digits = 1;
	if (n == INT64_MIN)
		n++;
	if (n < 0)
	{
		n = -n;
		digits++;
	}
	switch (base)
	{
		case BINARY:
			div = 2;
			break ;
		case OCTAL:
			div = 8;
			break ;
		case DECIMAL:
			div = 10;
			break ;
		case HEX:
			div = 16;
			break ;
		default:
			div = 10;
	}
	while (n > div - 1)
	{
		n /= div;
		digits++;
	}
	return (digits);
}

char	*__getline(const int32_t fd)
{
	ssize_t		rv;
	static char	buf[BUFFER_SIZE + 1];
	char		*out;

	out = NULL;
	rv = read(fd, NULL, 0);
	while (rv >= 0)
	{
		if (*buf && !_bufcpy(buf, &out))
			return (NULL);
		if (out && out[strlen(out) - 1] == '\n')
			return (out);
		rv = read(fd, buf, BUFFER_SIZE);
		if (rv < 1)
			break ;
	}
	return (out);
}

static inline char	*_trimzeros(const char *n)
{
	static char	out[128];
	size_t	i;

	if (!n)
		return (NULL);
	i = 0;
	if (*n == '+' || *n == '-')
		out[i++] = *n++;
	while (*n == '0' && n[1])
		n++;
	__strlcpy(&out[i], n, strlen(n) + 1);
	return (out);
}

static inline char	*_bufcpy(char *buf, char **out)
{
	size_t	llen;
	size_t	blen;
	char	*cpy;
	char	*tmp;

	blen = strlen(buf);
	llen = __strclen(buf, '\n');
	if (llen < blen)
		llen++;
	__push(*out);
	cpy = __calloc(llen + 1, sizeof(*cpy));
	__popblk(*out);
	__strlcpy(cpy, buf, llen + 1);
	_bufsft(buf, &buf[llen], blen - llen);
	tmp = (!*out) ? cpy : __strjoin(*out, cpy);
	if (!cpy)
		return (NULL);
	*out = tmp;
	return (buf);
}

static inline char	*_bufsft(char *dst, const char *src, const size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
	memset(dst + n, '\0', strlen(dst + n));
	if (*dst)
		return (dst);
	return (NULL);
}
