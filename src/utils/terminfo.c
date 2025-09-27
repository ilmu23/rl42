// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<terminfo.c>>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "internal/_map.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_terminfo.h"

#define _BIT_COUNT_32	01036U

#define _CAN_NUM		0xFFFFFFFEU
#define _INVALID_STR	0xFFFFU

#define _BOOL_CAPS_DEFAULT_SIZE	12
#define _NUM_CAPS_DEFAULT_SIZE	6
#define _STR_CAPS_DEFAULT_SIZE	200

#define _OFF_BIT_COUNT		0
#define _OFF_NAME_LENGTH	2
#define _OFF_BOOL_COUNTS	4
#define _OFF_NUM_COUNTS		6
#define _OFF_STR_COUNTS		8
#define _OFF_NEXT_FREE		10
#define _OFF_BODY			12

#define offset(p, n)	((void *)((uintptr_t)p + n))

#define is_present_u16(n, i)	(n.u16[i] != (u16)TI_ABS_NUM && n.u16[i] != (u16)_CAN_NUM)
#define is_present_u32(n, i)	(n.u32[i] != (u32)TI_ABS_NUM && n.u32[i] != (u32)_CAN_NUM)

typedef struct {
	u16	bit_count;
	u16	name_length;
	struct {
		u16	bools;
		u16	nums;
		u16	strs;
	}	cap_counts;
	u16	next_free;
}	header;

typedef struct {
	size_t		size;
	void		*data;
	union {
		const u32	*u32;
		const u16	*u16;
	}			nums;
	const u16	*offs;
	const char	*term_names;
	const char	*strs;
	const u8	*bools;
	header		header;
}	entry;

typedef u8			boolean_cap;
typedef u32			numeric_cap;
typedef const char *string_cap;

static inline i32	_open(const char *term);
static inline u8	_extract_dirs(const char *list, vector dirs, vector allocs);
static inline u8	_get_entry(const i32 fd, entry *entry);

static struct {
	map	boolean;
	map	numeric;
	map	string;
}	caps;

static struct {
	entry	entry;
	u8		loaded;
}	description;

u8	ti_load(const char *term) {
	size_t	i;

	if (description.loaded)
		ti_unload();
	if (!_get_entry(_open(term), &description.entry))
		return 0;
	caps.boolean = map(boolean_cap, _BOOL_CAPS_DEFAULT_SIZE, INTEGER, NULL);
	caps.numeric = map(numeric_cap, _NUM_CAPS_DEFAULT_SIZE, INTEGER, NULL);
	caps.string = map(string_cap, _STR_CAPS_DEFAULT_SIZE, INTEGER, NULL);
	if (!caps.boolean || ! caps.numeric || !caps.string)
		goto ti_load_err_ret;
	for (i = 0; i < description.entry.header.cap_counts.bools; i++) {
		if (description.entry.bools[i] && !map_set(caps.boolean, i + 1, (boolean_cap){1}))
			goto ti_load_err_ret;
	}
	if (description.entry.header.bit_count == 16) for (i = 0; i < description.entry.header.cap_counts.nums; i++) {
		if (is_present_u16(description.entry.nums, i) && !map_set(caps.numeric, i + 1, (numeric_cap){description.entry.nums.u16[i]}))
			goto ti_load_err_ret;
	} else for (i = 0; i < description.entry.header.cap_counts.nums; i++) {
		if (is_present_u32(description.entry.nums, i) && !map_set(caps.numeric, i + 1, (numeric_cap){description.entry.nums.u32[i]}))
			goto ti_load_err_ret;
	}
	for (i = 0; i < description.entry.header.cap_counts.strs; i++) {
		if (description.entry.offs[i] != _INVALID_STR && !map_set(caps.string, i + 1, (string_cap){offset(description.entry.strs, description.entry.offs[i])}))
			goto ti_load_err_ret;
	}
	description.loaded = 1;
	return 1;
ti_load_err_ret:
	map_delete(caps.boolean);
	map_delete(caps.numeric);
	map_delete(caps.string);
	return 0;
}

const char *ti_getname(void) {
	return description.entry.term_names;
}

i32	ti_getflag(const u8 name) {
	boolean_cap	*val;

	if (name > TI_BOOLEAN_CAPS)
		return TI_NOT_BOOL;
	val = map_get(caps.boolean, name);
	return (val != MAP_NOT_FOUND) ? 1 : 0;
}

i32	ti_getnum(const u8 name) {
	numeric_cap	*val;

	if (name > TI_NUMERIC_CAPS)
		return TI_NOT_NUM;
	val = map_get(caps.numeric, name);
	return (val != MAP_NOT_FOUND) ? *val : TI_ABS_NUM;
}

const char	*ti_getstr(const u16 name) {
	string_cap	*val;

	if (name > TI_STRING_CAPS)
		return TI_NOT_STR;
	val = map_get(caps.string, name);
	return (val != MAP_NOT_FOUND) ? *val : TI_ABS_STR;
}

