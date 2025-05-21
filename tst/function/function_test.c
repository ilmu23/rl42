// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<function_test.c>>

#include <stdio.h>

#include "internal/_function.h"
#include "internal/test/defs.h"

#define _FN_1_NAME	"fn-1"
static inline u8	_fn_1([[maybe_unused]] void *x) { return 1; }
#define _FN_2_NAME	"fn-2"
static inline u8	_fn_2([[maybe_unused]] void *x) { return 2; }
#define _FN_3_NAME	"fn-3"
static inline u8	_fn_3([[maybe_unused]] void *x) { return 3; }
#define _FN_4_NAME	"fn-4"
static inline u8	_fn_4([[maybe_unused]] void *x) { return 4; }
static inline u8	_fn_5([[maybe_unused]] void *x) { return 4; }

#define _FN_COUNT 4

static inline u8	_print_fn_info(const rl42_fn_info *fn_info1, const rl42_fn_info *fn_info2, const u8 check, const size_t i);

static const struct {
	const rl42_fn	f;
	const char		*fname;
}	functions[] = {
	{.f = _fn_1, .fname = _FN_1_NAME},
	{.f = _fn_2, .fname = _FN_2_NAME},
	{.f = _fn_3, .fname = _FN_3_NAME},
	{.f = _fn_4, .fname = _FN_4_NAME},
};

i32	main(void) {
	rl42_fn_info	*fn_info1;
	rl42_fn_info	*fn_info2;
	size_t			i;
	i32				rv;
	u8				check;

	rv = 0;
	info("Test 1 ---- fn_info on non-registered functions\n");
	fn_info1 = get_fn_info(_fn_1);
	if (fn_info1 != NULL)
		rv = 1;
	fprintf(stderr, "%sget_fn_info(_fn_1): %p, expected %p" ENDL, hl(fn_info1 == NULL), (void *)fn_info1, NULL);
	fn_info1 = get_fn_info_name(_FN_1_NAME);
	if (fn_info1 != NULL)
		rv = 1;
	fprintf(stderr, "%sget_fn_info_name(" _FN_1_NAME "): %p, expected %p" ENDL, hl(fn_info1 == NULL), (void *)fn_info1, NULL);
	for (i = 0; i < _FN_COUNT; i++) {
		if (!rl42_register_function(functions[i].f, functions[i].fname)) {
			error("Failed to register function %s: %s\n", functions[i].fname, strerror(errno));
			rv = 1;
		}
	}
	info("TEST 2 ---- fn_info on registered functions\n");
	for (i = 0; i < _FN_COUNT; i++) {
		fn_info1 = get_fn_info(functions[i].f);
		fn_info2 = get_fn_info_name(functions[i].fname);
		check = (fn_info1 && fn_info1 == fn_info2) ? 1 : 0;
		if (!_print_fn_info(fn_info1, fn_info2, check, i))
			rv = 1;
	}
	info("TEST 3 ---- rl42_register_function with already registered function / name\n");
	if (rl42_register_function(_fn_1, "fn-5"))
		rv = 1;
	if (rl42_register_function(_fn_5, "fn-1"))
		rv = 1;
	if (rl42_register_function(_fn_3, "fn-3"))
		rv = 1;
	return rv;
}

static inline u8	_print_fn_info(const rl42_fn_info *fn_info1, const rl42_fn_info *fn_info2, const u8 check, const size_t i) {
	u8	_check;
	u8	rv;

	rv = 1;
	if (check) {
		fprintf(stderr, SGR_OK "get_fn_info(_fn_%zu) == get_fn_info_name(%s)" ENDL, i + 1, functions[i].fname);
		_check = (fn_info1->f == functions[i].f) ? 1 : 0;
		if (!_check)
			rv = 0;
		fprintf(stderr, "%s\tfunction address: %s" ENDL, hl(_check), (_check) ? "OK" : "KO");
		_check = (fn_info1->fname == functions[i].fname) ? 1 : 0;
		if (!_check)
			rv = 0;
		fprintf(stderr, "%s\tfunction name: '%s', expected '%s'" ENDL, hl(_check), fn_info1->fname, functions[i].fname);
	} else {
		_check = (fn_info1 && fn_info1->f == functions[i].f && fn_info1->fname == functions[i].fname) ? 1 : 0;
		fprintf(stderr, "%sget_fn_info(_fn_%zu): %s" ENDL, hl(_check), i + 1, (_check) ? "OK" : "KO");
		_check = (fn_info2 && fn_info2->f == functions[i].f && fn_info2->fname == functions[i].fname) ? 1 : 0;
		fprintf(stderr, "%sget_fn_info_name(%s): %s" ENDL, hl(_check), functions[i].fname, (_check) ? "OK" : "KO");
	}
	return (check) ? rv : 0;
}
