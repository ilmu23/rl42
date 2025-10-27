// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<macro.c>>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define __RL42_INTERNAL

#include "function.h"

#include "internal/_map.h"
#include "internal/_rl42.h"
#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/_display.h"
#include "internal/_function.h"

#define _NAME_BUFFER_SIZE	64

#define _CALLER_CONTENT_PTR_OFFSET	6
#define _CALLER_CALL_ADDRESS_OFFSET	16

#define _PROT_REST	PROT_READ | PROT_EXEC
#define _PROT_EDIT	_PROT_REST | PROT_WRITE

#define get_byte(n, b)	(((uintptr_t)n >> ((8 - b) * 8)) & 0xFF)

typedef u8	 caller_segment[32];

static size_t	macro_counts[3];
static map		macros[3];

static inline const char	*_id_to_name(const size_t id, const rl42_editing_mode emode);
static inline void			*_get_ptr(u8 buf[8]);
static inline void			_copy_ptr(u8 buf[8], const void *ptr);
static inline u8			_exec_macro(rl42_line *line, const char *content);

static void	_delete_macro(caller_segment **macro);

static caller_segment	caller = {
	'\x55',	// push %rbp				// 00
	'\x48',	// movq %rsp,%rbp			// 01
	'\x89',	// movq %rsp,%rbp			// 02
	'\xe5',	// movq %rsp,%rbp			// 03
	'\x48', // movabs content,%rsx		// 04
	'\xbe', // movabs content,%rsx		// 05
	'\x00', // movabs content,%rsx		// 06
	'\x00', // movabs content,%rsx		// 07
	'\x00', // movabs content,%rsx		// 08
	'\x00', // movabs content,%rsx		// 09
	'\x00', // movabs content,%rsx		// 10
	'\x00', // movabs content,%rsx		// 11
	'\x00', // movabs content,%rsx		// 12
	'\x00', // movabs content,%rsx		// 13
	'\x48',	// movabs _exec_macro,%rax	// 14
	'\xb8',	// movabs _exec_macro,%rax	// 15
	'\x00',	// movabs _exec_macro,%rax	// 16
	'\x00',	// movabs _exec_macro,%rax	// 17
	'\x00',	// movabs _exec_macro,%rax	// 18
	'\x00',	// movabs _exec_macro,%rax	// 19
	'\x00',	// movabs _exec_macro,%rax	// 20
	'\x00',	// movabs _exec_macro,%rax	// 21
	'\x00',	// movabs _exec_macro,%rax	// 22
	'\x00',	// movabs _exec_macro,%rax	// 23
	'\xff',	// call %rax				// 24
	'\xd0',	// call %rax				// 25
	'\xc9',	// leave					// 26
	'\xc3',	// ret						// 27
	'\x90', // nop / padding			// 28
	'\x90', // nop / padding			// 29
	'\x90', // nop / padding			// 30
	'\x90', // nop / padding			// 31
};

rl42_fn_info	*create_macro(const size_t id, const char *content, const rl42_editing_mode emode) {
	caller_segment	*macro;
	const char		*tmp;
	u8				in_map;

	tmp = cstr_substr(content, 1, strlen(content) - 2);
	if (!tmp)
		return NULL;
	if (!macros[emode]) {
		macros[emode] = map(caller_segment *, 8, INTEGER, (void (*)(void *))_delete_macro);
		if (!macros[emode])
			return NULL;
	}
	in_map = 0;
	macro = mmap((void *)(uintptr_t)_exec_macro, sizeof(*macro), _PROT_EDIT, MAP_ANON | MAP_PRIVATE, 0, 0);
	if (macro == MAP_FAILED)
		return NULL;
	memcpy(macro, caller, sizeof(*macro));
	_copy_ptr(&(*macro)[_CALLER_CONTENT_PTR_OFFSET], tmp);
	if (mprotect(macro, sizeof(*macro), _PROT_REST) == -1)
		goto _create_macro_err;
	if (!map_set(macros[emode], id, macro))
		goto _create_macro_err;
	in_map = 1;
	state_flags |= STATE_REGISTER_MACRO;
	if (!rl42_register_function((rl42_fn)(uintptr_t)macro, _id_to_name(id, emode)))
		goto _create_macro_err;
	state_flags &= ~STATE_REGISTER_MACRO;
	macro_counts[emode]++;
	return get_fn_info_fn((rl42_fn)(uintptr_t)macro);
_create_macro_err:
	if (in_map)
		map_erase(macros[emode], id);
	else
		_delete_macro(&macro);
	state_flags &= ~STATE_REGISTER_MACRO;
	return NULL;
}