void	ti_unload(void) {
	if (description.loaded) {
		munmap(description.entry.data, description.entry.size);
		map_delete(caps.boolean);
		map_delete(caps.numeric);
		map_delete(caps.string);
		description.loaded = 0;
	}
}

static inline i32	_open(const char *term) {
	const char	*tmp;
	ssize_t		rv;
	size_t		i;
	size_t		size;
	vector		dirs;
	vector		allocs;
	char		buf[PATH_MAX + 1];
	char		path[PATH_MAX + 1];

	dirs = vector(const char *, 8, NULL);
	allocs = vector(void *, 4, free);
	if (!dirs || !allocs)
		goto _open_err_ret;
	tmp = getenv("TERMINFO");
	if (tmp && !vector_push(dirs, tmp))
		goto _open_err_ret;
	tmp = getenv("HOME");
	if (tmp) {
		tmp = cstr_joinb(tmp, "/.terminfo", buf, PATH_MAX + 1);
		if (!vector_push(dirs, tmp))
			goto _open_err_ret;
	}
	tmp = getenv("TERMINFO_DIRS");
	if (tmp && !_extract_dirs(tmp, dirs, allocs))
		goto _open_err_ret;
	if (!vector_push(dirs, (const char *){"/etc/terminfo"}))
		goto _open_err_ret;
	if (!vector_push(dirs, (const char *){"/lib/terminfo"}))
		goto _open_err_ret;
	if (!vector_push(dirs, (const char *){"/usr/share/terminfo"}))
		goto _open_err_ret;
	for (i = rv = 0, size = vector_size(dirs); i < size; i++) {
		rv = snprintf(path, PATH_MAX, "%s/%s", *(const char **)vector_get(dirs, i), term);
		if (rv == -1 || access(path, R_OK) == 0)
			break ;
		rv = snprintf(path, PATH_MAX, "%s/%c/%s", *(const char **)vector_get(dirs, i), *term, term);
		if (rv == -1 || access(path, R_OK) == 0)
			break ;
	}
	vector_delete(allocs);
	vector_delete(dirs);
	return (rv != -1) ? open(path, O_RDONLY) : -1;
_open_err_ret:
	vector_delete(allocs);
	vector_delete(dirs);
	return -1;
}

static inline u8	_extract_dirs(const char *list, vector dirs, vector allocs) {
	const char	*tmp;
	size_t		i;
	size_t		j;
	char		buf[PATH_MAX + 1];

	buf[PATH_MAX] = '\0';
	for (i = j = 0; list[j]; j++) {
		if (buf[j] == ':') {
			if (j > i) {
				if (j - i > PATH_MAX)
					return 0;
				memcpy(buf, &list[i], j - i);
				buf[j - i] = '\0';
				tmp = strdup(buf);
				if (!tmp)
					return 0;
				if (!vector_push(allocs, tmp)) {
					free((void *)tmp);
					return 0;
				}
				if (!vector_push(dirs, tmp))
					return 0;
			}
		}
	}
	return 1;
}

static inline u8	_get_entry(const i32 fd, entry *entry) {
	struct stat	st;
	size_t		names_offset;
	size_t		bools_offset;
	size_t		nums_offset;
	size_t		offs_offset;
	size_t		strs_offset;

	if (fd == -1 || fstat(fd, &st) == -1)
		return 0;
	entry->size = st.st_size;
	entry->data = mmap(NULL, entry->size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (entry->data == MAP_FAILED)
		return 0;
	entry->header.bit_count = (*(u16 *)offset(entry->data, _OFF_BIT_COUNT) == _BIT_COUNT_32) ? 32 : 16;
	entry->header.name_length = *(u16 *)offset(entry->data, _OFF_NAME_LENGTH);
	entry->header.cap_counts.bools = *(u16 *)offset(entry->data, _OFF_BOOL_COUNTS);
	entry->header.cap_counts.nums = *(u16 *)offset(entry->data, _OFF_NUM_COUNTS);
	entry->header.cap_counts.strs = *(u16 *)offset(entry->data, _OFF_STR_COUNTS);
	entry->header.next_free = *(u16 *)offset(entry->data, _OFF_NEXT_FREE);
	names_offset = _OFF_BODY;
	bools_offset = names_offset + (entry->header.name_length * sizeof(*entry->term_names));
	nums_offset = bools_offset + (entry->header.cap_counts.bools * sizeof(*entry->bools));
	nums_offset += nums_offset & 1;
	offs_offset = nums_offset + (entry->header.cap_counts.nums * ((entry->header.bit_count == 32) ? sizeof(u32) : sizeof(u16)));
	strs_offset = offs_offset + (entry->header.cap_counts.strs * sizeof(*entry->offs));
	entry->term_names = offset(entry->data, names_offset);
	entry->bools = offset(entry->data, bools_offset);
	entry->nums.u32 = offset(entry->data, nums_offset);
	entry->offs = offset(entry->data, offs_offset);
	entry->strs = offset(entry->data, strs_offset);
	return 1;
}
