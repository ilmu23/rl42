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
#include "internal/_term.h"
#include "internal/_utils.h"
#include "internal/_terminfo_caps.h"

#define _BUF_SIZE	17

#define kb_event(c, t, m)	((rl42_kb_event){.code = c, .text = t, .mods = m})

static i32	efd;

static u32	kbs;
u32			kcbs;

static inline rl42_kb_event	*_parse_event(const char buf[_BUF_SIZE], rl42_kb_event *event);

rl42_kb_event	*kb_listen(const i32 timeout) {
	static rl42_kb_event	kb_event;
	struct epoll_event		event;
	ssize_t					rv;
	char					buf[_BUF_SIZE];

	rv = epoll_wait(efd, &event, 1, timeout);
	if (rv == 1) {
		rv = read(0, buf, 16);
		buf[rv] = '\0';
		return _parse_event(buf, &kb_event);
	}
	if (rv == -1)
		error("rl42: kb_listen: %s", strerror(errno));
	return NULL;
}

void	clean_kb_listener(void) {
	epoll_ctl(efd, EPOLL_CTL_DEL, 0, NULL);
	close(efd);
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

static inline rl42_kb_event	*_parse_event(const char buf[_BUF_SIZE], rl42_kb_event *event) {
	memset(event, 0, sizeof(*event));
	if (strncmp(buf, "\x1b[", 2) == 0 || strncmp(buf, "\x1bO", 2) == 0) {
		switch (term_match_key_seq(buf)) {
			case ti_kf1:
				*event = kb_event(KB_KEY_LEGACY_F1, 0, 0);
				return event;
			case ti_kf2:
				*event = kb_event(KB_KEY_LEGACY_F2, 0, 0);
				return event;
			case ti_kf3:
				*event = kb_event(KB_KEY_LEGACY_F3, 0, 0);
				return event;
			case ti_kf4:
				*event = kb_event(KB_KEY_LEGACY_F4, 0, 0);
				return event;
			case ti_kf5:
				*event = kb_event(KB_KEY_LEGACY_F5, 0, 0);
				return event;
			case ti_kf6:
				*event = kb_event(KB_KEY_LEGACY_F6, 0, 0);
				return event;
			case ti_kf7:
				*event = kb_event(KB_KEY_LEGACY_F7, 0, 0);
				return event;
			case ti_kf8:
				*event = kb_event(KB_KEY_LEGACY_F8, 0, 0);
				return event;
			case ti_kf9:
				*event = kb_event(KB_KEY_LEGACY_F9, 0, 0);
				return event;
			case ti_kf10:
				*event = kb_event(KB_KEY_LEGACY_F10, 0, 0);
				return event;
			case ti_kf11:
				*event = kb_event(KB_KEY_LEGACY_F11, 0, 0);
				return event;
			case ti_kf12:
				*event = kb_event(KB_KEY_LEGACY_F12, 0, 0);
				return event;
			case ti_kcuu1:
				*event = kb_event(KB_KEY_LEGACY_UP, 0, 0);
				return event;
			case ti_kcud1:
				*event = kb_event(KB_KEY_LEGACY_DOWN, 0, 0);
				return event;
			case ti_kcub1:
				*event = kb_event(KB_KEY_LEGACY_LEFT, 0, 0);
				return event;
			case ti_kcuf1:
				*event = kb_event(KB_KEY_LEGACY_RIGHT, 0, 0);
				return event;
			case ti_kich1:
				*event = kb_event(KB_KEY_LEGACY_INSERT, 0, 0);
				return event;
			case ti_khome:
				*event = kb_event(KB_KEY_LEGACY_HOME, 0, 0);
				return event;
			case ti_knp:
				*event = kb_event(KB_KEY_LEGACY_PAGE_UP, 0, 0);
				return event;
			case ti_kdch1:
				*event = kb_event(KB_KEY_LEGACY_DELETE, 0, 0);
				return event;
			case ti_kend:
				*event = kb_event(KB_KEY_LEGACY_END, 0, 0);
				return event;
			case ti_kpp:
				*event = kb_event(KB_KEY_LEGACY_PAGE_DOWN, 0, 0);
				return event;
			case ti_kri:
				*event = kb_event(KB_KEY_LEGACY_UP, 0, KB_MOD_SHIFT);
				return event;
			case ti_kind:
				*event = kb_event(KB_KEY_LEGACY_DOWN, 0, KB_MOD_SHIFT);
				return event;
			case ti_kLFT:
				*event = kb_event(KB_KEY_LEGACY_LEFT, 0, KB_MOD_SHIFT);
				return event;
			case ti_kRIT:
				*event = kb_event(KB_KEY_LEGACY_RIGHT, 0, KB_MOD_SHIFT);
				return event;
			case ti_kIC:
				*event = kb_event(KB_KEY_LEGACY_INSERT, 0, KB_MOD_SHIFT);
				return event;
			case ti_kHOM:
				*event = kb_event(KB_KEY_LEGACY_HOME, 0, KB_MOD_SHIFT);
				return event;
			case ti_kNXT:
				*event = kb_event(KB_KEY_LEGACY_PAGE_UP, 0, KB_MOD_SHIFT);
				return event;
			case ti_kDC:
				*event = kb_event(KB_KEY_LEGACY_DELETE, 0, KB_MOD_SHIFT);
				return event;
			case ti_kEND:
				*event = kb_event(KB_KEY_LEGACY_END, 0, KB_MOD_SHIFT);
				return event;
			case ti_kPRV:
				*event = kb_event(KB_KEY_LEGACY_PAGE_DOWN, 0, KB_MOD_SHIFT);
				return event;
		}
		return NULL;
	}
	if (buf[0] == '\x1b' && buf[1]) {
		event->mods |= KB_MOD_ALT;
		buf++;
	}
	event->code = utf8_decode(buf);
	// TODO: proper unicode case checks
	if (event->code == kcbs) {
		event->mods |= KB_MOD_CTRL;
		event->code = kbs;
	} else if (event->code < ' ') {
		event->mods |= KB_MOD_CTRL;
		event->code |= 0x60;
	} else if (event->code >= 'A' && event->code <= 'Z') {
		event->mods |= KB_MOD_SHIFT;
		event->code |= 0x20;
	}
	event->text = event->code;
	return event;
}
