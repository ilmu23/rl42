// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<output.c>>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "internal/_term.h"

#define _BUFFER_SIZE	0x8000

static struct {
	size_t	i;
	char	data[_BUFFER_SIZE];
}	output_buffer;

ssize_t	term_putsf(const char *fmt, ...) {
	va_list	args;
	ssize_t	rv;
	size_t	i;
	size_t	n;
	char	*buf;

	rv = 0;
	va_start(args, fmt);
	rv = vsnprintf(&output_buffer.data[output_buffer.i], _BUFFER_SIZE - output_buffer.i, fmt, args);
	if (rv == -1)
		return -1;
	if ((size_t)rv >= _BUFFER_SIZE - output_buffer.i) {
		buf = malloc(rv * sizeof(*buf));
		if (!buf)
			return -1;
		va_end(args);
		va_start(args, fmt);
		rv = vsnprintf(buf, rv, fmt, args);
		if (rv == -1)
			goto _term_putsf_err;
		i = 0;
		do {
			n = min((size_t)rv - i, _BUFFER_SIZE - output_buffer.i);
			memcpy(&output_buffer.data[output_buffer.i], &buf[i], n);
			output_buffer.i += n;
			if (!term_flush_outbuf())
				goto _term_putsf_err;
			i += n;
		} while (i < (size_t)rv);
		free(buf);
	}
	output_buffer.i += (size_t)rv;
	return rv;
_term_putsf_err:
	free(buf);
	return -1;
}

ssize_t	term_putchar(const char c) {
	if (output_buffer.i == _BUFFER_SIZE && !term_flush_outbuf())
		return -1;
	output_buffer.data[output_buffer.i++] = c;
	return 1;
}

ssize_t	term_putchar_unbuffered(const char c) {
	return write(1, &c, sizeof(c));
}

void	term_clear_outbuf(void) {
	output_buffer.i = 0;
}

u8	term_flush_outbuf(void) {
	ssize_t	rv;

_term_outbuf_flush_write:
	rv = write(1, output_buffer.data, output_buffer.i);
	if (rv == -1)
		return 0;
	output_buffer.i -= (size_t)rv;
	if (output_buffer.i) {
		memmove(output_buffer.data, &output_buffer.data[rv], output_buffer.i * sizeof(output_buffer.data));
		goto _term_outbuf_flush_write;
	}
	return 1;
}
