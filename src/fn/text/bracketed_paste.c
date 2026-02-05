// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<bracketed_paste.c>>

#include <string.h>
#include <unistd.h>

#define __RL42_INTERNAL
#include "function.h"

#include "internal/_defs.h"
#include "internal/_term.h"
#include "internal/_darray.h"
#include "internal/_display.h"

#define _BP_END	"\x1b[201~"

#define _BUF_SIZE	4096

extern darray	input_buf;

static inline ssize_t	_fill_buf(char *buf, const size_t buf_size);
static inline u8		_paste(rl42_line *line, const char *buf, const size_t n);

rl42_fn(bracketed_paste) {
	csi_match	bp_end;
	ssize_t		rv;
	size_t		paste_len;
	size_t		i;
	char		buf[_BUF_SIZE];

	i = 0;
_bracketed_paste_read:
	if (darray_size(input_buf) == 0) {
		rv = read(0, &buf[i], _BUF_SIZE - i);
		if (rv == -1)
			return 0;
	} else
		rv = _fill_buf(buf, _BUF_SIZE);
_bracketed_paste_find_bp_end:
	bp_end = term_find_csi(buf, rv + i, _BP_END[strl_len(_BP_END) - 1]);
	if (!bp_end.start) {
		if (!_paste(line, buf, rv))
			return 0;
		i = 0;
		goto _bracketed_paste_read;
	}
	if (!bp_end.complete) {
		paste_len = (size_t)((uintptr_t)bp_end.start - (uintptr_t)buf);
		if (!_paste(line, buf, paste_len))
			return 0;
		memmove(buf, &buf[paste_len], _BUF_SIZE - paste_len);
		memset(&buf[_BUF_SIZE - paste_len], 0, paste_len);
		i = strlen(buf);
		goto _bracketed_paste_read;
	}
	if (strncmp(bp_end.start, _BP_END, strl_len(_BP_END)) != 0) {
		paste_len = (size_t)((uintptr_t)bp_end.start - (uintptr_t)buf) + term_csi_len(bp_end.start);
		if (!_paste(line, buf, paste_len))
			return 0;
		memmove(buf, &buf[paste_len], _BUF_SIZE - paste_len);
		memset(&buf[_BUF_SIZE - paste_len], 0, paste_len);
		rv -= paste_len;
		i = strlen(buf);
		if (rv)
			goto _bracketed_paste_find_bp_end;
		goto _bracketed_paste_read;
	}
	paste_len = (bp_end.start != buf) ? (size_t)((uintptr_t)bp_end.start - (uintptr_t)buf) : 0;
	if (paste_len) {
		if (!_paste(line, buf, paste_len))
			return 0;
		if (paste_len + strl_len(_BP_END) != (size_t)rv && !darray_insert_n(input_buf, -1, rv - paste_len, &bp_end.start[strl_len(_BP_END)]))
			return 0;
	}
	return term_display_line(line, 0);
}

static inline ssize_t	_fill_buf(char *buf, const size_t buf_size) {
	size_t	inbuf_size;

	inbuf_size = darray_size(input_buf);
	if (inbuf_size > buf_size)
		inbuf_size = buf_size;
	memcpy(buf, darray_start(input_buf), inbuf_size * sizeof(*buf));
	if (inbuf_size != darray_size(input_buf))
		darray_erase_n(input_buf, 0, inbuf_size);
	else
		darray_clear(input_buf);
	return inbuf_size;
}

static inline u8	_paste(rl42_line *line, const char *buf, const size_t n) {
	size_t	csize;
	size_t	i;
	u32		ucp;

	for (i = 0, csize = charsize_utf8(buf[i]); i <= n - csize; csize = charsize_utf8(buf[i]), i += csize) {
		ucp = utf8_decode(&buf[i]);
		if (!darray_insert(line->line, line->i++, ucp))
			return 0;
	}
	if (i != n && !darray_insert_n(input_buf, 0, n - i, &buf[i]))
		return 0;
	return 1;
}
