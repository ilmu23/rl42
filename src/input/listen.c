// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<listen.c>>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "internal/_kb.h"
#include "internal/_defs.h"
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_terminfo_caps.h"

#define _BUF_SIZE	17

#define kb_event(c, t, m)	((rl42_kb_event){ .esc = NULL, .code = c, .text = t, .mods = m })
#define kb_event_ue(e)		((rl42_kb_event){ .esc = e, .code = KB_UNRECOGNIZED_ESCAPE, .text = 0, .mods = 0 })

static i32	efd;

static u32	kbs;
u32			kcbs;

darray	input_buf;

static inline rl42_kb_event	*_parse_event(const char *buf, const size_t buf_size, rl42_kb_event *event);
static inline const char	*_fill_buf(char *buf, const size_t buf_size);

rl42_kb_event	*kb_listen(const i32 timeout) {
	char	buf[_BUF_SIZE];

	return kb_listen_buf(timeout, buf, _BUF_SIZE);
}

rl42_kb_event	*kb_listen_buf(const i32 timeout, char *buf, const size_t buf_size) {
	static rl42_kb_event	kb_event;
	struct epoll_event		event;
	ssize_t					rv;

	if (darray_size(input_buf) > 0)
		return (_parse_event(_fill_buf(buf, buf_size), buf_size, &kb_event));
	term_show_cursor();
	rv = epoll_wait(efd, &event, 1, timeout);
	term_hide_cursor();
	if (rv == 1) {
		rv = read(0, buf, buf_size - 1);
		if (rv == -1) {
			error("rl42: kb_listen: %s", strerror(errno));
			return NULL;
		}
		buf[rv] = '\0';
		return _parse_event(buf, buf_size, &kb_event);
	}
	if (rv == -1) {
		if (errno == EINTR)
			return (kb_listen(timeout));
		error("rl42: kb_listen: %s", strerror(errno));
	}
	return NULL;
}

void	clean_kb_listener(void) {
	epoll_ctl(efd, EPOLL_CTL_DEL, 0, NULL);
	close(efd);
}

u32	kb_event_to_ucp(const rl42_kb_event *event) {
	u32	ucp;

	ucp = event->code;
	if (event->mods & KB_MOD_SHIFT)
		ucp = to_upper(ucp);
	if (event->mods & KB_MOD_CTRL && event->code != kcbs)
		ucp &= ~0x60;
	return ucp;
}

u8	init_kb_listener(void) {
	struct epoll_event	ev;
	const char			*seq;

	seq = term_get_seq(ti_kbs);
	if (seq) {
		kbs = *seq;
		kcbs = (kbs == '\b') ? '\x7f' : '\b';
	}
	efd = epoll_create(1);
	if (efd == -1)
		return 0;
	ev = (struct epoll_event){
		.events = EPOLLIN,
		.data.fd = 0
	};
	return (epoll_ctl(efd, EPOLL_CTL_ADD, 0, &ev) != -1) ? 1 : 0;
}

