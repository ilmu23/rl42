// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<terminfo.c>>

#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <linux/limits.h>

#include "internal/_utils.h"
#include "internal/_terminfo.h"
#include "internal/test/defs.h"

typedef struct {
	const char	*name_str;
	u16			name;
}	cap;

extern char	**environ;

char	*(*rl42_getenv)(const char *) = getenv;

static const cap	bool_caps[TI42_BOOLEAN_CAPS] = {
	{ .name_str = "OTMT", .name = ti42_OTMT },
	{ .name_str = "OTNL", .name = ti42_OTNL },
	{ .name_str = "OTXR", .name = ti42_OTXR },
	{ .name_str = "OTbs", .name = ti42_OTbs },
	{ .name_str = "OTnc", .name = ti42_OTnc },
	{ .name_str = "OTns", .name = ti42_OTns },
	{ .name_str = "OTpt", .name = ti42_OTpt },
	{ .name_str = "am", .name = ti42_am },
	{ .name_str = "bce", .name = ti42_bce },
	{ .name_str = "bw", .name = ti42_bw },
	{ .name_str = "ccc", .name = ti42_ccc },
	{ .name_str = "chts", .name = ti42_chts },
	{ .name_str = "cpix", .name = ti42_cpix },
	{ .name_str = "crxm", .name = ti42_crxm },
	{ .name_str = "da", .name = ti42_da },
	{ .name_str = "daisy", .name = ti42_daisy },
	{ .name_str = "db", .name = ti42_db },
	{ .name_str = "eo", .name = ti42_eo },
	{ .name_str = "eslok", .name = ti42_eslok },
	{ .name_str = "gn", .name = ti42_gn },
	{ .name_str = "hc", .name = ti42_hc },
	{ .name_str = "hls", .name = ti42_hls },
	{ .name_str = "hs", .name = ti42_hs },
	{ .name_str = "hz", .name = ti42_hz },
	{ .name_str = "in", .name = ti42_in },
	{ .name_str = "km", .name = ti42_km },
	{ .name_str = "lpix", .name = ti42_lpix },
	{ .name_str = "mc5i", .name = ti42_mc5i },
	{ .name_str = "mir", .name = ti42_mir },
	{ .name_str = "msgr", .name = ti42_msgr },
	{ .name_str = "ndscr", .name = ti42_ndscr },
	{ .name_str = "npc", .name = ti42_npc },
	{ .name_str = "nrrmc", .name = ti42_nrrmc },
	{ .name_str = "nxon", .name = ti42_nxon },
	{ .name_str = "os", .name = ti42_os },
	{ .name_str = "sam", .name = ti42_sam },
	{ .name_str = "ul", .name = ti42_ul },
	{ .name_str = "xenl", .name = ti42_xenl },
	{ .name_str = "xhp", .name = ti42_xhp },
	{ .name_str = "xhpa", .name = ti42_xhpa },
	{ .name_str = "xon", .name = ti42_xon },
	{ .name_str = "xsb", .name = ti42_xsb },
	{ .name_str = "xt", .name = ti42_xt },
	{ .name_str = "xvpa", .name = ti42_xvpa },
};

static const cap	num_caps[TI42_NUMERIC_CAPS] = {
	{ .name_str = "OTdB", .name = ti42_OTdB },
	{ .name_str = "OTdC", .name = ti42_OTdC },
	{ .name_str = "OTdN", .name = ti42_OTdN },
	{ .name_str = "OTdT", .name = ti42_OTdT },
	{ .name_str = "OTkn", .name = ti42_OTkn },
	{ .name_str = "OTug", .name = ti42_OTug },
	{ .name_str = "bitwin", .name = ti42_bitwin },
	{ .name_str = "bitype", .name = ti42_bitype },
	{ .name_str = "btns", .name = ti42_btns },
	{ .name_str = "bufsz", .name = ti42_bufsz },
	{ .name_str = "colors", .name = ti42_colors },
	{ .name_str = "cols", .name = ti42_cols },
	{ .name_str = "cps", .name = ti42_cps },
	{ .name_str = "it", .name = ti42_it },
	{ .name_str = "lh", .name = ti42_lh },
	{ .name_str = "lines", .name = ti42_lines },
	{ .name_str = "lm", .name = ti42_lm },
	{ .name_str = "lw", .name = ti42_lw },
	{ .name_str = "ma", .name = ti42_ma },
	{ .name_str = "maddr", .name = ti42_maddr },
	{ .name_str = "mcs", .name = ti42_mcs },
	{ .name_str = "mjump", .name = ti42_mjump },
	{ .name_str = "mls", .name = ti42_mls },
	{ .name_str = "ncv", .name = ti42_ncv },
	{ .name_str = "nlab", .name = ti42_nlab },
	{ .name_str = "npins", .name = ti42_npins },
	{ .name_str = "orc", .name = ti42_orc },
	{ .name_str = "orhi", .name = ti42_orhi },
	{ .name_str = "orl", .name = ti42_orl },
	{ .name_str = "orvi", .name = ti42_orvi },
	{ .name_str = "pairs", .name = ti42_pairs },
	{ .name_str = "pb", .name = ti42_pb },
	{ .name_str = "spinh", .name = ti42_spinh },
	{ .name_str = "spinv", .name = ti42_spinv },
	{ .name_str = "vt", .name = ti42_vt },
	{ .name_str = "widcs", .name = ti42_widcs },
	{ .name_str = "wnum", .name = ti42_wnum },
	{ .name_str = "wsl", .name = ti42_wsl },
	{ .name_str = "xmc", .name = ti42_xmc },
};

