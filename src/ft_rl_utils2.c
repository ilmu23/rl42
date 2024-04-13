/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rl_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:24:22 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/11 18:50:20 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_rl_internal.h"

static inline void	insertword(t_rl_word **head, t_rl_word *word);

t_rl_input	*ft_rl_strinput(const char *s)
{
	t_rl_input	*out;

	out = ft_push(ft_alloc(sizeof(*out)));
	if (!out)
		return (NULL);
	*out = (t_rl_input){.plen = 0, .prompt = NULL, .cursor = NULL,
		.exittype = ACL, .head = ft_rl_strwords(s), .current = NULL};
	out->current = out->head;
	while (out->current && out->current->next)
		out->current = out->current->next;
	return (out);
}

t_rl_word	*ft_rl_strwords(const char *s)
{
	t_rl_word	*out;
	size_t		start;
	size_t		end;

	start = 0;
	out = NULL;
	while (s[start])
	{
		end = start;
		while (s[end] && !ft_isspace(s[end]))
			end++;
		insertword(&out, ft_rl_strword(ft_substr(s, start, end - start)));
		start = end;
		while (s[end] && ft_isspace(s[end]))
			end++;
		insertword(&out, ft_rl_strword(ft_substr(s, start, end - start)));
		start += (s[start] != '\0');
	}
	return (out);
}

t_rl_word	*ft_rl_strword(const char *s)
{
	t_rl_word	*out;

	if (!s || !*s)
		return (NULL);
	ft_push((void *)s);
	out = ft_push(ft_alloc(sizeof(*out)));
	if (!out)
		return (NULL);
	*out = (t_rl_word){.len = ft_strlen(s), .word = (char *)s, .wtype = NORMAL,
		.next = NULL, .prev = NULL};
	out->i = out->len;
	if (*s == ' ')
		out->wtype = SPACE;
	return (out);
}

uint16_t	ft_rl_isdir(const char *path)
{
	struct stat	file;

	stat(path, &file);
	return (file.st_mode & S_IFDIR);
}

static inline void	insertword(t_rl_word **head, t_rl_word *word)
{
	t_rl_word	*w;

	if (!word)
		return ;
	if (!*head)
		*head = word;
	else
	{
		w = *head;
		while (w->next)
			w = w->next;
		w->next = word;
		word->prev = w;
	}
}
