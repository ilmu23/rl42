/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __lft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 00:37:03 by ivalimak          #+#    #+#             */
/*   Updated: 2024/08/15 19:05:09 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_H
# define __UTILS_H
# include <math.h>
# include <ctype.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/param.h>

# define E_ALLOC "\e[1;31mrl42: fatal: memory allocation failed\e[m"

# define _HEXARR "0123456789ABCDEF"

# define _GC_START 256
# define _GC_MAPSIZE 4096

# define PTRAP_ENABLE 0x1U
# define PTRAP_DISABLE 0x2U
# define PTRAP_FLUSH 0xCU
# define PTRAP_CLEAN 0x4U
# define PTRAP_POP 0x8U

# define HMAP_SALT_1 347ULL
# define HMAP_SALT_2 2903ULL
# define HMAP_SALT_3 4973ULL

# define HMAP_DEF_SIZE 23ULL

# define PF_FLAG_ALT 0x1U
# define PF_FLAG_ZERO 0x2U
# define PF_FLAG_LEFT 0x4U
# define PF_FLAG_SPACE 0x8U
# define PF_FLAG_SIGN 0x10U
# define PF_FLAG_GROUP 0x20U
# define PF_FLAG_WIDTH 0x40U
# define PF_FLAG_PRECISION 0x80U

# define PF_LENGTH_HHALF 0x1U
# define PF_LENGTH_HALF 0x2U
# define PF_LENGTH_LONG 0x4U
# define PF_LENGTH_LLONG 0x8U
# define PF_LENGTH_IMAX 0x10U
# define PF_LENGTH_SIZE 0x20U
# define PF_LENGTH_DIFF 0x40U

# define PF_FLAG_CHARS "#0- +'"
# define PF_FORMAT_SPEC "diouxXcspn%"
# define PF_FORMAT_SPEC_EXP "diouxXcspn"
# define PF_FORMAT_SPEC_INT "diouxX"

typedef enum e_base				t_base;
typedef enum e_format_type		t_format_type;

typedef struct s_hmap_pair		t_hmap_pair;
typedef struct s_hmap			t_hmap;
typedef struct s_list			t_list;

typedef struct s_pf_arg			t_pf_arg;
typedef struct s_pf_conversion	t_pf_conversion;

typedef struct s_objpair		t_objpair;
typedef struct s_objmap			t_objmap;
typedef struct s_obj			t_obj;
typedef struct s_vm				t_vm;

enum e_base
{
	BINARY,
	OCTAL,
	DECIMAL,
	HEX
};

enum e_format_type
{
	d,
	i,
	o,
	u,
	x,
	X,
	c,
	s,
	p,
	n,
	percent
};

struct s_hmap_pair
{
	const char	*key;
	void		*value;
};

struct s_hmap
{
	size_t		size;
	size_t		bsize;
	size_t		count;
	t_hmap_pair	**items;
};

struct s_list
{
	const void	*blk;
	size_t		*size;
	t_list		*next;
	t_list		*prev;
};

struct s_pf_arg
{
	t_format_type	type;
	union
	{
		int64_t		intval;
		uint64_t	uintval;
		uint8_t		charval;
		uintptr_t	ptrval;
	};
};

struct s_pf_conversion
{
	t_pf_arg	arg;
	uint8_t		flags;
	size_t		width;
	size_t		precision;
	uint8_t		length;
};

struct s_objpair
{
	const char	*key;
	const t_obj	*obj;
};

struct s_objmap
{
	size_t		size;
	size_t		bsize;
	uint64_t	count;
	t_list		**objs;
};

struct s_obj
{
	size_t		marks;
	size_t		traps;
	size_t		blksize;
	size_t		asize;
	const void	*blk;
	t_obj		*next;
	t_obj		*nfree;
	t_obj		*pfree;
};

struct s_vm
{
	t_obj		*head;
	t_obj		*free;
	uint8_t		ptrap;
	t_objmap	objmap;
	uint64_t	maxobjs;
	uint64_t	objcount;
};


extern const uint8_t	__hmap_sentinel;

/**   gc   **/
t_objpair	*__newpair(const char *key, const t_obj *obj);

size_t		__getblksize(const void *blk);

uint8_t		__pushtrap_status(void);

int32_t		__return(int32_t rval);

t_obj		*__newobj(const size_t n);

t_obj		*__objmap_get(const char *key);
t_obj		*__getobj(const void *blk);

t_vm		*__getvm(void);

char		*__blkkey(const void *blk);

void		*__alloc(const size_t n);
void		*__calloc(const size_t n, const size_t size);
void		*__gc_calloc(const size_t n, const size_t size);

void		*__push(const void *blk);
void		*__pusharr(const void *blk);

void		__pushn(size_t blks, ...);
void		__pushtrap(const uint8_t op);

