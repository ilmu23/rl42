// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<misc.h>>

#pragma once

#include "function.h"

#ifdef __DUMP_FN
#define DUMP_BUFFER_SIZE	4096

ssize_t	add_to_dump_buf(const char *fmt, ...);
ssize_t	flush_dump_buf(void);
#endif

rl42_fn(backward_char_search);
rl42_fn(dump_functions);
rl42_fn(dump_macros);
rl42_fn(dump_variables);
rl42_fn(emacs_editing_mode);
rl42_fn(exchange_point_and_mark);
rl42_fn(forward_char_search);
rl42_fn(numeric_argument);
rl42_fn(prefix_meta);
rl42_fn(reload_config_file);
rl42_fn(revert_line);
rl42_fn(rl42_abort);
rl42_fn(set_mark);
rl42_fn(tilde_expand);
rl42_fn(unset_mark);
rl42_fn(vi_editing_mode);
