// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<settings.c>>

#if __STDC_VERSION__ < 202311L
# include <bsd/string.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "rl42.h"

#include "internal/_term.h"
#include "internal/_history.h"
#include "internal/_keybinds.h"

#define _EP_BUF_SIZE	PATH_MAX + 1

#define _is_valid(s)	(s >= 0 && s < RL42_SETTING_NONE)

#define _set_hfile_success(p)	(((history_file && p) || (!history_file && !p)))

const char	*history_file = NULL;

static rl42_setting_val	settings[RL42_SETTING_NONE] = {
	{ .i64 = RL42_BELL_NONE },	// bell-style
	{ .u64 = rl42_conf_on },	// colored-completion-prefix
	{ .i64 = -1 },				// completion-display-width
	{ .u64 = rl42_conf_off },	// completion-ignore-case
	{ .u64 = rl42_conf_off },	// completion-map-case
	{ .i64 = 100 },				// completion-query-items
	{ .u64 = rl42_conf_off },	// disable-completion
	{ .i64 = RL42_EM_EMACS },	// editing-mode
	{ .u64 = rl42_conf_on },	// enable-highlight
	{ .u64 = rl42_conf_off },	// expand-tilde
	{ .i64 = -1 },				// history-size
	{ .u64 = rl42_conf_off },	// horizontal-scroll-mode
	{ .i64 = 500 },				// keyseq-timeout
	{ .u64 = rl42_conf_on },	// mark-directories
	{ .u64 = rl42_conf_off },	// mark-symlinked directories
	{ .u64 = rl42_conf_off },	// match-hidden-files
	{ .u64 = rl42_conf_on },	// page-completions
	{ .u64 = rl42_conf_off },	// search-ignore-case
	{ .u64 = rl42_conf_off },	// visible-stats
	{ .hlc = { .type = RL42_HL_INDEX, .val.index = 39 }},	// highlight-color
	{ .u64 = rl42_conf_on },	// enable-bracketed-paste
	{ .u64 = rl42_conf_off },	// disable-history
};

static inline const char	*_expand_path(const char *path);

u8	rl42_set_history_file(const char *path) {
	hist_clean();
	free((void *)history_file);
	history_file = (path) ? _expand_path(path) : NULL;
	if (_set_hfile_success(path))
		return hist_load(history_file);
	return 0;
}

const char	*rl42_get_history_file(void) {
	return history_file;
}

u8	rl42_set(const rl42_setting setting, const rl42_setting_val value) {
	if (!_is_valid(setting))
		return 0;
	settings[setting] = value;
	switch (setting) {
		case RL42_HISTORY_SIZE:
			hist_remove_extra_nodes();
			break ;
		case RL42_ENABLE_BRACKETED_PASTE:
			if (!term_set_bpm((value.u64) ? BPM_ENABLED : BPM_DISABLED)) {
				settings[setting].u64 = (settings[setting].u64) ? rl42_conf_off : rl42_conf_on;
				return 0;
			}
			break ;
		case RL42_EDITING_MODE:
			set_editing_mode(settings[setting].i64);
			break ;
		default:
			;
	}
	return 1;
}

rl42_setting_val	rl42_get(const rl42_setting setting) {
	if (!_is_valid(setting))
		return (rl42_setting_val){ .u64 = 0 };
	return settings[setting];
}

static inline const char	*_expand_path(const char *path) {
	char	buf[_EP_BUF_SIZE];

	if (*path == '~') {
		memset(buf, 0, sizeof(buf));
		strlcpy(buf, rl42_getenv("HOME"), sizeof(buf));
		strlcat(buf, &path[1], sizeof(buf));
		return strdup(buf);
	}
	return strdup(path);
}
