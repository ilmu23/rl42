// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<terminfo.c>>

#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <linux/limits.h>

#if __STDC_VERION__ != 202301L
#include <bsd/string.h>
#endif

#include "internal/_map.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_terminfo.h"

#define _BUFFER_SIZE 4096

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

#define _TPM_PARAMS		9
#define _TPM_VARCOUNT	26
#define _TPM_SVARS		_TPM_VARCOUNT
#define _TPM_DVARS		_TPM_VARCOUNT

#define _TPM_F_DVARS_INIT_DONE	0x1U
#define _TPM_F_INCREMENT_P12		0x2U
#define _TPM_F_IN_CONDITIONAL	0x4U

#define _TPS_DELAY_ALWAYS		0x1U
#define _TPS_DELAY_NORMAL		0x2U
#define _TPS_DELAY_MANDATORY	0x4U

#define offset(p, n)	((void *)((uintptr_t)p + n))

#define is_present_u16(n, i)	(n.u16[i] != (u16)TI_ABS_NUM && n.u16[i] != (u16)_CAN_NUM)
#define is_present_u32(n, i)	(n.u32[i] != (u32)TI_ABS_NUM && n.u32[i] != (u32)_CAN_NUM)

#define tp_stack_top(st)		(*(uintptr_t *)vector_last(st))
#define tp_stack_push(st, val)	(vector_push(st, (uintptr_t){val}))

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

static inline u8	_tpm_sprintf(const char **seq, char buf[_BUFFER_SIZE + 1], const uintptr_t val);

static inline i64	_tps_get_baud_rate(void);
static inline i8	_tps_sleep(const u64 ms, const u8 sleep_type, ssize_t (*putc)(const char));
static inline u8	_tps_is_delay(const char *s);

static struct {
	map	boolean;
	map	numeric;
	map	string;
}	caps;

static struct {
	entry	entry;
	char	name[NAME_MAX + 1];
	u8		loaded;
}	description;

static char	seq_buf[_BUFFER_SIZE + 1];

