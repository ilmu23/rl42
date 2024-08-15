/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __str.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:57:52 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/16 01:22:07 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline size_t	_splitcount(const char *s, const uint8_t c);
static inline char		*_makesplit(const char *s, const uint8_t c);
static inline char		*_basemin(const t_base base);

uint64_t	__strclen(const char *s, const uint8_t c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

uint8_t	__strequals(const char *s1, const char *s2)
{
	size_t	len1;

	if (!s1 || !s2)
		return (0);
	len1 = strlen(s1);
	if (strncmp(s1, s2, len1))
		return (0);
	return ((len1 == strlen(s2)) ? 1 : 0);
}

size_t	__strlcpy(char *dst, const char *src, const size_t size)
{
	size_t	i;

	if (size == 0)
		return (strlen(src));
	i = 0;
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (strlen(src));
}

char	**__split(const char *s, const uint8_t c)
{
	char	**out;
	size_t	splits;
	size_t	i;

	if (!s)
		return (NULL);
	splits = _splitcount(s, c);
	__push(s);
	out = __push(__calloc((splits + 1), sizeof(*out)));
	__popblk(s);
	if (!out)
		return (NULL);
	i = 0;
	while (i <= splits)
	{
		out[i++] = _makesplit(s, c);
		if (!out[i - 1] && i <= splits)
			return (NULL);
	}
	while (i-- > 0)
		__popblk(out[i]);
	__popblk(out);
	return (out);
}

char	*__substr(const char *s, const size_t start, const size_t len)
{
	char	*out;
	size_t	slen;

	if (!s)
		return (NULL);
	if (start >= strlen(s))
		return (__strdup(""));
	slen = strlen(&s[start]);
	__push(s);
	out = __calloc(((len > slen) ? slen : len) + 1, sizeof(*out));
	__popblk(s);
	if (!out)
		return (NULL);
	__strlcpy(out, &s[start], ((len > slen) ? slen : len) + 1);
	return (out);
}

char	*__strsjoin(const char *s1, const char *s2, const uint8_t c)
{
	char	*out;
	size_t	len1;
	size_t	len2;

	if (!s2)
		return (NULL);
	len1 = (s1) ? strlen(s1) : 0;
	len2 = strlen(s2);
	__pushn(2, s1, s2);
	out = __calloc(len1 + len2 + (c != 0 && s1) + 1, sizeof(*out));
	__popblks((s1) ? 2 : 1, s2, s1);
	if (!out)
		return (NULL);
	if (s1)
		__strlcpy(out, s1, len1 + 1);
	if (c && s1)
		out[len1] = c;
	__strlcpy(&out[strlen(out)], s2, len2 + 1);
	return (out);
}

char	*__strjoin(const char *s1, const char *s2)
{
	return (__strsjoin(s1, s2, 0));
}

char	*__strtrim(const char *s, const char *set)
{
	size_t	i;
	size_t	j;

	if (!s || !set)
		return (NULL);
	if (!*s)
		return (__strdup(""));
	i = 0;
	j = strlen(s) - 1;
	while (strchr(set, s[i]))
		i++;
	while (j > i && strchr(set, s[j]))
		j--;
	return (__substr(s, i, j + 1 - i));
}

char	*__strnjoin(size_t n, ...)
{
	char	*out;
	va_list	args;

	out = NULL;
	va_start(args, n);
	while (n--)
	{
		out = __strjoin(out, va_arg(args, char *));
		if (!out)
			break ;
	}
	va_end(args);
	return (out);
}

char	*__strdup(const char *s)
{
	char	*out;
	size_t	asize;

	asize = strlen(s) + 1;
	__push(s);
	out = __calloc(asize, sizeof(*out));
	__popblk(s);
	if (!out)
		return (NULL);
	__strlcpy(out, s, asize);
	return (out);
}

char	*__utoa_base(uint64_t n, const t_base base)
{
	char	*out;
	uint8_t	div;
	size_t	i;

	i = __uintlen_base(n, base);
	out = __calloc(i-- + 1, sizeof(*out));
	if (!out)
		return (NULL);
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
		out[i--] = _HEXARR[n % div];
		n /= div;
	}
	out[i] =  _HEXARR[n];
	return (out);
}

char	*__itoa_base(int64_t n, const t_base base)
{
	char	*out;
	int8_t	div;
	uint8_t	neg;
	size_t	i;

	if (n == INT64_MIN)
		return (_basemin(base));
	i = __intlen_base(n, base);
	out = __calloc(i-- + 1, sizeof(*out));
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
	if (!out)
		return (NULL);
	neg = n < 0;
	if (neg)
		n = -n;
	while (n > div - 1)
	{
		out[i--] = _HEXARR[n % div];
		n /= div;
	}
	out[i--] = _HEXARR[n];
	if (neg)
		out[i] = '-';
	return (out);
}

char	*__strnstr(const char *s1, const char *s2, const size_t n)
{
	size_t	i;
	size_t	j;

	if (!s1)
		return (NULL);
	if (!*s2)
		return ((char *)s1);
	i = 0;
	while (s1[i])
	{
		j = 0;
		while (s1[i + j] == s2[j] && i + j < n)
		{
			if (!s1[i + j])
				break ;
			j++;
		}
		if (!s2[j])
			return ((char *)s1 + i);
		i++;
	}
	return (NULL);
}

char	*__strlower(char *s)
{
	size_t	i;

	if (s)
	{
		i = 0;
		while (s[i])
		{
			s[i] = tolower(s[i]);
			i++;
		}
	}
	return (s);
}

static inline size_t	_splitcount(const char *s, const uint8_t c)
{
	size_t	splits;
	size_t	i;

	if (!c)
		return ((!*s) ? 0 : 1);
	i = 0;
	splits = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i - 1] != c)
			splits++;
	}
	return (splits);
}

static inline char	*_makesplit(const char *s, const uint8_t c)
{
	static size_t	i = 0;
	size_t			j;

	while (s[i] && s[i] == c)
		i++;
	j = i;
	while (s[i] && s[i] != c)
		i++;
	if (j == i)
	{
		i = 0;
		return (NULL);
	}
	return (__push(__substr(s, j, i - j)));
}

static inline char	*_basemin(const t_base base)
{
	switch (base)
	{
		case BINARY:
			return (__strjoin("-1", __cstr('0', 63)));
		case OCTAL:
			return (__strdup("-1000000000000000000000"));
		case DECIMAL:
			return (__strdup("-9223372036854775808"));
		case HEX:
			return (__strdup("-8000000000000000"));
	}
	return (NULL);
}
