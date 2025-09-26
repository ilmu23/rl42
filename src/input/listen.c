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
#include "internal/_utils.h"

#define _BUF_SIZE	17

static i32	efd;

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
	if (strncmp(buf, "\x1b[", 2) == 0) {
		// TODO: escape parsing
		return NULL;
	}
	if (buf[0] == '\x1b' && buf[1]) {
		event->mods |= KB_MOD_ALT;
		buf++;
	}
	event->code = utf8_decode(buf);
	// TODO: proper unicode case checks
	if (event->code < ' ') {
		event->mods |= KB_MOD_CTRL;
		event->code |= 0x60;
	} else if (event->code >= 'A' && event->code <= 'Z') {
		event->mods |= KB_MOD_SHIFT;
		event->code |= 0x20;
	}
	event->text = event->code;
	return event;
}
