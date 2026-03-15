// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<keybind_test.c>>

#include <stdio.h>
#include <unistd.h>

#include "rl42.h"
#include "function.h"

#include "internal/_utils.h"
#include "internal/test/defs.h"

#define _FN_1_NAME	"fn-1"
static inline u8	_fn_1([[maybe_unused]] rl42_line *x) { return 1; }
#define _FN_2_NAME	"fn-2"
static inline u8	_fn_2([[maybe_unused]] rl42_line *x) { return 1; }

i32	main(void) {
	i32	rv;
	u8	_rv;

	if (!rl42_register_function(_fn_1, _FN_1_NAME) || !rl42_register_function(_fn_2, _FN_2_NAME)) {
		error("Unable to register functions\n");
		return 1;
	}
	rv = 0;
	rl42_unbind("m", RL42_EM_CURRENT);
	info("Test 1 ---- valid bindings\n");
	_rv = rl42_bind("<C-x><C-a>a", _FN_1_NAME, RL42_BM_WARN, RL42_EM_CURRENT);
	if (!_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(<C-x><C-a>a, " _FN_1_NAME ", RL42_BM_WARN, RL42_EM_CURRENT): %s" ENDL, hl(_rv), (_rv) ? "OK" : "KO");
	_rv = rl42_bind("m", _FN_2_NAME, RL42_BM_WARN, RL42_EM_CURRENT);
	if (!_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(m, " _FN_2_NAME ", RL42_BM_WARN, RL42_EM_CURRENT): %s" ENDL, hl(_rv), (_rv) ? "OK" : "KO");
	info("Test 2 ---- invalid bindings\n");
	_rv = rl42_bind("<HELLO>", _FN_1_NAME, RL42_BM_WARN, RL42_EM_EMACS);
	if (_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(<HELLO>, " _FN_1_NAME ", RL42_BM_WARN, RL42_EM_EMACS): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	_rv = rl42_bind("<C-e>", "cool-function", RL42_BM_WARN, RL42_EM_VI_CMD);
	if (_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(<C-e>, cool-function, RL42_BM_WARN, RL42_EM_VI_CMD): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	_rv = rl42_bind("<C-x><C-r", _FN_2_NAME, RL42_BM_WARN, RL42_EM_VI_INS);
	if (_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(<C-x><C-r, " _FN_2_NAME ", RL42_BM_WARN, RL42_EM_VI_INS): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	info("Test 3 ---- remap\n");
	_rv = rl42_bind("m", _FN_2_NAME, RL42_BM_WARN, RL42_EM_CURRENT);
	if (_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(m, " _FN_2_NAME ", RL42_BM_WARN, RL42_EM_CURRENT): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	_rv = rl42_bind("m", _FN_2_NAME, RL42_BM_REMAP, RL42_EM_CURRENT);
	if (!_rv)
		rv = 1;
	fprintf(stderr, "%srl42_bind(m, " _FN_2_NAME ", RL42_BM_REMAP, RL42_EM_CURRENT): %s" ENDL, hl(_rv), (_rv) ? "OK" : "KO");
	rl42_cleanup();
	return rv;
}
