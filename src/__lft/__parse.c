/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 23:20:25 by ivalimak          #+#    #+#             */
/*   Updated: 2024/10/18 12:25:05 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline __t_pf_conversion	*_newstring(const char *s);
static inline __t_pf_conversion	*_newconversion(const char *cnv, __t_list **args);
static inline __t_pf_arg			_gettype(const char *cnv, __t_list **args, size_t n);
static inline __t_format_type		_argtype(const uint8_t c);
static inline uint8_t			_getflags(const char **cnv);

__t_list	*__getconversions(const char *f, __t_list *args)
{
	const __t_list	*out;
	__t_pf_conversion	*conversion;

	out = NULL;
	while (*f)
	{
		if (*f != '%')
		{
			conversion = _newstring(__substr(f, 0, __strclen(f, '%')));
			f += __strclen(f, '%');
		}
		else
		{
			conversion = _newconversion(__getcnvstr(++f), &args);
			while (*f && !strchr(PF_FORMAT_SPEC, *f))
				f++;
			f++;
		}
		__lstadd_back(&out, __lstnew(conversion));
	}
	return ((__t_list *)out);
}

uint8_t	__getlength(const char **cnv)
{
	static const char	specs[7][3] = {"hh", "h", "ll", "l", "j", "z", "t"};
	size_t				i;

	i = 0;
	while (strncmp(*cnv, specs[i], strlen(specs[i])))
	{
		if (++i == 7)
			return (0);
	}
	(*cnv) += 1 + (i == 0 || i == 2);
	if (i == 0)
		return (PF_LENGTH_HHALF);
	if (i == 1)
		return (PF_LENGTH_HALF);
	if (i == 2)
		return (PF_LENGTH_LLONG);
	if (i == 3)
		return (PF_LENGTH_LONG);
	if (i == 4)
		return (PF_LENGTH_IMAX);
	if (i == 5)
		return (PF_LENGTH_SIZE);
	return (PF_LENGTH_DIFF);
}

size_t	__getwidth(const char **cnv, __t_list **args)
{
	size_t	n;

	if (isdigit(**cnv))
	{
		n = strtoul((*cnv)++, NULL, 10);
		while (isdigit(**cnv))
			(*cnv)++;
		return (n);
	}
	if (**cnv != '*')
		return (0);
	n = 0;
	if (isdigit(*++(*cnv)))
		n = strtoul(*cnv, NULL, 10);
	if (n)
	{
		*cnv += __uintlen_base(n, DECIMAL) + 1;
		return ((size_t)__getarg(*args, n));
	}
	return ((size_t)__nextarg(args));
}

size_t	__getprecision(const char **cnv, __t_list **args)
{
	size_t	n;

	if (isdigit(**cnv))
	{
		n = strtoul((*cnv)++, NULL, 10);
		while (isdigit(**cnv))
			(*cnv)++;
		return (n);
	}
	if (**cnv != '*')
		return (0);
	n = 0;
	if (isdigit(*++(*cnv)))
		n = strtoul(*cnv, NULL, 10);
	if (n)
	{
		*cnv += __uintlen_base(n, DECIMAL) + 1;
		return ((size_t)__getarg(*args, n));
	}
	return ((size_t)__nextarg(args));
}

uint8_t	__getflags(const char **cnv)
{
	const char	*tmp;
	uint8_t		flags;
	uint8_t		fspec;

	flags = _getflags(cnv);
	tmp = *cnv;
	if (isdigit(*tmp) || *tmp == '*')
	{
		flags |= PF_FLAG_WIDTH;
		while (isdigit(*tmp))
			tmp++;
		tmp += (*tmp == '*');
	}
	fspec = tmp[strlen(tmp) - 1];
	if (*tmp == '.')
	{
		if (strchr(PF_FORMAT_SPEC_INT, fspec))
			flags = (PF_FLAG_PRECISION | (flags & ~PF_FLAG_ZERO));
		else
			flags |= PF_FLAG_PRECISION;
	}
	return (flags);
}

static inline __t_pf_conversion	*_newstring(const char *str)
{
	__t_pf_conversion	*out;

	__push(str);
	out = __calloc(1, sizeof(*out));
	if (!out)
		return (NULL);
	*out = (__t_pf_conversion){.arg.type = s, .arg.ptrval = (uintptr_t)str,
		.flags = 0, .width = 0, .precision = 0, .length = 0};
	return (out);
}

static inline __t_pf_conversion	*_newconversion(const char *cnv, __t_list **args)
{
	__t_pf_conversion	*out;
	size_t			argn;

	__push(cnv);
	out = __calloc(1, sizeof(*out));
	if (!out)
		return (NULL);
	argn = 0;
	if (strchr(cnv, '$'))
	{
		argn = atol(cnv);
		cnv += __uintlen_base(argn, DECIMAL) + 1;
	}
	out->flags = __getflags(&cnv);
	if (isdigit(*cnv) || *cnv == '*')
		out->width = __getwidth(&cnv, args);
	if (*cnv == '.')
	{
		cnv++;
		out->precision = __getprecision(&cnv, args);
	}
	out->length = __getlength(&cnv);
	out->arg = _gettype(cnv, args, argn);
	return (out);
}

static inline __t_pf_arg	_gettype(const char *cnv, __t_list **args, size_t n)
{
	__t_pf_arg	arg;

	arg.type = _argtype(*cnv);
	if (arg.type != percent && !n)
		arg.ptrval = __nextarg(args);
	else if (arg.type != percent)
		arg.ptrval = __getarg(*args, n);
	return (arg);
}

static inline __t_format_type	_argtype(const uint8_t chr)
{
	if (chr == 'd' || chr == 'i')
		return ((chr == 'd') ? d : i);
	if (chr == 'o')
		return (o);
	if (chr == 'u')
		return (u);
	if (chr == 'x' || chr == 'X')
		return ((chr == 'x') ? x : X);
	if (chr == 'c')
		return (c);
	if (chr == 's')
		return (s);
	if (chr == 'p')
		return (p);
	if (chr == 'n')
		return (n);
	return (percent);
}

static inline uint8_t	_getflags(const char **cnv)
{
	const char	*tmp;
	uint8_t		flags;

	tmp = *cnv;
	flags = 0;
	while (strchr(PF_FLAG_CHARS, *tmp))
	{
		if (*tmp == '#')
			flags |= PF_FLAG_ALT;
		else if (*tmp == '0' && !(flags & PF_FLAG_LEFT))
			flags |= PF_FLAG_ZERO;
		else if (*tmp == '-')
			flags = (PF_FLAG_LEFT | (flags & ~PF_FLAG_ZERO));
		else if (*tmp == ' ' && !(flags & PF_FLAG_SIGN))
			flags |= PF_FLAG_SPACE;
		else if (*tmp == '+')
			flags = (PF_FLAG_SIGN | (flags & ~PF_FLAG_SPACE));
		else
			flags |= PF_FLAG_GROUP;
		tmp++;
	}
	*cnv = tmp;
	return (flags);
}