u8	edit_macro(rl42_fn_info *macro, const char *content) {
	caller_segment	*_macro;
	const char		*tmp;

	if (!macro->macro)
		return 1;
	tmp = cstr_substr(content, 1, strlen(content) - 2);
	if (!tmp)
		return 0;
	_macro = (caller_segment *)(uintptr_t)macro->f;
	if (mprotect(_macro, sizeof(*_macro), _PROT_EDIT) == -1)
		return 0;
	free(_get_ptr(&(*_macro)[_CALLER_CONTENT_PTR_OFFSET]));
	_copy_ptr(&(*_macro)[_CALLER_CONTENT_PTR_OFFSET], tmp);
	if (mprotect(_macro, sizeof(*_macro), _PROT_REST) != -1)
		return 1;
	return 0;
}

size_t	get_next_macro_id(const rl42_editing_mode emode) {
	return macro_counts[emode] + 1;
}

void	delete_macros(void) {
	map_delete(macros[EMACS]);
	map_delete(macros[VI_CMD]);
	map_delete(macros[VI_INS]);
	macros[EMACS] = NULL;
	macros[VI_CMD] = NULL;
	macros[VI_INS] = NULL;
}

void	delete_macro_name(const size_t id, const rl42_editing_mode emode) {
	map_erase(macros[emode], id);
}

void	init_caller(void) {
	_copy_ptr(&caller[_CALLER_CALL_ADDRESS_OFFSET], (void *)(uintptr_t)_exec_macro);
}

static inline const char	*_id_to_name(const size_t id, const rl42_editing_mode emode) {
	static const char	*emode_strs[3] = { "emacs", "vi-cmd", "vi-ins" };
	static char			name_buf[_NAME_BUFFER_SIZE];

	snprintf(name_buf, _NAME_BUFFER_SIZE, "%s-macro-%zu", emode_strs[emode], id);
	return name_buf;
}

static inline void	*_get_ptr(u8 buf[8]) {
	uintptr_t	ptr;
	size_t		i;

	for (i = ptr = 0; i < 8; i++)
		ptr |= ((uintptr_t)buf[i] << (i * 8));
	return (void *)ptr;
}

static inline void	_copy_ptr(u8 buf[8], const void *ptr) {
	buf[0] = get_byte(ptr, 8);
	buf[1] = get_byte(ptr, 7);
	buf[2] = get_byte(ptr, 6);
	buf[3] = get_byte(ptr, 5);
	buf[4] = get_byte(ptr, 4);
	buf[5] = get_byte(ptr, 3);
	buf[6] = get_byte(ptr, 2);
	buf[7] = get_byte(ptr, 1);
}

static inline u8	_exec_macro(rl42_line *line, const char *content) {
	cvector	_content;
	size_t	len;
	size_t	i;

	_content = cstr_to_rl42str(content);
	if (!_content)
		return 0;
	for (i = 0, len = vector_size(_content); i < len; i++)
		if (!__vec_ins(line->line, line->i++, vector_get(_content, i)))
			break ;
	vector_delete((vector)_content);
	return (i == len) ? term_display_line(line, 0) : 0;
}

static void	_delete_macro(caller_segment **macro) {
	free(_get_ptr(&(**macro)[_CALLER_CONTENT_PTR_OFFSET]));
	munmap(*macro, sizeof(**macro));
}