void		__popall(void);
void		__popn(size_t blks);
void		__popblk(const void *blk);
void		__popblks(size_t blks, ...);

void		__unmark(const void *blk);
void		__mark(const void *blk);

void		__objmap_rm(const char *key);
void		__objmap_add(const char *key, const t_obj *obj);
void		__objmap_addpair(t_list **lst, t_objpair *pair);

void		__clean(void);
void		__sweep(void);

void		__exit(const int32_t estat);
/**   gc   **/

/**   hmap   **/
uint64_t	__nextprime(uint64_t n);
uint64_t	__gethash(const char *s, const size_t size, const size_t attempt);
uint64_t	__strhash(const char *s, const uint64_t salt, const size_t size);

uint8_t		__mapadd(t_hmap *hmap, const char *key, const void *val);
uint8_t		__maprm(t_hmap *hmap, const char *key);

uint8_t		__resizemap(t_hmap *hmap, const size_t bsize);
uint8_t		__shrinkmap(t_hmap *hmap);
uint8_t		__growmap(t_hmap *hmap);

t_hmap		*__mapnew(void);
t_hmap		*__mapnew_size(const size_t size);

void		*__mapget(t_hmap *hmap, const char *key);

void		__mappop(t_hmap_pair *pair);
void		__mappop_all(t_hmap *hmap);
/**   hmap   **/

/**   list   **/
t_list		*__lstnew(const void *blk);

t_list		*__lstpop(const t_list *node);
t_list		*__lstpush(const t_list *node);
t_list		*__lstlast(const t_list *list);
t_list		*__lstfirst(const t_list *list);

void		__lstpopall(const t_list *list);
void		__lstpushall(const t_list *list);
void		__lstadd_back(const t_list **list, t_list *node);
void		__lstadd_front(const t_list **list, t_list *node);
void		__lstrmnode(const t_list **list, const t_list *node);
/**   list   **/

/**   printf   **/
ssize_t		__printf(const char *f, ...);
ssize_t		__dprintf(const int32_t fd, const char *f, ...);
ssize_t		__sprintf(char *s, const char *f, ...);
ssize_t		__snprintf(char *s, const size_t size, const char *f, ...);
ssize_t		__vprintf(const char *f, va_list args);
ssize_t		__vdprintf(const int32_t fd, const char *f, va_list args);
ssize_t		__vsprintf(char *s, const char *f, va_list args);
ssize_t		__vsnprintf(char *s, const size_t size, const char *f, va_list args);

uintptr_t	__nextarg(t_list **args);
uintptr_t	__getarg(const t_list *args, size_t argn);

ssize_t		__getlen(t_list *strings);

uint8_t		__getflags(const char **conversion);
uint8_t		__getlength(const char **conversion);

size_t		__getwidth(const char **conversion, t_list **args);
size_t		__getprecision(const char **conversion, t_list **args);

t_list		*__getconversions(const char *f, t_list *args);

char		*__expandint(t_pf_conversion *cnv);
char		*__expanduint(t_pf_conversion *cnv);
char		*__expandchar(t_pf_conversion *cnv);
char		*__expandstr(t_pf_conversion *cnv);
char		*__getcnvstr(const char *f);
char		*__cstr(const uint8_t c, const size_t len);

void		__expandconversions(t_list *conversions, t_list **strings);
void		__cast(t_pf_conversion *cnv);
/**   printf   **/

/**   put   **/
ssize_t		__putchar_fd(const uint8_t c, const int32_t fd);
ssize_t		__putendl_fd(const char *s, const int32_t fd);
ssize_t		__putstr_fd(const char *s, const int32_t fd);
/**   put   **/

/**   str   **/
uint64_t	__strclen(const char *s, const uint8_t c);

uint8_t		__strequals(const char *s1, const char *s2);

size_t		__strlcpy(char *dst, const char *src, const size_t size);

char		**__split(const char *s, const uint8_t c);

char		*__substr(const char *s, const size_t start, const size_t len);
char		*__strsjoin(const char *s1, const char *s2, const uint8_t c);
char		*__strjoin(const char *s1, const char *s2);
char		*__strtrim(const char *s, const char *set);
char		*__strnjoin(size_t n, ...);
char		*__strdup(const char *s);
char		*__utoa_base(uint64_t n, const t_base base);
char		*__itoa_base(int64_t n, const t_base base);

char		*__strnstr(const char *s1, const char *s2, const size_t n);
char		*__strlower(char *s);
/**   str   **/

/**   misc   **/
uint8_t		__isint(const char *n);

size_t		__uintlen_base(uint64_t n, const t_base base);
size_t		__intlen_base(int64_t n, const t_base base);

char		*__getline(const int32_t fd);
/**   misc   **/

#endif
