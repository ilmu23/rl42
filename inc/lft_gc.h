/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_gc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:52:28 by ivalimak          #+#    #+#             */
/*   Updated: 2024/04/10 22:38:51 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LFT_GC_H
# define LFT_GC_H
# include <stdarg.h>
# include <stdlib.h>
# include <stdint.h>
# include "lft_data.h"
# include "lft_limits.h"

# define GCRESET "\e[0m"
# define GCGEN "\e[1mGC: "
# define GCCLEAN "\e[1;36mGC: clean: "
# define GCALLOC "\e[1;34mGC: alloc: "
# define GCOBJ "\e[1;34mGC: obj: "
# define GCSWEEP "\e[1;35mGC: sweep: "
# define GCPUSH "\e[1;32mGC: push: "
# define GCPOP "\e[1;31mGC: pop: "

# ifndef GC_START 
#  define GC_START 256 
# endif

/*   alloc   */
t_obj	*ft_newobj(size_t n);

void	*ft_alloc(size_t n);
void	*ft_calloc(size_t n, size_t size);
void	*ft_arrdup(const void *arr, const size_t size);
/*   alloc   */

/*   no alloc   */
/**   blk   **/
size_t	ft_getblksize(const void *blk);
/**   blk   **/

/**   obj   **/
t_obj	*ft_getobj(const void *blk);

void	ft_unmark(const void *blk);
void	ft_mark(const void *blk);
/**   obj   **/

/**   pop   **/
void	*ft_pop(void);

void	ft_popblk(const void *blk);
void	ft_popblks(size_t blks, ...);
void	ft_popn(size_t blks);
void	ft_popall(void);
/**   pop   **/

/**   push   **/
void	*ft_push(const void *blk);
void	*ft_pusharr(const void *blk);

void	ft_pushn(size_t blks, ...);
/**   push   **/

/**   stack   **/
void	ft_stackrm(t_stack *node);
void	ft_stackadd(const void *blk);
/**   stack   **/

/**   utils   **/
t_vm	*ft_getvm(void);

void	ft_sweep(t_vm *vm);
void	ft_clean(void);
void	ft_exit(int32_t estat);

int32_t	ft_return(int32_t rval);
/**   utils   **/
/*   no alloc   */

#endif
