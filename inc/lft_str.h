/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_str.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:56:48 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/06 12:45:16 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_STR_H
# define LFT_STR_H
# include <stdarg.h>
# include <stdlib.h>
# include <stdint.h>
# include "lft_limits.h"

/*   alloc   */
char	**ft_split(const char *s, const uint8_t c);

char	*ft_substr(const char *s, uint64_t start, size_t len);
char	*ft_strtrim(const char *s, const char *set);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strsjoin(const char *s1, const char *s2, const uint8_t c);
char	*ft_strnjoin(size_t n, ...);
char	*ft_strsnjoin(const uint8_t sep, size_t n, ...);
char	*ft_strremove(const char *s, const uint8_t c);
char	*ft_strdup(const char *s);
/*   alloc   */

/*   no alloc   */
uint8_t	ft_strequals(const char *s1, const char *s2);

int8_t	ft_strncmp(const char *s1, const char *s2, size_t n);

size_t	ft_strlen(const char *s);
size_t	ft_strclen(const char *s, const uint8_t c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_wordcount(const char *s);

char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strchr(const char *s, const uint8_t c);
char	*ft_strrchr(const char *s, const uint8_t c);
char	*ft_strupper(char *s);
char	*ft_strlower(char *s);
char	*ft_strrev(char *s);
/*   no alloc   */

#endif
