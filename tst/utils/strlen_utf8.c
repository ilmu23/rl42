// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<test.c>>

#include <stdio.h>

#include "internal/_utils.h"
#include "internal/test/defs.h"

#define S1	"ööödwadaälawdhÄÄÄdwadal"
#define S2	"Hello there"
#define S3	"藤原とうふ店 (自家用)"
#define S4	"ちょっと横槍を刺させてもらうのん。今お兄ちゃんがLinuxと呼んでいる物は実は正しく言うとGNU/Linuxって言うのん。最近はGNUプラスLinuxとも言うのん。Linux単体は実はOSじゃなくて、GNUと言う完成されたシステムのフリーコンポーネントの一つとしてGNUのcorelib、shellユティリティ、その他バイタルシステムと一緒にPOSIXの定義上OSとして実装可能になってるん。パソコンの利用者は皆知らずい毎日改変されたGNUシステムを使ってるのん。とある経緯を経て、昨今よく使われるGNUがLinuxと呼ばれるようになっているのん。なのに、GNU Projectに作られたGNUシステムと知らずにいるのん。Linux自体は実際存在していて、皆も使ってるん。ただ、それはシステムの一部としてなのん。Linuxはkernalと言って、他のプログラムにパソコンのリソースを振り分ける役目を持つのん。KernalはOSの大事な一部だけど、単体では意味が無いのん。完成されたシステムの一部の中でしか機能できないのん。Linuxはよく、GNU OSの一部と一緒に使われるのん、いや、GNU/Linuxなのん。Linuxデストリは実は皆GNU/Linuxのデストリなのん!"

#define L1	23
#define L2	11
#define L3	12
#define L4	537

#define check(result, expected)	(result == expected)

i32	main(void) {
	size_t	l1;
	size_t	l2;
	size_t	l3;
	size_t	l4;

	l1 = strlen_utf8(S1);
	l2 = strlen_utf8(S2);
	l3 = strlen_utf8(S3);
	l4 = strlen_utf8(S4);
	fprintf(stderr, "%sstrlen_utf8(%s): %zu" ENDL, hl(check(l1, L1)), S1, l1);
	fprintf(stderr, "%sstrlen_utf8(%s): %zu" ENDL, hl(check(l2, L2)), S2, l2);
	fprintf(stderr, "%sstrlen_utf8(%s): %zu" ENDL, hl(check(l3, L3)), S3, l3);
	fprintf(stderr, "%sstrlen_utf8(%s): %zu" ENDL, hl(check(l4, L4)), S4, l4);
	if (l1 != L1 || l2 != L2 || l3 != L3 || l4 != L4)
		return 1;
	return 0;
}