u8	ti_load(const char *term) {
	size_t	i;

	if (description.loaded) {
		if (strcmp(description.name, term) == 0)
			return 1;
		ti_unload();
	}
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
	snprintf(description.name, NAME_MAX, "%s", term);
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

const char	*ti_tgoto(const char *seq, const i32 row, const i32 col) {
	if (row < 1 || col < 1)
		return NULL;
	return ti_tparm(seq, row - 1, col - 1);
}

const char	*ti_tparm(const char *seq, ...) {
	static uintptr_t	svars[_TPM_SVARS];
	uintptr_t			dvars[_TPM_DVARS];
	uintptr_t			params[_TPM_PARAMS];
	uintptr_t			x;
	uintptr_t			y;
	va_list				args;
	vector				stack;
	size_t				seqlen;
	size_t				i;
	char				tmp[_BUFFER_SIZE + 1];
	u16					present_params;
	u8					param_count;
	u8					flags;

	if (seq == TI_ABS_STR || seq == TI_NOT_STR)
		return NULL;
	for (present_params = param_count = seqlen = 0; seq[seqlen]; seqlen++) {
		if (seq[seqlen] == 'p') switch (seq[++seqlen]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (!((present_params >> (seq[seqlen] - '1')) & 0x1U)) {
					present_params |= 0x1U << (seq[seqlen] - '1');
					if (seq[seqlen] - '0' > param_count)
						param_count = seq[seqlen] - '0';
				}
				break ;
			default:
				return NULL;
		}
	}
	stack = vector(uintptr_t, 5, NULL);
	if (!stack)
		goto _ti_tparm_err_ret;
	va_start(args, seq);
	for (i = 0; i < _TPM_PARAMS; i++) {
		params[i] = (param_count) ? va_arg(args, uintptr_t) : (uintptr_t)-1;
		param_count -= (present_params >> i) & 0x1U;
	}
	va_end(args);
	memset(seq_buf, 0, strlen(seq_buf));
	for (i = flags = 0; *seq && i < _BUFFER_SIZE; seq++) {
		if (*seq == '%') switch (*(++seq)) {
			case 'c':
				seq_buf[i++] = (u8)tp_stack_top(stack);
				vector_pop(stack);
				break ;
			case 'd':
			case 'o':
			case 'x':
			case 'X':
			case 's':
			case ':':
			case ' ':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (!_tpm_sprintf(&seq, tmp, tp_stack_top(stack)))
					goto _ti_tparm_err_ret;
				i = strlcat(seq_buf, tmp, _BUFFER_SIZE + 1);
				if (i >= sizeof(seq_buf))
					goto _ti_tparm_err_ret;
				break ;
			case 'i':
				if (~flags & _TPM_F_INCREMENT_P12) {
					params[0]++;
					params[1]++;
					flags |= _TPM_F_INCREMENT_P12;
				}
				break ;
			case 'p':
				seq++;
				if (in_range(*seq, '1', '9'))
					tp_stack_push(stack, params[*seq - '1']);
				else
					goto _ti_tparm_err_ret;
				break ;
			case 'P':
				if (islower(*(++seq))) {
					if (~flags & _TPM_F_DVARS_INIT_DONE) {
						memset(dvars, 0, sizeof(dvars));
						flags |= _TPM_F_DVARS_INIT_DONE;
					}
					dvars[*seq - 'a'] = tp_stack_top(stack);
				} else
					svars[*seq - 'A'] = tp_stack_top(stack);
				vector_pop(stack);
				break ;
			case 'g':
				if (islower(*(++seq))) {
					if (~flags & _TPM_F_DVARS_INIT_DONE) {
						memset(dvars, 0, sizeof(dvars));
						flags |= _TPM_F_DVARS_INIT_DONE;
					}
					tp_stack_push(stack, dvars[*seq - 'a']);
				} else
					tp_stack_push(stack, svars[*seq - 'A']);
				break ;
			case '\'':
				tp_stack_push(stack, *(++seq));
				seq++;
				break ;
			case '{':
				for (x = 0, seq++; isdigit(*seq); seq++)
					x = x * 10 + *seq - '0';
				tp_stack_push(stack, x);
				break ;
			case 'l':
				x = strlen((const char *)tp_stack_top(stack));
				vector_pop(stack);
				tp_stack_push(stack, x);
				break ;
			case '+':
			case '-':
			case '*':
			case '/':
			case 'm':
			case '|':
			case '^':
			case '=':
			case '>':
			case '<':
			case 'A':
			case 'O':
				x = tp_stack_top(stack);
				vector_pop(stack);
				y = tp_stack_top(stack);
				vector_pop(stack);
				switch (*seq) {
					case '+':
						tp_stack_push(stack, y + x);
						break ;
					case '-':
						tp_stack_push(stack, y - x);
						break ;
					case '*':
						tp_stack_push(stack, y * x);
						break ;
					case '/':
						tp_stack_push(stack, y / x);
						break ;
					case 'm':
						tp_stack_push(stack, y % x);
						break ;
					case '|':
						tp_stack_push(stack, y | x);
						break ;
					case '^':
						tp_stack_push(stack, y ^ x);
						break ;
					case '=':
						tp_stack_push(stack, y == x);
						break ;
					case '>':
						tp_stack_push(stack, y > x);
						break ;
					case '<':
						tp_stack_push(stack, y < x);
						break ;
					case 'A':
						tp_stack_push(stack, y && x);
						break ;
					case 'O':
						tp_stack_push(stack, y || x);
				}
				break ;
			case '!':
			case '~':
				x = tp_stack_top(stack);
				vector_pop(stack);
				tp_stack_push(stack, (*seq == '!') ? !x : ~x);
				break ;
			case '?':
				if (flags & _TPM_F_IN_CONDITIONAL)
					goto _ti_tparm_err_ret;
				flags |=  _TPM_F_IN_CONDITIONAL;
				break ;
			case 't':
				if (~flags & _TPM_F_IN_CONDITIONAL)
					goto _ti_tparm_err_ret;
				x = tp_stack_top(stack);
				vector_pop(stack);
				if (!x) {
					for (seq++, y = 0; ; seq++) {
						if (*seq == '%') switch (*(++seq)) {
							case '?':
								y++;
								break ;
							case ';':
							case 'e':
								if (y == 0) {
									if (*seq == ';')
										flags &= ~_TPM_F_IN_CONDITIONAL;
									goto _ti_tparm_continue;
								}
								y -= (*seq == ':') ? 1 : 0;
								break ;
							case '\0':
								goto _ti_tparm_continue;
						}
					}
				}
				break ;
			case 'e':
				if (~flags & _TPM_F_IN_CONDITIONAL)
					goto _ti_tparm_err_ret;
				for (seq++, x = 0; ; seq++) {
					if (*seq == '%') switch (*(++seq)) {
						case '?':
							x++;
							break ;
						case ';':
							if (x == 0) {
								flags &= ~_TPM_F_IN_CONDITIONAL;
								goto _ti_tparm_continue;
							}
							x--;
						case '\0':
							goto _ti_tparm_continue;
					}
				}
				break ;
			case ';':
				if (~flags & _TPM_F_IN_CONDITIONAL)
					goto _ti_tparm_err_ret;
				flags &=  ~_TPM_F_IN_CONDITIONAL;
				break ;
			case '%':
				seq_buf[i++] = '%';
		} else
			seq_buf[i++] = *seq;
_ti_tparm_continue:
		;
	}
	vector_delete(stack);
	seq_buf[i] = '\0';
	return (i < _BUFFER_SIZE) ? seq_buf : NULL;
_ti_tparm_err_ret:
	vector_delete(stack);
	return NULL;
}

#define str_equals(s1, s2)	(s1 == s2 || (s1 && s2 && strcmp(s1, s2) == 0))

#define _TPS_SPEEDCOUNT	31

#define _TPS_SLEEP_NANOSLEEP	0
#define _TPS_SLEEP_PAD			1

static const speed_t	_tps_speeds[_TPS_SPEEDCOUNT * 2] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
						13, 14, 15, 4097, 4098, 4099, 4100, 4101, 4102, 4103, 4104, 4105, 4106,
						4107, 4108, 4109, 4110, 41111, 0, 50, 75, 110, 134, 150, 200, 300, 600,
						1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 130400,
						460800, 500000, 576000, 921600, 1000000, 1152000, 1500000,
						2000000, 2500000, 3000000, 3500000, 4000000
};

