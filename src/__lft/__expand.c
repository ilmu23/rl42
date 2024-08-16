/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __expand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 03:12:20 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/16 16:57:47 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "__lft.h"

static inline t_base	_getbase(t_format_type type);
static inline char		*_padstr(t_pf_conversion *cnv, char *str, size_t slen);
static inline void		_mvsign(char *s, uint8_t pc);
static inline void		_addstr(t_list **out, t_pf_conversion *cnv, char *str);
static inline void		_storelen(t_pf_conversion *cnv, t_list *out);

void	__expandconversions(t_list *conversions, t_list **strings)
{
	t_pf_conversion	*cnv;
	char			*str;

	str = NULL;
	while (conversions)
	{
		cnv = (t_pf_conversion *)conversions->blk;
		if (cnv->arg.type == d || cnv->arg.type == i)
			str = __expandint(cnv);
		else if (cnv->arg.type == o || cnv->arg.type == u
			|| cnv->arg.type == x || cnv->arg.type == X || cnv->arg.type == p)
			str = __expanduint(cnv);
		else if (cnv->arg.type == c)
			str = __expandchar(cnv);
		else if (cnv->arg.type == s)
			str = __expandstr(cnv);
		else if (cnv->arg.type == percent)
			str = __strdup("%");
		if (cnv->arg.type == n)
			_storelen(cnv, *strings);
		else
			_addstr(strings, cnv, str);
		conversions = conversions->next;
	}
}

char	*__expandint(t_pf_conversion *cnv)
{
	size_t	slen;
	char	*out;

	if (cnv->flags & PF_FLAG_PRECISION && !cnv->precision && !cnv->arg.intval)
		return (NULL);
	__cast(cnv);
	out = __push(__itoa_base(cnv->arg.intval, DECIMAL));
	if (!out)
		return (NULL);
	slen = strlen(out);
	if (cnv->flags & PF_FLAG_PRECISION && cnv->precision > slen - (*out == '-'))
		out = __strjoin(__cstr('0', cnv->precision - slen + (*out == '-')
					- (cnv->flags & (PF_FLAG_SPACE | PF_FLAG_SIGN)
						&& cnv->arg.intval >= 0)), out);
	if (strrchr(out, '-') && out && strrchr(out, '-') != out)
	{
		*strrchr(out, '-') = '0';
		*out = '-';
	}
	if (cnv->arg.intval >= 0 && cnv->flags & PF_FLAG_SIGN)
		out = __strjoin("+", out);
	else if (cnv->arg.intval >= 0 && cnv->flags & PF_FLAG_SPACE)
		out = __strjoin(" ", out);
	return (out);
}

char	*__expanduint(t_pf_conversion *cnv)
{
	size_t	slen;
	char	*out;

	if (cnv->flags & PF_FLAG_PRECISION && !cnv->precision && !cnv->arg.intval)
		return (NULL);
	__cast(cnv);
	out = __push(__utoa_base(cnv->arg.uintval, _getbase(cnv->arg.type)));
	if (!out)
		return (NULL);
	slen = strlen(out);
	if (cnv->flags & PF_FLAG_PRECISION && cnv->precision > slen)
		out = __strjoin(__cstr('0', cnv->precision - slen), out);
	if ((cnv->flags & PF_FLAG_ALT && cnv->arg.uintval != 0)
		|| cnv->arg.type == p)
	{
		if (cnv->arg.type == o && out && *out != '0')
			out = __strjoin("0", out);
		else if (cnv->arg.type == x || cnv->arg.type == X || cnv->arg.type == p)
			out = __strjoin("0X", out);
	}
	if (cnv->arg.type == x || cnv->arg.type == p)
		__strlower(out);
	return (out);
}

char	*__expandchar(t_pf_conversion *cnv)
{
	char	*out;

	out = __calloc(2, sizeof(*out));
	if (!out)
		return (NULL);
	*out = cnv->arg.charval;
	return (out);
}

char	*__expandstr(t_pf_conversion *cnv)
{
	size_t	slen;
	char	*out;

	if (!cnv->arg.ptrval && (!(cnv->flags & PF_FLAG_PRECISION)
			|| cnv->precision >= 6))
		return (__strdup("(null)"));
	slen = strlen((char *)cnv->arg.ptrval);
	if (slen && cnv->flags & PF_FLAG_PRECISION)
		slen = MIN(slen, cnv->precision);
	if (!slen)
		return (NULL);
	out = __push(__calloc(slen + 1, sizeof(*out)));
	if (!out)
		return (NULL);
	__strlcpy(out, (char *)cnv->arg.ptrval, slen + 1);
	return (out);
}

static inline t_base	_getbase(t_format_type type)
{
	if (type == o)
		return (OCTAL);
	else if (type == x || type == X || type == p)
		return (HEX);
	return (DECIMAL);
}

static inline char	*_padstr(t_pf_conversion *cnv, char *str, size_t slen)
{
	char	*out;
	uint8_t	pc;

	pc = ' ';
	__push(str);
	if ((cnv->arg.type == d || cnv->arg.type == i || cnv->arg.type == o
			|| cnv->arg.type == u || cnv->arg.type == x || cnv->arg.type == X)
		&& cnv->flags & PF_FLAG_ZERO)
		pc = '0';
	if (cnv->flags & PF_FLAG_LEFT)
		out = __strjoin(str, __cstr(pc, cnv->width - slen));
	else
		out = __strjoin(__cstr(pc, cnv->width - slen), str);
	if (slen == 0)
		out[(strlen(out) - 1) * (!*str && cnv->arg.type == c
				&& !(cnv->flags & PF_FLAG_LEFT))] = '\0';
	if (pc == '0')
		_mvsign(out, pc);
	return (out);
}

static inline void	_mvsign(char *s, uint8_t pc)
{
	if (strrchr(s, '-'))
		pc = '-';
	else if (strrchr(s, '+'))
		pc = '+';
	else if (strrchr(s, ' '))
		pc = ' ';
	if (pc != '0')
	{
		*strrchr(s, pc) = '0';
		*s = pc;
	}
}

static inline void	_addstr(t_list **out, t_pf_conversion *cnv, char *str)
{
	size_t	slen;

	if (cnv->arg.type == percent)
		cnv->flags = 0;
	slen = (str) ? strlen(str) : 0;
	if (cnv->flags & PF_FLAG_WIDTH && cnv->width > slen)
	{
		if (!str)
			str = __cstr(' ', cnv->width);
		else
			str = _padstr(cnv, str, slen);
	}
	__lstadd_back((const t_list **)out, __lstnew(str));
}

static inline void	_storelen(t_pf_conversion *cnv, t_list *out)
{
	*(ssize_t *)cnv->arg.ptrval = __getlen(out);
}

