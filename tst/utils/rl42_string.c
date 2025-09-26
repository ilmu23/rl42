// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<rl42_string.c>>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/_utils.h"
#include "internal/_vector.h"
#include "internal/test/defs.h"

#define STR_COUNT	4

#define S1	"ööödwadaälawdhÄÄÄdwadal"
#define S2	"Hello there"
#define S3	"藤原とうふ店 (自家用)"
#define S4	"ちょっと横槍を刺させてもらうのん。今お兄ちゃんがLinuxと呼んでいる物は実は正しく言うとGNU/Linuxって言うのん。最近はGNUプラスLinuxとも言うのん。Linux単体は実はOSじゃなくて、GNUと言う完成されたシステムのフリーコンポーネントの一つとしてGNUのcorelib、shellユティリティ、その他バイタルシステムと一緒にPOSIXの定義上OSとして実装可能になってるん。パソコンの利用者は皆知らずい毎日改変されたGNUシステムを使ってるのん。とある経緯を経て、昨今よく使われるGNUがLinuxと呼ばれるようになっているのん。なのに、GNU Projectに作られたGNUシステムと知らずにいるのん。Linux自体は実際存在していて、皆も使ってるん。ただ、それはシステムの一部としてなのん。Linuxはkernalと言って、他のプログラムにパソコンのリソースを振り分ける役目を持つのん。KernalはOSの大事な一部だけど、単体では意味が無いのん。完成されたシステムの一部の中でしか機能できないのん。Linuxはよく、GNU OSの一部と一緒に使われるのん、いや、GNU/Linuxなのん。Linuxデストリは実は皆GNU/Linuxのデストリなのん!"

#define L1	23
#define L2	11
#define L3	12
#define L4	537

#define r1	rstrings[0]
#define r2	rstrings[1]
#define r3	rstrings[2]
#define r4	rstrings[3]

#define rstr_check(rstr, exp_len)					(vector_size(rstr) == exp_len)
#define cstr_check(cstr, exp_str, exp_len, exp_sze)	(memcmp(cstr, exp_str, exp_sze) == 0 \
													 && strlen_utf8(cstr) == exp_len \
													 && strlen(cstr) == exp_sze)

#define c1	cstrings[0]
#define c2	cstrings[1]
#define c3	cstrings[2]
#define c4	cstrings[3]

const size_t		strsizes[STR_COUNT] = {sizeof(S1) - 1, sizeof(S2) - 1, sizeof(S3) - 1, sizeof(S4) - 1};
const size_t		strlengths[STR_COUNT] = {L1, L2, L3, L4};
const char			*strings[STR_COUNT] = {S1, S2, S3, S4};

i32	main(void) {
	const char	*cstrings[STR_COUNT];
	vector		rstrings[STR_COUNT];
	size_t		i;
	size_t		j;
	size_t		len;
	char		buf[5];
	i32			rv;

	rv = 0;
	r1 = cstr_to_rl42str(S1);
	r2 = cstr_to_rl42str(S2);
	r3 = cstr_to_rl42str(S3);
	r4 = cstr_to_rl42str(S4);
	for (i = 0; i < STR_COUNT; i++) {
		if (!rstr_check(rstrings[i], strlengths[i]))
			rv = 1;
		fprintf(stderr, "%scstr_to_rl42str(S%zu): \"", hl(rstr_check(rstrings[i], strlengths[i])), i + 1);
		for (j = 0, len = vector_size(rstrings[i]); j < len; j++) {
			utf8_encode(*(u32 *)vector_get(rstrings[i], j), buf);
			fputs(buf, stderr);
		}
		fputs("\"" ENDL, stderr);
	}
	c1 = rl42str_to_cstr(r1);
	c2 = rl42str_to_cstr(r2);
	c3 = rl42str_to_cstr(r3);
	c4 = rl42str_to_cstr(r4);
	for (i = 0; i < STR_COUNT; i++) {
		if (!cstr_check(cstrings[i], strings[i], strlengths[i], strsizes[i]))
			rv = 1;
		fprintf(stderr, "%srl42str_to_cstr(r%zu): \"%s\"" ENDL,
				hl(cstr_check(cstrings[i], strings[i], strlengths[i], strsizes[i])),
				i + 1, cstrings[i]);
		vector_delete(rstrings[i]);
		free((void *)cstrings[i]);
	}
	return rv;
}