static speed_t	_tps_ospeed;

ssize_t	ti_tputs(const char *s, const size_t affln, ssize_t (*putc)(const char)) {
	ssize_t	bytes_written;
	ssize_t	rv;
	u64		delay_ms;
	u8		delay_type;
	u8		delay;

	if (s == TI_ABS_STR || s == TI_NOT_STR)
		return 0;
	delay = 0;
	if (str_equals(s, ti_getstr(ti_bel)) || str_equals(s, ti_getstr(ti_flash)))
		delay |= _TPS_DELAY_ALWAYS;
	if (!ti_getflag(ti_xon) && ti_getnum(ti_pb) != (i32)TI_ABS_NUM && _tps_get_baud_rate() >= ti_getnum(ti_pb))
		delay |= _TPS_DELAY_NORMAL;
	delay_type = (ti_getflag(ti_npc)) ? _TPS_SLEEP_NANOSLEEP : _TPS_SLEEP_PAD;
	bytes_written = 0;
	do {
		if (_tps_is_delay(s)) {
			s += 2;
			delay_ms = strtoul(s, (char **)&s, 10);
			if (*s == '.') {
				delay_ms *= 10;
				if (isdigit(*(++s)))
					delay_ms += *s++ - '0';
			}
			while (*s == '*' || *s == '/') {
				if (*s++ == '*')
					delay_ms *= affln;
				else
					delay |= _TPS_DELAY_MANDATORY;
			}
			s++;
			if (delay_ms && delay)
				rv = _tps_sleep(delay_ms, delay_type, putc);
		} else
			rv = putc(*s++);
		bytes_written += rv;
	} while (*s && rv == 1);
	return (rv != -1) ? bytes_written : -1;
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

static inline u8	_tpm_sprintf(const char **seq, char buf[_BUFFER_SIZE + 1], const uintptr_t val) {
	const char	*tmp;
	ssize_t		rv;
	size_t		i;
	char		fmt[_BUFFER_SIZE + 1];

	tmp = *seq;
	if (*tmp == ':')
		tmp++;
	for (i = 1, *fmt = '%'; i < _BUFFER_SIZE && strchr("-*# 0123456789.", *tmp) && *tmp; tmp++, i++)
		fmt[i] = *tmp;
	if (i == _BUFFER_SIZE)
		return 0;
	fmt[i++] = *tmp;
	fmt[i] = '\0';
	switch (fmt[i - 1]) {
		case 'd':
			rv = snprintf(buf, _BUFFER_SIZE, fmt, (const i32)val);
			break ;
		case 'o':
		case 'x':
		case 'X':
			rv = snprintf(buf, _BUFFER_SIZE, fmt, (const u32)val);
			break ;
		case 's':
			rv = snprintf(buf, _BUFFER_SIZE, fmt, (const char *)val);
	}
	*seq = tmp;
	return (rv != -1 && rv < _BUFFER_SIZE) ? 1 : 0;
}

static inline i64	_tps_get_baud_rate(void) {
	size_t	i;

	for (i = 0; i < _TPS_SPEEDCOUNT; i++)
		if (_tps_speeds[i] >= _tps_ospeed)
			return (_tps_speeds[i] == _tps_ospeed) ? (i64)_tps_speeds[i + _TPS_SPEEDCOUNT] : -1;
	return -1;
}

static inline i8	_tps_sleep(const u64 ms, const u8 sleep_type, ssize_t (*putc)(const char)) {
	static const char	*pad_char;
	struct timespec		alarm_clock;
	ssize_t				bytes_written;
	u64					pads_needed;


	if  (sleep_type == _TPS_SLEEP_NANOSLEEP) {
		if (clock_gettime(CLOCK_MONOTONIC, &alarm_clock) == -1)
			return 0;
		errno = 0;
		alarm_clock.tv_nsec += ms * 1000000;
		while (alarm_clock.tv_nsec >= 1000000000) {
			alarm_clock.tv_nsec -= 1000000000;
			alarm_clock.tv_sec++;
		}
		while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &alarm_clock, NULL) != 0)
			if (errno != EINTR)
				return 0;
	} else {
		if (!pad_char) {
			pad_char = ti_getstr(ti_pad);
			if (pad_char == TI_ABS_STR)
				pad_char = "\0";
		}
		for (pads_needed = (ms * _tps_get_baud_rate()) / 1000; pads_needed; pads_needed--) {
			bytes_written = putc(*pad_char);
			if (bytes_written == -1)
				return 0;
		}
	}
	return 1;
}

static inline u8	_tps_is_delay(const char *s) {
	size_t	n;

	if (*s++ != '$')
		return 0;
	if (*s++ != '<')
		return 0;
	for (n = 0; isdigit(*s++); n++)
		;
	if (!n)
		return 0;
	if (*s == '.' && (!isdigit(*(++s)) || isdigit(*(++s))))
		return 0;
	if (*s == '*' || *s == '/')
		n = *s++;
	if ((*s == '*' || *s == '/') && *s++ == (const char)n)
		return 0;
	return (*s == '>') ? 1 : 0;
}
