/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_limits.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:56:54 by ivalimak          #+#    #+#             */
/*   Updated: 2024/03/13 23:05:19 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_LIMITS_H
# define LFT_LIMITS_H

// 8-bit linits
# ifndef INT8_MIN
#  define INT8_MIN -128
# endif
# ifndef INT8_MAX
#  define INT8_MAX 127
# endif
# ifndef UINT8_MAX
#  define UINT8_MAX 255
# endif

// 16-bit limist
# ifndef INT16_MIN
#  define INT16_MIN -32768
# endif
# ifndef INT16_MAX
#  define INT16_MAX 32767
# endif
# ifndef UINT16_MAX
#  define UINT16_MAX 65535
# endif

// 32-bit limits
# ifndef INT32_MIN
#  define INT32_MIN -2147483648
# endif
# ifndef INT32_MAX
#  define INT32_MAX 2147483647
# endif
# ifndef UINT32_MAX
#  define UINT32_MAX 4294967295
# endif

// 64-bit limits
# ifndef INT64_MIN
#  define INT64_MIN -9223372036854775808
# endif
# ifndef INT64_MAX
#  define INT64_MAX 9223372036854775807
# endif
# ifndef UINT64_MAX
#  define UINT64_MAX 18446744073709551615
# endif

// char limits
# ifndef CHAR_MIN 
#  define CHAR_MIN INT8_MIN
# endif
# ifndef CHAR_MAX 
#  define CHAR_MAX INT8_MAX
# endif
# ifndef UCHAR_MAX 
#  define UCHAR_MAX UINT8_MAX
# endif

// short limits
# ifndef SHORT_MIN 
#  define SHORT_MIN INT16_MIN
# endif
# ifndef SHORT_MAX 
#  define SHORT_MAX INT16_MAX
# endif
# ifndef USHORT_MAX 
#  define USHORT_MAX UINT16_MAX
# endif

// int limits
# ifndef INT_MIN 
#  define INT_MIN INT32_MIN
# endif
# ifndef INT_MAX 
#  define INT_MAX INT32_MAX
# endif
# ifndef UINT_MAX 
#  define UINT_MAX UINT32_MAX
# endif

// long limits
# ifndef LONG_MIN 
#  define LONG_MIN INT64_MIN
# endif
# ifndef LONG_MAX 
#  define LONG_MAX INT64_MAX
# endif
# ifndef ULONG_MAX 
#  define ULONG_MAX UINT64_MAX
# endif

// ascii range
# ifndef ASCII_MIN
#  define ASCII_MIN 0
# endif
# ifndef ASCII_MAX
#  define ASCII_MAX INT8_MAX
# endif

#endif