static const cap str_caps[TI42_STRING_CAPS] = {
	{ .name_str = "acsc", .name = ti42_acsc },
	{ .name_str = "bel", .name = ti42_bel },
	{ .name_str = "bicr", .name = ti42_bicr },
	{ .name_str = "binel", .name = ti42_binel },
	{ .name_str = "birep", .name = ti42_birep },
	{ .name_str = "blink", .name = ti42_blink },
	{ .name_str = "bold", .name = ti42_bold },
	{ .name_str = "cbt", .name = ti42_cbt },
	{ .name_str = "chr", .name = ti42_chr },
	{ .name_str = "civis", .name = ti42_civis },
	{ .name_str = "clear", .name = ti42_clear },
	{ .name_str = "cmdch", .name = ti42_cmdch },
	{ .name_str = "cnorm", .name = ti42_cnorm },
	{ .name_str = "colornm", .name = ti42_colornm },
	{ .name_str = "cpi", .name = ti42_cpi },
	{ .name_str = "cr", .name = ti42_cr },
	{ .name_str = "csin", .name = ti42_csin },
	{ .name_str = "csnm", .name = ti42_csnm },
	{ .name_str = "csr", .name = ti42_csr },
	{ .name_str = "cub", .name = ti42_cub },
	{ .name_str = "cub1", .name = ti42_cub1 },
	{ .name_str = "cud", .name = ti42_cud },
	{ .name_str = "cud1", .name = ti42_cud1 },
	{ .name_str = "cuf", .name = ti42_cuf },
	{ .name_str = "cuf1", .name = ti42_cuf1 },
	{ .name_str = "cup", .name = ti42_cup },
	{ .name_str = "cuu", .name = ti42_cuu },
	{ .name_str = "cuu1", .name = ti42_cuu1 },
	{ .name_str = "cvr", .name = ti42_cvr },
	{ .name_str = "cvvis", .name = ti42_cvvis },
	{ .name_str = "cwin", .name = ti42_cwin },
	{ .name_str = "dch", .name = ti42_dch },
	{ .name_str = "dch1", .name = ti42_dch1 },
	{ .name_str = "dclk", .name = ti42_dclk },
	{ .name_str = "defbi", .name = ti42_defbi },
	{ .name_str = "defc", .name = ti42_defc },
	{ .name_str = "devt", .name = ti42_devt },
	{ .name_str = "dial", .name = ti42_dial },
	{ .name_str = "dim", .name = ti42_dim },
	{ .name_str = "dispc", .name = ti42_dispc },
	{ .name_str = "dl", .name = ti42_dl },
	{ .name_str = "dl1", .name = ti42_dl1 },
	{ .name_str = "docr", .name = ti42_docr },
	{ .name_str = "dsl", .name = ti42_dsl },
	{ .name_str = "ech", .name = ti42_ech },
	{ .name_str = "ed", .name = ti42_ed },
	{ .name_str = "ehhlm", .name = ti42_ehhlm },
	{ .name_str = "el", .name = ti42_el },
	{ .name_str = "el1", .name = ti42_el1 },
	{ .name_str = "elhlm", .name = ti42_elhlm },
	{ .name_str = "elohlm", .name = ti42_elohlm },
	{ .name_str = "enacs", .name = ti42_enacs },
	{ .name_str = "endbi", .name = ti42_endbi },
	{ .name_str = "erhlm", .name = ti42_erhlm },
	{ .name_str = "ethlm", .name = ti42_ethlm },
	{ .name_str = "evhlm", .name = ti42_evhlm },
	{ .name_str = "ff", .name = ti42_ff },
	{ .name_str = "flash", .name = ti42_flash },
	{ .name_str = "fln", .name = ti42_fln },
	{ .name_str = "fsl", .name = ti42_fsl },
	{ .name_str = "getm", .name = ti42_getm },
	{ .name_str = "hd", .name = ti42_hd },
	{ .name_str = "home", .name = ti42_home },
	{ .name_str = "hook", .name = ti42_hook },
	{ .name_str = "hpa", .name = ti42_hpa },
	{ .name_str = "ht", .name = ti42_ht },
	{ .name_str = "hts", .name = ti42_hts },
	{ .name_str = "hu", .name = ti42_hu },
	{ .name_str = "hup", .name = ti42_hup },
	{ .name_str = "ich", .name = ti42_ich },
	{ .name_str = "ich1", .name = ti42_ich1 },
	{ .name_str = "if", .name = ti42_if },
	{ .name_str = "il", .name = ti42_il },
	{ .name_str = "il1", .name = ti42_il1 },
	{ .name_str = "ind", .name = ti42_ind },
	{ .name_str = "indn", .name = ti42_indn },
	{ .name_str = "initc", .name = ti42_initc },
	{ .name_str = "initp", .name = ti42_initp },
	{ .name_str = "invis", .name = ti42_invis },
	{ .name_str = "ip", .name = ti42_ip },
	{ .name_str = "iprog", .name = ti42_iprog },
	{ .name_str = "is1", .name = ti42_is1 },
	{ .name_str = "is2", .name = ti42_is2 },
	{ .name_str = "is3", .name = ti42_is3 },
	{ .name_str = "kBEG", .name = ti42_kBEG },
	{ .name_str = "kCAN", .name = ti42_kCAN },
	{ .name_str = "kCMD", .name = ti42_kCMD },
	{ .name_str = "kCPY", .name = ti42_kCPY },
	{ .name_str = "kCRT", .name = ti42_kCRT },
	{ .name_str = "kDC", .name = ti42_kDC },
	{ .name_str = "kDL", .name = ti42_kDL },
	{ .name_str = "kEND", .name = ti42_kEND },
	{ .name_str = "kEOL", .name = ti42_kEOL },
	{ .name_str = "kEXT", .name = ti42_kEXT },
	{ .name_str = "kFND", .name = ti42_kFND },
	{ .name_str = "kHLP", .name = ti42_kHLP },
	{ .name_str = "kHOM", .name = ti42_kHOM },
	{ .name_str = "kIC", .name = ti42_kIC },
	{ .name_str = "kLFT", .name = ti42_kLFT },
	{ .name_str = "kMOV", .name = ti42_kMOV },
	{ .name_str = "kMSG", .name = ti42_kMSG },
	{ .name_str = "kNXT", .name = ti42_kNXT },
	{ .name_str = "kOPT", .name = ti42_kOPT },
	{ .name_str = "kPRT", .name = ti42_kPRT },
	{ .name_str = "kPRV", .name = ti42_kPRV },
	{ .name_str = "kRDO", .name = ti42_kRDO },
	{ .name_str = "kRES", .name = ti42_kRES },
	{ .name_str = "kRIT", .name = ti42_kRIT },
	{ .name_str = "kRPL", .name = ti42_kRPL },
	{ .name_str = "kSAV", .name = ti42_kSAV },
	{ .name_str = "kSPD", .name = ti42_kSPD },
	{ .name_str = "kUND", .name = ti42_kUND },
	{ .name_str = "ka1", .name = ti42_ka1 },
	{ .name_str = "ka3", .name = ti42_ka3 },
	{ .name_str = "kb2", .name = ti42_kb2 },
	{ .name_str = "kbeg", .name = ti42_kbeg },
	{ .name_str = "kbs", .name = ti42_kbs },
	{ .name_str = "kc1", .name = ti42_kc1 },
	{ .name_str = "kc3", .name = ti42_kc3 },
	{ .name_str = "kcan", .name = ti42_kcan },
	{ .name_str = "kcbt", .name = ti42_kcbt },
	{ .name_str = "kclo", .name = ti42_kclo },
	{ .name_str = "kclr", .name = ti42_kclr },
	{ .name_str = "kcmd", .name = ti42_kcmd },
	{ .name_str = "kcpy", .name = ti42_kcpy },
	{ .name_str = "kcrt", .name = ti42_kcrt },
	{ .name_str = "kctab", .name = ti42_kctab },
	{ .name_str = "kcub1", .name = ti42_kcub1 },
	{ .name_str = "kcud1", .name = ti42_kcud1 },
	{ .name_str = "kcuf1", .name = ti42_kcuf1 },
	{ .name_str = "kcuu1", .name = ti42_kcuu1 },
	{ .name_str = "kdch1", .name = ti42_kdch1 },
	{ .name_str = "kdl1", .name = ti42_kdl1 },
	{ .name_str = "ked", .name = ti42_ked },
	{ .name_str = "kel", .name = ti42_kel },
	{ .name_str = "kend", .name = ti42_kend },
	{ .name_str = "kent", .name = ti42_kent },
	{ .name_str = "kext", .name = ti42_kext },
	{ .name_str = "kf0", .name = ti42_kf0 },
	{ .name_str = "kf1", .name = ti42_kf1 },
	{ .name_str = "kf10", .name = ti42_kf10 },
	{ .name_str = "kf11", .name = ti42_kf11 },
	{ .name_str = "kf12", .name = ti42_kf12 },
	{ .name_str = "kf13", .name = ti42_kf13 },
	{ .name_str = "kf14", .name = ti42_kf14 },
	{ .name_str = "kf15", .name = ti42_kf15 },
	{ .name_str = "kf16", .name = ti42_kf16 },
	{ .name_str = "kf17", .name = ti42_kf17 },
	{ .name_str = "kf18", .name = ti42_kf18 },
	{ .name_str = "kf19", .name = ti42_kf19 },
	{ .name_str = "kf2", .name = ti42_kf2 },
	{ .name_str = "kf20", .name = ti42_kf20 },
	{ .name_str = "kf21", .name = ti42_kf21 },
	{ .name_str = "kf22", .name = ti42_kf22 },
	{ .name_str = "kf23", .name = ti42_kf23 },
	{ .name_str = "kf24", .name = ti42_kf24 },
	{ .name_str = "kf25", .name = ti42_kf25 },
	{ .name_str = "kf26", .name = ti42_kf26 },
	{ .name_str = "kf27", .name = ti42_kf27 },
	{ .name_str = "kf28", .name = ti42_kf28 },
	{ .name_str = "kf29", .name = ti42_kf29 },
	{ .name_str = "kf3", .name = ti42_kf3 },
	{ .name_str = "kf30", .name = ti42_kf30 },
	{ .name_str = "kf31", .name = ti42_kf31 },
	{ .name_str = "kf32", .name = ti42_kf32 },
	{ .name_str = "kf33", .name = ti42_kf33 },
	{ .name_str = "kf34", .name = ti42_kf34 },
	{ .name_str = "kf35", .name = ti42_kf35 },
	{ .name_str = "kf36", .name = ti42_kf36 },
	{ .name_str = "kf37", .name = ti42_kf37 },
	{ .name_str = "kf38", .name = ti42_kf38 },
	{ .name_str = "kf39", .name = ti42_kf39 },
	{ .name_str = "kf4", .name = ti42_kf4 },
	{ .name_str = "kf40", .name = ti42_kf40 },
	{ .name_str = "kf41", .name = ti42_kf41 },
	{ .name_str = "kf42", .name = ti42_kf42 },
	{ .name_str = "kf43", .name = ti42_kf43 },
	{ .name_str = "kf44", .name = ti42_kf44 },
	{ .name_str = "kf45", .name = ti42_kf45 },
	{ .name_str = "kf46", .name = ti42_kf46 },
	{ .name_str = "kf47", .name = ti42_kf47 },
	{ .name_str = "kf48", .name = ti42_kf48 },
	{ .name_str = "kf49", .name = ti42_kf49 },
	{ .name_str = "kf5", .name = ti42_kf5 },
	{ .name_str = "kf50", .name = ti42_kf50 },
	{ .name_str = "kf51", .name = ti42_kf51 },
	{ .name_str = "kf52", .name = ti42_kf52 },
	{ .name_str = "kf53", .name = ti42_kf53 },
	{ .name_str = "kf54", .name = ti42_kf54 },
	{ .name_str = "kf55", .name = ti42_kf55 },
	{ .name_str = "kf56", .name = ti42_kf56 },
	{ .name_str = "kf57", .name = ti42_kf57 },
	{ .name_str = "kf58", .name = ti42_kf58 },
	{ .name_str = "kf59", .name = ti42_kf59 },
	{ .name_str = "kf6", .name = ti42_kf6 },
	{ .name_str = "kf60", .name = ti42_kf60 },
	{ .name_str = "kf61", .name = ti42_kf61 },
	{ .name_str = "kf62", .name = ti42_kf62 },
	{ .name_str = "kf63", .name = ti42_kf63 },
	{ .name_str = "kf7", .name = ti42_kf7 },
	{ .name_str = "kf8", .name = ti42_kf8 },
	{ .name_str = "kf9", .name = ti42_kf9 },
	{ .name_str = "kfnd", .name = ti42_kfnd },
	{ .name_str = "khlp", .name = ti42_khlp },
	{ .name_str = "khome", .name = ti42_khome },
	{ .name_str = "khts", .name = ti42_khts },
	{ .name_str = "kich1", .name = ti42_kich1 },
	{ .name_str = "kil1", .name = ti42_kil1 },
	{ .name_str = "kind", .name = ti42_kind },
	{ .name_str = "kll", .name = ti42_kll },
	{ .name_str = "kmous", .name = ti42_kmous },
	{ .name_str = "kmov", .name = ti42_kmov },
	{ .name_str = "kmrk", .name = ti42_kmrk },
	{ .name_str = "kmsg", .name = ti42_kmsg },
	{ .name_str = "knp", .name = ti42_knp },
	{ .name_str = "knxt", .name = ti42_knxt },
	{ .name_str = "kopn", .name = ti42_kopn },
	{ .name_str = "kopt", .name = ti42_kopt },
	{ .name_str = "kpp", .name = ti42_kpp },
	{ .name_str = "kprt", .name = ti42_kprt },
	{ .name_str = "kprv", .name = ti42_kprv },
	{ .name_str = "krdo", .name = ti42_krdo },
	{ .name_str = "kref", .name = ti42_kref },
	{ .name_str = "kres", .name = ti42_kres },
	{ .name_str = "krfr", .name = ti42_krfr },
	{ .name_str = "kri", .name = ti42_kri },
	{ .name_str = "krmir", .name = ti42_krmir },
	{ .name_str = "krpl", .name = ti42_krpl },
	{ .name_str = "krst", .name = ti42_krst },
	{ .name_str = "ksav", .name = ti42_ksav },
	{ .name_str = "kslt", .name = ti42_kslt },
	{ .name_str = "kspd", .name = ti42_kspd },
	{ .name_str = "ktbc", .name = ti42_ktbc },
	{ .name_str = "kund", .name = ti42_kund },
	{ .name_str = "lf0", .name = ti42_lf0 },
	{ .name_str = "lf1", .name = ti42_lf1 },
	{ .name_str = "lf10", .name = ti42_lf10 },
	{ .name_str = "lf2", .name = ti42_lf2 },
	{ .name_str = "lf3", .name = ti42_lf3 },
	{ .name_str = "lf4", .name = ti42_lf4 },
	{ .name_str = "lf5", .name = ti42_lf5 },
	{ .name_str = "lf6", .name = ti42_lf6 },
	{ .name_str = "lf7", .name = ti42_lf7 },
	{ .name_str = "lf8", .name = ti42_lf8 },
	{ .name_str = "lf9", .name = ti42_lf9 },
	{ .name_str = "ll", .name = ti42_ll },
	{ .name_str = "lpi", .name = ti42_lpi },
	{ .name_str = "mc0", .name = ti42_mc0 },
	{ .name_str = "mc4", .name = ti42_mc4 },
	{ .name_str = "mc5", .name = ti42_mc5 },
	{ .name_str = "mc5p", .name = ti42_mc5p },
	{ .name_str = "mcub", .name = ti42_mcub },
	{ .name_str = "mcub1", .name = ti42_mcub1 },
	{ .name_str = "mcud", .name = ti42_mcud },
	{ .name_str = "mcud1", .name = ti42_mcud1 },
	{ .name_str = "mcuf", .name = ti42_mcuf },
	{ .name_str = "mcuf1", .name = ti42_mcuf1 },
	{ .name_str = "mcuu", .name = ti42_mcuu },
	{ .name_str = "mcuu1", .name = ti42_mcuu1 },
	{ .name_str = "mgc", .name = ti42_mgc },
	{ .name_str = "mhpa", .name = ti42_mhpa },
	{ .name_str = "minfo", .name = ti42_minfo },
	{ .name_str = "mrcup", .name = ti42_mrcup },
	{ .name_str = "mvpa", .name = ti42_mvpa },
	{ .name_str = "nel", .name = ti42_nel },
	{ .name_str = "oc", .name = ti42_oc },
	{ .name_str = "op", .name = ti42_op },
	{ .name_str = "pad", .name = ti42_pad },
	{ .name_str = "pause", .name = ti42_pause },
	{ .name_str = "pctrm", .name = ti42_pctrm },
	{ .name_str = "pfkey", .name = ti42_pfkey },
	{ .name_str = "pfloc", .name = ti42_pfloc },
	{ .name_str = "pfx", .name = ti42_pfx },
	{ .name_str = "pfxl", .name = ti42_pfxl },
	{ .name_str = "pln", .name = ti42_pln },
	{ .name_str = "porder", .name = ti42_porder },
	{ .name_str = "prot", .name = ti42_prot },
	{ .name_str = "pulse", .name = ti42_pulse },
	{ .name_str = "qdial", .name = ti42_qdial },
	{ .name_str = "rbim", .name = ti42_rbim },
	{ .name_str = "rc", .name = ti42_rc },
	{ .name_str = "rcsd", .name = ti42_rcsd },
	{ .name_str = "rep", .name = ti42_rep },
	{ .name_str = "reqmp", .name = ti42_reqmp },
	{ .name_str = "rev", .name = ti42_rev },
	{ .name_str = "rf", .name = ti42_rf },
	{ .name_str = "rfi", .name = ti42_rfi },
	{ .name_str = "ri", .name = ti42_ri },
	{ .name_str = "rin", .name = ti42_rin },
	{ .name_str = "ritm", .name = ti42_ritm },
	{ .name_str = "rlm", .name = ti42_rlm },
	{ .name_str = "rmacs", .name = ti42_rmacs },
	{ .name_str = "rmam", .name = ti42_rmam },
	{ .name_str = "rmclk", .name = ti42_rmclk },
	{ .name_str = "rmcup", .name = ti42_rmcup },
	{ .name_str = "rmdc", .name = ti42_rmdc },
	{ .name_str = "rmicm", .name = ti42_rmicm },
	{ .name_str = "rmir", .name = ti42_rmir },
	{ .name_str = "rmkx", .name = ti42_rmkx },
	{ .name_str = "rmln", .name = ti42_rmln },
	{ .name_str = "rmm", .name = ti42_rmm },
	{ .name_str = "rmp", .name = ti42_rmp },
	{ .name_str = "rmpch", .name = ti42_rmpch },
	{ .name_str = "rmsc", .name = ti42_rmsc },
	{ .name_str = "rmso", .name = ti42_rmso },
	{ .name_str = "rmul", .name = ti42_rmul },
	{ .name_str = "rmxon", .name = ti42_rmxon },
	{ .name_str = "rs1", .name = ti42_rs1 },
	{ .name_str = "rs2", .name = ti42_rs2 },
	{ .name_str = "rs3", .name = ti42_rs3 },
	{ .name_str = "rshm", .name = ti42_rshm },
	{ .name_str = "rsubm", .name = ti42_rsubm },
	{ .name_str = "rsupm", .name = ti42_rsupm },
	{ .name_str = "rum", .name = ti42_rum },
	{ .name_str = "rwidm", .name = ti42_rwidm },
	{ .name_str = "s0ds", .name = ti42_s0ds },
	{ .name_str = "s1ds", .name = ti42_s1ds },
	{ .name_str = "s2ds", .name = ti42_s2ds },
	{ .name_str = "s3ds", .name = ti42_s3ds },
	{ .name_str = "sbim", .name = ti42_sbim },
	{ .name_str = "sc", .name = ti42_sc },
	{ .name_str = "scesa", .name = ti42_scesa },
	{ .name_str = "scesc", .name = ti42_scesc },
	{ .name_str = "sclk", .name = ti42_sclk },
	{ .name_str = "scp", .name = ti42_scp },
	{ .name_str = "scs", .name = ti42_scs },
	{ .name_str = "scsd", .name = ti42_scsd },
	{ .name_str = "sdrfq", .name = ti42_sdrfq },
	{ .name_str = "setab", .name = ti42_setab },
	{ .name_str = "setaf", .name = ti42_setaf },
	{ .name_str = "setb", .name = ti42_setb },
	{ .name_str = "setcolor", .name = ti42_setcolor },
	{ .name_str = "setf", .name = ti42_setf },
	{ .name_str = "sgr", .name = ti42_sgr },
	{ .name_str = "sgr0", .name = ti42_sgr0 },
	{ .name_str = "sgr1", .name = ti42_sgr1 },
	{ .name_str = "sitm", .name = ti42_sitm },
	{ .name_str = "slength", .name = ti42_slength },
	{ .name_str = "slines", .name = ti42_slines },
	{ .name_str = "slm", .name = ti42_slm },
	{ .name_str = "smacs", .name = ti42_smacs },
	{ .name_str = "smam", .name = ti42_smam },
	{ .name_str = "smcup", .name = ti42_smcup },
	{ .name_str = "smdc", .name = ti42_smdc },
	{ .name_str = "smgb", .name = ti42_smgb },
	{ .name_str = "smgbp", .name = ti42_smgbp },
	{ .name_str = "smgl", .name = ti42_smgl },
	{ .name_str = "smglp", .name = ti42_smglp },
	{ .name_str = "smglr", .name = ti42_smglr },
	{ .name_str = "smgr", .name = ti42_smgr },
	{ .name_str = "smgrp", .name = ti42_smgrp },
	{ .name_str = "smgt", .name = ti42_smgt },
	{ .name_str = "smgtb", .name = ti42_smgtb },
	{ .name_str = "smgtp", .name = ti42_smgtp },
	{ .name_str = "smicm", .name = ti42_smicm },
	{ .name_str = "smir", .name = ti42_smir },
	{ .name_str = "smkx", .name = ti42_smkx },
	{ .name_str = "smln", .name = ti42_smln },
	{ .name_str = "smm", .name = ti42_smm },
	{ .name_str = "smpch", .name = ti42_smpch },
	{ .name_str = "smsc", .name = ti42_smsc },
	{ .name_str = "smso", .name = ti42_smso },
	{ .name_str = "smul", .name = ti42_smul },
	{ .name_str = "smxon", .name = ti42_smxon },
	{ .name_str = "snlq", .name = ti42_snlq },
	{ .name_str = "snrmq", .name = ti42_snrmq },
	{ .name_str = "sshm", .name = ti42_sshm },
	{ .name_str = "ssubm", .name = ti42_ssubm },
	{ .name_str = "ssupm", .name = ti42_ssupm },
	{ .name_str = "subcs", .name = ti42_subcs },
	{ .name_str = "sum", .name = ti42_sum },
	{ .name_str = "supcs", .name = ti42_supcs },
	{ .name_str = "swidm", .name = ti42_swidm },
	{ .name_str = "tbc", .name = ti42_tbc },
	{ .name_str = "tone", .name = ti42_tone },
	{ .name_str = "tsl", .name = ti42_tsl },
	{ .name_str = "u0", .name = ti42_u0 },
	{ .name_str = "u1", .name = ti42_u1 },
	{ .name_str = "u2", .name = ti42_u2 },
	{ .name_str = "u3", .name = ti42_u3 },
	{ .name_str = "u4", .name = ti42_u4 },
	{ .name_str = "u5", .name = ti42_u5 },
	{ .name_str = "u6", .name = ti42_u6 },
	{ .name_str = "u7", .name = ti42_u7 },
	{ .name_str = "u8", .name = ti42_u8 },
	{ .name_str = "u9", .name = ti42_u9 },
	{ .name_str = "uc", .name = ti42_uc },
	{ .name_str = "vpa", .name = ti42_vpa },
	{ .name_str = "wait", .name = ti42_wait },
	{ .name_str = "wind", .name = ti42_wind },
	{ .name_str = "wingo", .name = ti42_wingo },
	{ .name_str = "xoffc", .name = ti42_xoffc },
	{ .name_str = "xonc", .name = ti42_xonc },
	{ .name_str = "zerom", .name = ti42_zerom },
};

