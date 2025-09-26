// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<message.c>>

#include <stdio.h>
#include <stdarg.h>

#include "internal/_defs.h"
#include "internal/_utils.h"

static inline void	_print_message(const char *fmt, va_list args);

u8	error(const char *fmt, ...) {
	va_list	args;

	va_start(args, fmt);
	fputs(SGR_ERROR, stderr);
	_print_message(fmt, args);
	fputs(SGR0, stderr);
	va_end(args);
	return 0;
}

u8	warn(const char *fmt, ...) {
	va_list	args;

	va_start(args, fmt);
	fputs(SGR_WARN, stderr);
	_print_message(fmt, args);
	fputs(SGR0, stderr);
	va_end(args);
	return 0;
}

u8	info(const char *fmt, ...) {
	va_list	args;

	va_start(args, fmt);
	fputs(SGR_INFO, stderr);
	_print_message(fmt, args);
	fputs(SGR0, stderr);
	va_end(args);
	return 0;
}

static inline void	_print_message(const char *fmt, va_list args) {
	vfprintf(stderr, fmt, args);
	fflush(stderr);
}
