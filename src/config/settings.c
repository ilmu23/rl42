// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<settings.c>>

#include "rl42.h"

#define _is_valid(s)	(s >= 0 && s < RL42_SETTING_NONE)

static rl42_setting_val	settings[RL42_SETTING_NONE] = {
	{ .i64 = RL42_BELL_NONE },	// bell-style
	{ .u64 = rl42_conf_on },	// colored-completion-prefix
	{ .i64 = -1 },				// completion-display-width
	{ .u64 = rl42_conf_off },	// completion-ignore-case
	{ .u64 = rl42_conf_off },	// completion-map-case
	{ .i64 = 100 },				// completion-query-items
	{ .u64 = rl42_conf_off },	// disable-completion
	{ .i64 = EMACS },			// editing-mode
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
	{ .hlc = { .type = RL42_HL_INDEX, .val.index = 39 }} // highlight-color
};

u8	rl42_set(const rl42_setting setting, const rl42_setting_val value) {
	if (!_is_valid(setting))
		return 0;
	settings[setting] = value;
	return 1;
}

rl42_setting_val	rl42_get(const rl42_setting setting) {
	if (!_is_valid(setting))
		return (rl42_setting_val){ .u64 = 0 };
	return settings[setting];
}
