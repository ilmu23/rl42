// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<keybind_test.c>>

#include <stdio.h>

#include "internal/_function.h"
#include "internal/_keybinds.h"
#include "internal/test/defs.h"

#define _FN_1_NAME	"fn-1"
static inline u8	_fn_1([[maybe_unused]] void *x) { return 1; }
#define _FN_2_NAME	"fn-2"
static inline u8	_fn_2([[maybe_unused]] void *x) { return 1; }

i32	main(void) {
	i32	rv;
	u8	_rv;

	if (!rl42_register_function(_fn_1, _FN_1_NAME) || !rl42_register_function(_fn_2, _FN_2_NAME)) {
		error("Unable to register functions\n");
		return 1;
	}
	rv = 0;
	// SOMEHOW DISABLE STDERR FOR THE DURATION OF RL42_INIT
	rl42_init();
	info("Test 1 ---- valid bindings\n");
	_rv = rl42_bind("<C-x><C-a>a", _FN_1_NAME, WARN, CURRENT);
	if (!_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(<C-x><C-a>a, " _FN_1_NAME ", WARN, CURRENT): %s" ENDL, hl(_rv), (_rv) ? "OK" : "KO");
	_rv = rl42_bind("m", _FN_2_NAME, WARN, CURRENT);
	if (!_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(m, " _FN_2_NAME ", WARN, CURRENT): %s" ENDL, hl(_rv), (_rv) ? "OK" : "KO");
	info("Test 2 ---- invalid bindings\n");
	_rv = rl42_bind("<HELLO>", _FN_1_NAME, WARN, EMACS);
	if (_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(<HELLO>, " _FN_1_NAME ", WARN, EMACS): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	_rv = rl42_bind("<C-e>", "cool-function", WARN, VI_CMD);
	if (_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(<C-e>, cool-function, WARN, VI_CMD): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	_rv = rl42_bind("<C-x><C-r", _FN_2_NAME, WARN, VI_INS);
	if (_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(<C-x><C-r, " _FN_2_NAME ", WARN, VI_INS): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	info("Test 3 ---- remap\n");
	_rv = rl42_bind("m", _FN_1_NAME, WARN, CURRENT);
	if (_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(m, " _FN_1_NAME ", WARN, CURRENT): %s" ENDL, hl(!_rv), (!_rv) ? "OK" : "KO");
	_rv = rl42_bind("m", _FN_1_NAME, REMAP, CURRENT);
	if (!_rv)
		rv = 0;
	fprintf(stderr, "%srl42_bind(m, " _FN_1_NAME ", REMAP, CURRENT): %s" ENDL, hl(_rv), (_rv) ? "OK" : "KO");
	return rv;
}
