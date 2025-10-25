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
	{ .u64 = RL42_BELL_NONE },	// bell-style
	{ .u64 = 1 },				// colored-completion-prefix
	{ .i64 = -1 },				// completion-display-width
	{ .u64 = 0 },				// completion-ignore-case
	{ .u64 = 0 },				// completion-map-case
	{ .i64 = 100 },				// completion-query-items
	{ .u64 = 0 },				// disable-completion
	{ .u64 = EMACS },			// editing-mode
	{ .u64 = 1 },				// enable-highlight
	{ .u64 = 0 },				// expand-tilde
	{ .i64 = -1 },				// history-size
	{ .u64 = 0 },				// horizontal-scroll-mode
	{ .i64 = 500 },				// keyseq-timeout
	{ .u64 = 1 },				// mark-directories
	{ .u64 = 0 },				// mark-symlinked directories
	{ .u64 = 0 },				// match-hidden-files
	{ .u64 = 1 },				// page-completions
	{ .u64 = 0 },				// search-ignore-case
	{ .u64 = 0 },				// visible-stats
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