static inline pid_t	_run_infocmp(const i32 outfile);
static inline void	_dump_caps(const i32 outfile);
static inline void	_dump_file(const char *name, const char *data);
static inline i32	_cmp_output(const i32 tester, const i32 infocmp);

static void	_unlinker(void);

i32	main(void) {
	pid_t	pid;
	i32		stat;
	i32		fd1;
	i32		fd2;

	if (atexit(_unlinker) == -1)
		return 1;
	if (!ti42_load(getenv("TERM")))
		return 1;
	fd1 = open(".tester.out", O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
	fd2 = open(".infocmp.out", O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
	if (fd1 == -1 || fd2 == -1)
		return 1;
	pid = _run_infocmp(fd2);
	if (pid == -1)
		return 1;
	_dump_caps(fd1);
	waitpid(pid, &stat, 0);
	if (!WIFEXITED(stat) || WEXITSTATUS(stat) != 0)
		return 1;
	return _cmp_output(fd1, fd2);
}

static inline pid_t	_run_infocmp(const i32 outfile) {
	pid_t	rv;
	char	*av[3] = {"infocmp", "-1", NULL};
	char	buf[PATH_MAX + 1];
	char	*infocmp;
	char	*start;
	char	*colon;
	char	*path;

	path = getenv("PATH");
	if (!path)
		return -1;
	path = strdup(path);
	if (!path)
		return -1;
	start = path;
	infocmp = NULL;
	do {
		colon = strchr(start, ':');
		if (colon)
			*colon = '\0';
		if (snprintf(buf, PATH_MAX, "%s/infocmp", start) == -1)
			return -1;
		if (access(buf, X_OK) == 0) {
			infocmp = buf;
			break ;
		}
		start = (char *)((uintptr_t)colon + 1);
	} while (colon);
	free(path);
	if (!infocmp)
		return -1;
	rv = fork();
	if (rv == 0) {
		dup2(outfile, 1);
		execve(infocmp, av, environ);
		exit(1);
	}
	return rv;
}

static inline void	_dump_caps(const i32 outfile) {
	const char	*sval;
	size_t		i;
	size_t		j;
	size_t		k;
	char		*tmp;
	i32			ival;
	u8			osc;

	dprintf(outfile, "%s,\n", ti42_getname());
	for (i = 0; i < TI42_BOOLEAN_CAPS; i++) {
		ival = ti42_getflag(bool_caps[i].name);
		if (ival)
			dprintf(outfile, "\t%s,\n", bool_caps[i].name_str);
	}
	for (i = 0; i < TI42_NUMERIC_CAPS; i++) {
		ival = ti42_getnum(num_caps[i].name);
		if (ival != (i32)TI42_ABS_NUM) {
			if (ival <= 100)
				dprintf(outfile, "\t%s#%d,\n", num_caps[i].name_str, ival);
			else
				dprintf(outfile, "\t%s#%#x,\n", num_caps[i].name_str, ival);
		}
	}
	for (i = 0; i < TI42_STRING_CAPS; i++) {
		sval = ti42_getstr(str_caps[i].name);
		if (sval != TI42_ABS_STR) {
			tmp = malloc((strlen(sval) * 4) * sizeof(*tmp));
			if (!tmp)
				return ;
			for (osc = j = k = 0; sval[j]; j++, k++) {
				if (!isprint(sval[j]) || sval[j] == ',' || sval[j] == '\\' || sval[j] == '\x7f') {
					switch (sval[j]) {
						case '\n':
							tmp[k++] = '\\';
							tmp[k] = 'n';
							break ;
						case '\r':
							tmp[k++] = '\\';
							tmp[k] = 'r';
							break ;
						case '\x1b':
							if (sval[j + 1] == ']')
								osc = 1;
							tmp[k++] = '\\';
							tmp[k] = 'E';
							break ;
						case ',':
						case '\\':
							tmp[k++] = '\\';
							tmp[k] = sval[j];
							break ;
						default:
							if (!osc) {
								tmp[k++] = '^';
								tmp[k] = (sval[j]) != '\x7f' ? sval[j] + '@' : '?';
							} else {
								tmp[k++] = '\\';
								tmp[k++] = ((sval[j] >> 6) & 0x7U) + '0';
								tmp[k++] = ((sval[j] >> 4) & 0x7U) + '0';
								tmp[k] = (sval[j] & 0x7U) + '0';
							}
					}
				} else {
					if (osc && (sval[j] == '\x07' || sval[j] == '\x9c'))
						osc = 0;
					tmp[k] = sval[j];
				}
			}
			tmp[k] = '\0';
			dprintf(outfile, "\t%s=%s,\n", str_caps[i].name_str, tmp);
			free(tmp);
		}
	}
}

static inline void	_dump_file(const char *name, const char *data) {
	const char	*start;
	char		*esc;
	char		escaped;

	start = data;
	fprintf(stderr, "\n%s: {\n", name);
	do {
		esc = (char *)start;
		while (*esc && !in_range(*esc, 0x1, 0x1F))
			esc++;
		if (*esc) {
			escaped = *esc;
			*esc = '\0';
		} else
			esc = NULL;
		write(2, start, strlen(start));
		switch (escaped) {
			case '\n':
				write(2, &escaped, 1);
				break ;
			case '\r':
				write(2, "\\r", 2);
				break ;
			case '\x1b':
				write(2, "\\E", 2);
				break ;
			default:
				write(2, "^", 1);
				escaped += '@';
				write(2, &escaped, 1);
		}
		start = esc + 1;
	} while (esc);
	fputs("}\n", stderr);
}

static inline i32	_cmp_output(const i32 tester, const i32 infocmp) {
	struct stat	st;
	const char	*f1;
	const char	*f2;
	const char	*tmp;
	size_t		f2_l1_len;
	size_t		f1_size;
	size_t		f2_size;
	i32			rv;

	rv = 1;
	f1 = MAP_FAILED;
	f2 = MAP_FAILED;
	if (fstat(tester, &st) == -1)
		goto _stat_err;
	f1_size = st.st_size;
	if (fstat(infocmp, &st) == -1)
		goto _stat_err;
	f2_size = st.st_size;
	f1 = mmap(NULL, f1_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, tester, 0);
	f2 = mmap(NULL, f2_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, infocmp, 0);
	close(infocmp);
	close(tester);
	if (f1 == MAP_FAILED || f2 == MAP_FAILED)
		goto _unmap_files;
	tmp = strchr(f2, '\n');
	f2_l1_len = tmp - f2 + 1;
	f2 += f2_l1_len;
	f2_size -= f2_l1_len;
	if (f1_size != f2_size || memcmp(f1, f2, f1_size) != 0) {
		fputs(SGR_KO "Output mismatch:\n", stderr);
		_dump_file("tester", f1);
		_dump_file("infocmp", f2);
		fputs("\x1b[m", stderr);
	} else {
		fputs(SGR_OK "Output match\n", stderr);
		_dump_file("output", f1);
		fputs("\x1b[m", stderr);
		rv = 0;
	}
	f2_size += f2_l1_len;
	f2 -= f2_l1_len;
_unmap_files:
	munmap((void *)f1, f1_size);
	munmap((void *)f2, f2_size);
	if (rv)
		fprintf(stderr, SGR_KO "Fatal error: %s" ENDL, strerror(errno));
	return rv;
_stat_err:
	fprintf(stderr, SGR_KO "Fatal error: %s" ENDL, strerror(errno));
	close(infocmp);
	close(tester);
	return 1;
}

static void	_unlinker(void) {
	unlink(".infocmp.out");
	unlink(".tester.out");
}
