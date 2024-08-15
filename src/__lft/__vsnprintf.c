/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __vsnprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 22:34:58 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 19:12:39 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline size_t	_argcount(const char *f);
static inline size_t	_getdmax(const char *f);
static inline t_list	*_getargs(va_list args, size_t argc);
static inline void		_join(char *str, size_t size, t_list *conversions);

ssize_t	__vsnprintf(char *s, size_t size, const char *f, va_list args)
{
	t_list	*arglist;
	t_list	*conversions;
	t_list	*strings;
	uint8_t	ptrap_st;

	ptrap_st = __pushtrap_status();
	__pushtrap(PTRAP_ENABLE);
	__push(s);
	arglist = _getargs(args, _argcount(f));
	conversions = __getconversions(f, arglist);
	strings = NULL;
	__expandconversions(conversions, &strings);
	if (s)
		_join(s, size, strings);
	__pushtrap(ptrap_st);
	return (__getlen(strings));
}

static inline size_t	_argcount(const char *f)
{
	const char	*tmp;
	size_t		argc;
	char		*conversion;

	tmp = f;
	argc = 0;
	while (*tmp)
	{
		if (*tmp++ == '%')
		{
			conversion = __getcnvstr(tmp);
			if (strchr(conversion, '$'))
				return (_getdmax(f));
			while (*conversion)
			{
				if (*conversion == '*')
					argc++;
				else if (strchr(PF_FORMAT_SPEC_EXP, *conversion))
					argc++;
				conversion++;
				tmp++;
			}
		}
	}
	return (argc);
}

static inline size_t	_getdmax(const char *f)
{
	size_t	dmax;
	size_t	dcur;
	char	*conversion;

	dmax = 0;
	while (*f)
	{
		if (*f++ == '%')
		{
			conversion = __getcnvstr(f);
			while (*conversion)
			{
				if (isdigit(*conversion))
				{
					dcur = atol(conversion);
					conversion += __uintlen_base(dcur, DECIMAL);
					if (*conversion == '$' && dcur >= dmax)
						dmax = dcur;
				}
				conversion++;
				f++;
			}
		}
	}
	return (dmax);
}

static inline t_list	*_getargs(va_list args, size_t argc)
{
	const t_list	*out;
	uintptr_t		*val;

	out = NULL;
	while (argc--)
	{
		val = __calloc(1, sizeof(*val));
		if (!val)
			return (NULL);
		*val = va_arg(args, uintptr_t);
		__lstadd_back(&out, __lstnew(val));
	}
	return ((t_list *)out);
}

static inline void	_join(char *str, size_t size, t_list *strings)
{
	memset(str, 0, size);
	while (strings && size)
	{
		if (strings->blk)
		{
			memcpy(str, strings->blk,
				MIN(__getblksize(strings->blk), size));
			str += MIN(__getblksize(strings->blk), size) - 1;
			size -= MIN(__getblksize(strings->blk), size) - 1;
		}
		strings = strings->next;
	}
}