static inline rl42_kb_event	*_parse_event(const char *buf, const size_t buf_size, rl42_kb_event *event) {
	darray	escape;
	size_t	buf_len;
	size_t	seq_len;
	size_t	i;

	buf_len = strlen(buf);
	memset(event, 0, sizeof(*event));
	if (buf_size > 1 && strlen(buf) > 2 && (strncmp(buf, "\x1b[", 2) == 0 || strncmp(buf, "\x1bO", 2) == 0)) {
		switch (term_match_key_seq(buf)) {
			case ti_kf1:
				*event = kb_event(KB_KEY_LEGACY_F1, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf2:
				*event = kb_event(KB_KEY_LEGACY_F2, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf2));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf3:
				*event = kb_event(KB_KEY_LEGACY_F3, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf3));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf4:
				*event = kb_event(KB_KEY_LEGACY_F4, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf4));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf5:
				*event = kb_event(KB_KEY_LEGACY_F5, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf5));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf6:
				*event = kb_event(KB_KEY_LEGACY_F6, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf6));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf7:
				*event = kb_event(KB_KEY_LEGACY_F7, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf7));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf8:
				*event = kb_event(KB_KEY_LEGACY_F8, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf8));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf9:
				*event = kb_event(KB_KEY_LEGACY_F9, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf9));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf10:
				*event = kb_event(KB_KEY_LEGACY_F10, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf10));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf11:
				*event = kb_event(KB_KEY_LEGACY_F11, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf11));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kf12:
				*event = kb_event(KB_KEY_LEGACY_F12, 0, 0);
				seq_len = strlen(term_get_seq(ti_kf12));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kcuu1:
				*event = kb_event(KB_KEY_LEGACY_UP, 0, 0);
				seq_len = strlen(term_get_seq(ti_kcuu1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kcud1:
				*event = kb_event(KB_KEY_LEGACY_DOWN, 0, 0);
				seq_len = strlen(term_get_seq(ti_kcud1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kcub1:
				*event = kb_event(KB_KEY_LEGACY_LEFT, 0, 0);
				seq_len = strlen(term_get_seq(ti_kcub1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kcuf1:
				*event = kb_event(KB_KEY_LEGACY_RIGHT, 0, 0);
				seq_len = strlen(term_get_seq(ti_kcuf1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kich1:
				*event = kb_event(KB_KEY_LEGACY_INSERT, 0, 0);
				seq_len = strlen(term_get_seq(ti_kich1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_khome:
				*event = kb_event(KB_KEY_LEGACY_HOME, 0, 0);
				seq_len = strlen(term_get_seq(ti_khome));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_knp:
				*event = kb_event(KB_KEY_LEGACY_PAGE_UP, 0, 0);
				seq_len = strlen(term_get_seq(ti_knp));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kdch1:
				*event = kb_event(KB_KEY_LEGACY_DELETE, 0, 0);
				seq_len = strlen(term_get_seq(ti_kdch1));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kend:
				*event = kb_event(KB_KEY_LEGACY_END, 0, 0);
				seq_len = strlen(term_get_seq(ti_kend));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kpp:
				*event = kb_event(KB_KEY_LEGACY_PAGE_DOWN, 0, 0);
				seq_len = strlen(term_get_seq(ti_kpp));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kri:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_UP, 0, 0);
				seq_len = strlen(term_get_seq(ti_kri));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kind:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_DOWN, 0, 0);
				seq_len = strlen(term_get_seq(ti_kind));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kLFT:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_LEFT, 0, 0);
				seq_len = strlen(term_get_seq(ti_kLFT));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kRIT:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_RIGHT, 0, 0);
				seq_len = strlen(term_get_seq(ti_kRIT));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kIC:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_INSERT, 0, 0);
				seq_len = strlen(term_get_seq(ti_kIC));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kHOM:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_HOME, 0, 0);
				seq_len = strlen(term_get_seq(ti_kHOM));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kNXT:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_PAGE_UP, 0, 0);
				seq_len = strlen(term_get_seq(ti_kNXT));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kDC:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_DELETE, 0, 0);
				seq_len = strlen(term_get_seq(ti_kDC));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kEND:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_END, 0, 0);
				seq_len = strlen(term_get_seq(ti_kEND));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
			case ti_kPRV:
				*event = kb_event(KB_KEY_LEGACY_SHIFT_PAGE_DOWN, 0, 0);
				seq_len = strlen(term_get_seq(ti_kPRV));
				darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
				return event;
		}
		if (buf[1] == 'O')
			return NULL;
		seq_len = term_csi_len(buf);
		darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
		escape = darray(u32, seq_len, NULL);
		if (!escape)
			return NULL;
		for (i = 0; i < seq_len; i++)
			darray_push(escape, (u32){buf[i]});
		*event = kb_event_ue(escape);
		return event;
	}
	seq_len = 0;
	if (buf_size > 1 && buf[0] == '\x1b' && buf[1]) {
		event->mods |= KB_MOD_ALT;
		seq_len++;
	}
	event->code = utf8_decode(&buf[seq_len]);
	seq_len += charsize_utf8(buf[seq_len]);
	if (event->code == kcbs) {
		event->mods |= KB_MOD_CTRL;
		event->code = kbs;
	} else if (event->code < ' ') {
		event->mods |= KB_MOD_CTRL;
		event->code |= 0x60;
	} else if (is_upper(event->code)) {
		event->mods |= KB_MOD_SHIFT;
		event->code = to_lower(event->code);
	}
	darray_insert_n(input_buf, 0, buf_len - seq_len, &buf[seq_len]);
	event->text = event->code;
	return event;
}

static inline const char	*_fill_buf(char *buf, const size_t buf_size) {
	size_t	inbuf_size;

	inbuf_size = darray_size(input_buf);
	if (inbuf_size > buf_size - 1)
		inbuf_size = buf_size - 1;
	memcpy(buf, darray_start(input_buf), inbuf_size * sizeof(*buf));
	buf[inbuf_size] = '\0';
	if (inbuf_size != darray_size(input_buf))
		darray_erase_n(input_buf, 0, inbuf_size);
	else
		darray_clear(input_buf);
	return buf;
}
