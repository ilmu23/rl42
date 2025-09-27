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

static const cap	bool_caps[TI_BOOLEAN_CAPS] = {
	{ .name_str = "OTMT", .name = ti_OTMT },
	{ .name_str = "OTNL", .name = ti_OTNL },
	{ .name_str = "OTXR", .name = ti_OTXR },
	{ .name_str = "OTbs", .name = ti_OTbs },
	{ .name_str = "OTnc", .name = ti_OTnc },
	{ .name_str = "OTns", .name = ti_OTns },
	{ .name_str = "OTpt", .name = ti_OTpt },
	{ .name_str = "am", .name = ti_am },
	{ .name_str = "bce", .name = ti_bce },
	{ .name_str = "bw", .name = ti_bw },
	{ .name_str = "ccc", .name = ti_ccc },
	{ .name_str = "chts", .name = ti_chts },
	{ .name_str = "cpix", .name = ti_cpix },
	{ .name_str = "crxm", .name = ti_crxm },
	{ .name_str = "da", .name = ti_da },
	{ .name_str = "daisy", .name = ti_daisy },
	{ .name_str = "db", .name = ti_db },
	{ .name_str = "eo", .name = ti_eo },
	{ .name_str = "eslok", .name = ti_eslok },
	{ .name_str = "gn", .name = ti_gn },
	{ .name_str = "hc", .name = ti_hc },
	{ .name_str = "hls", .name = ti_hls },
	{ .name_str = "hs", .name = ti_hs },
	{ .name_str = "hz", .name = ti_hz },
	{ .name_str = "in", .name = ti_in },
	{ .name_str = "km", .name = ti_km },
	{ .name_str = "lpix", .name = ti_lpix },
	{ .name_str = "mc5i", .name = ti_mc5i },
	{ .name_str = "mir", .name = ti_mir },
	{ .name_str = "msgr", .name = ti_msgr },
	{ .name_str = "ndscr", .name = ti_ndscr },
	{ .name_str = "npc", .name = ti_npc },
	{ .name_str = "nrrmc", .name = ti_nrrmc },
	{ .name_str = "nxon", .name = ti_nxon },
	{ .name_str = "os", .name = ti_os },
	{ .name_str = "sam", .name = ti_sam },
	{ .name_str = "ul", .name = ti_ul },
	{ .name_str = "xenl", .name = ti_xenl },
	{ .name_str = "xhp", .name = ti_xhp },
	{ .name_str = "xhpa", .name = ti_xhpa },
	{ .name_str = "xon", .name = ti_xon },
	{ .name_str = "xsb", .name = ti_xsb },
	{ .name_str = "xt", .name = ti_xt },
	{ .name_str = "xvpa", .name = ti_xvpa },
};

static const cap	num_caps[TI_NUMERIC_CAPS] = {
	{ .name_str = "OTdB", .name = ti_OTdB },
	{ .name_str = "OTdC", .name = ti_OTdC },
	{ .name_str = "OTdN", .name = ti_OTdN },
	{ .name_str = "OTdT", .name = ti_OTdT },
	{ .name_str = "OTkn", .name = ti_OTkn },
	{ .name_str = "OTug", .name = ti_OTug },
	{ .name_str = "bitwin", .name = ti_bitwin },
	{ .name_str = "bitype", .name = ti_bitype },
	{ .name_str = "btns", .name = ti_btns },
	{ .name_str = "bufsz", .name = ti_bufsz },
	{ .name_str = "colors", .name = ti_colors },
	{ .name_str = "cols", .name = ti_cols },
	{ .name_str = "cps", .name = ti_cps },
	{ .name_str = "it", .name = ti_it },
	{ .name_str = "lh", .name = ti_lh },
	{ .name_str = "lines", .name = ti_lines },
	{ .name_str = "lm", .name = ti_lm },
	{ .name_str = "lw", .name = ti_lw },
	{ .name_str = "ma", .name = ti_ma },
	{ .name_str = "maddr", .name = ti_maddr },
	{ .name_str = "mcs", .name = ti_mcs },
	{ .name_str = "mjump", .name = ti_mjump },
	{ .name_str = "mls", .name = ti_mls },
	{ .name_str = "ncv", .name = ti_ncv },
	{ .name_str = "nlab", .name = ti_nlab },
	{ .name_str = "npins", .name = ti_npins },
	{ .name_str = "orc", .name = ti_orc },
	{ .name_str = "orhi", .name = ti_orhi },
	{ .name_str = "orl", .name = ti_orl },
	{ .name_str = "orvi", .name = ti_orvi },
	{ .name_str = "pairs", .name = ti_pairs },
	{ .name_str = "pb", .name = ti_pb },
	{ .name_str = "spinh", .name = ti_spinh },
	{ .name_str = "spinv", .name = ti_spinv },
	{ .name_str = "vt", .name = ti_vt },
	{ .name_str = "widcs", .name = ti_widcs },
	{ .name_str = "wnum", .name = ti_wnum },
	{ .name_str = "wsl", .name = ti_wsl },
	{ .name_str = "xmc", .name = ti_xmc },
};

static const cap str_caps[TI_STRING_CAPS] = {
	{ .name_str = "acsc", .name = ti_acsc },
	{ .name_str = "bel", .name = ti_bel },
	{ .name_str = "bicr", .name = ti_bicr },
	{ .name_str = "binel", .name = ti_binel },
	{ .name_str = "birep", .name = ti_birep },
	{ .name_str = "blink", .name = ti_blink },
	{ .name_str = "bold", .name = ti_bold },
	{ .name_str = "cbt", .name = ti_cbt },
	{ .name_str = "chr", .name = ti_chr },
	{ .name_str = "civis", .name = ti_civis },
	{ .name_str = "clear", .name = ti_clear },
	{ .name_str = "cmdch", .name = ti_cmdch },
	{ .name_str = "cnorm", .name = ti_cnorm },
	{ .name_str = "colornm", .name = ti_colornm },
	{ .name_str = "cpi", .name = ti_cpi },
	{ .name_str = "cr", .name = ti_cr },
	{ .name_str = "csin", .name = ti_csin },
	{ .name_str = "csnm", .name = ti_csnm },
	{ .name_str = "csr", .name = ti_csr },
	{ .name_str = "cub", .name = ti_cub },
	{ .name_str = "cub1", .name = ti_cub1 },
	{ .name_str = "cud", .name = ti_cud },
	{ .name_str = "cud1", .name = ti_cud1 },
	{ .name_str = "cuf", .name = ti_cuf },
	{ .name_str = "cuf1", .name = ti_cuf1 },
	{ .name_str = "cup", .name = ti_cup },
	{ .name_str = "cuu", .name = ti_cuu },
	{ .name_str = "cuu1", .name = ti_cuu1 },
	{ .name_str = "cvr", .name = ti_cvr },
	{ .name_str = "cvvis", .name = ti_cvvis },
	{ .name_str = "cwin", .name = ti_cwin },
	{ .name_str = "dch", .name = ti_dch },
	{ .name_str = "dch1", .name = ti_dch1 },
	{ .name_str = "dclk", .name = ti_dclk },
	{ .name_str = "defbi", .name = ti_defbi },
	{ .name_str = "defc", .name = ti_defc },
	{ .name_str = "devt", .name = ti_devt },
	{ .name_str = "dial", .name = ti_dial },
	{ .name_str = "dim", .name = ti_dim },
	{ .name_str = "dispc", .name = ti_dispc },
	{ .name_str = "dl", .name = ti_dl },
	{ .name_str = "dl1", .name = ti_dl1 },
	{ .name_str = "docr", .name = ti_docr },
	{ .name_str = "dsl", .name = ti_dsl },
	{ .name_str = "ech", .name = ti_ech },
	{ .name_str = "ed", .name = ti_ed },
	{ .name_str = "ehhlm", .name = ti_ehhlm },
	{ .name_str = "el", .name = ti_el },
	{ .name_str = "el1", .name = ti_el1 },
	{ .name_str = "elhlm", .name = ti_elhlm },
	{ .name_str = "elohlm", .name = ti_elohlm },
	{ .name_str = "enacs", .name = ti_enacs },
	{ .name_str = "endbi", .name = ti_endbi },
	{ .name_str = "erhlm", .name = ti_erhlm },
	{ .name_str = "ethlm", .name = ti_ethlm },
	{ .name_str = "evhlm", .name = ti_evhlm },
	{ .name_str = "ff", .name = ti_ff },
	{ .name_str = "flash", .name = ti_flash },
	{ .name_str = "fln", .name = ti_fln },
	{ .name_str = "fsl", .name = ti_fsl },
	{ .name_str = "getm", .name = ti_getm },
	{ .name_str = "hd", .name = ti_hd },
	{ .name_str = "home", .name = ti_home },
	{ .name_str = "hook", .name = ti_hook },
	{ .name_str = "hpa", .name = ti_hpa },
	{ .name_str = "ht", .name = ti_ht },
	{ .name_str = "hts", .name = ti_hts },
	{ .name_str = "hu", .name = ti_hu },
	{ .name_str = "hup", .name = ti_hup },
	{ .name_str = "ich", .name = ti_ich },
	{ .name_str = "ich1", .name = ti_ich1 },
	{ .name_str = "if", .name = ti_if },
	{ .name_str = "il", .name = ti_il },
	{ .name_str = "il1", .name = ti_il1 },
	{ .name_str = "ind", .name = ti_ind },
	{ .name_str = "indn", .name = ti_indn },
	{ .name_str = "initc", .name = ti_initc },
	{ .name_str = "initp", .name = ti_initp },
	{ .name_str = "invis", .name = ti_invis },
	{ .name_str = "ip", .name = ti_ip },
	{ .name_str = "iprog", .name = ti_iprog },
	{ .name_str = "is1", .name = ti_is1 },
	{ .name_str = "is2", .name = ti_is2 },
	{ .name_str = "is3", .name = ti_is3 },
	{ .name_str = "kBEG", .name = ti_kBEG },
	{ .name_str = "kCAN", .name = ti_kCAN },
	{ .name_str = "kCMD", .name = ti_kCMD },
	{ .name_str = "kCPY", .name = ti_kCPY },
	{ .name_str = "kCRT", .name = ti_kCRT },
	{ .name_str = "kDC", .name = ti_kDC },
	{ .name_str = "kDL", .name = ti_kDL },
	{ .name_str = "kEND", .name = ti_kEND },
	{ .name_str = "kEOL", .name = ti_kEOL },
	{ .name_str = "kEXT", .name = ti_kEXT },
	{ .name_str = "kFND", .name = ti_kFND },
	{ .name_str = "kHLP", .name = ti_kHLP },
	{ .name_str = "kHOM", .name = ti_kHOM },
	{ .name_str = "kIC", .name = ti_kIC },
	{ .name_str = "kLFT", .name = ti_kLFT },
	{ .name_str = "kMOV", .name = ti_kMOV },
	{ .name_str = "kMSG", .name = ti_kMSG },
	{ .name_str = "kNXT", .name = ti_kNXT },
	{ .name_str = "kOPT", .name = ti_kOPT },
	{ .name_str = "kPRT", .name = ti_kPRT },
	{ .name_str = "kPRV", .name = ti_kPRV },
	{ .name_str = "kRDO", .name = ti_kRDO },
	{ .name_str = "kRES", .name = ti_kRES },
	{ .name_str = "kRIT", .name = ti_kRIT },
	{ .name_str = "kRPL", .name = ti_kRPL },
	{ .name_str = "kSAV", .name = ti_kSAV },
	{ .name_str = "kSPD", .name = ti_kSPD },
	{ .name_str = "kUND", .name = ti_kUND },
	{ .name_str = "ka1", .name = ti_ka1 },
	{ .name_str = "ka3", .name = ti_ka3 },
	{ .name_str = "kb2", .name = ti_kb2 },
	{ .name_str = "kbeg", .name = ti_kbeg },
	{ .name_str = "kbs", .name = ti_kbs },
	{ .name_str = "kc1", .name = ti_kc1 },
	{ .name_str = "kc3", .name = ti_kc3 },
	{ .name_str = "kcan", .name = ti_kcan },
	{ .name_str = "kcbt", .name = ti_kcbt },
	{ .name_str = "kclo", .name = ti_kclo },
	{ .name_str = "kclr", .name = ti_kclr },
	{ .name_str = "kcmd", .name = ti_kcmd },
	{ .name_str = "kcpy", .name = ti_kcpy },
	{ .name_str = "kcrt", .name = ti_kcrt },
	{ .name_str = "kctab", .name = ti_kctab },
	{ .name_str = "kcub1", .name = ti_kcub1 },
	{ .name_str = "kcud1", .name = ti_kcud1 },
	{ .name_str = "kcuf1", .name = ti_kcuf1 },
	{ .name_str = "kcuu1", .name = ti_kcuu1 },
	{ .name_str = "kdch1", .name = ti_kdch1 },
	{ .name_str = "kdl1", .name = ti_kdl1 },
	{ .name_str = "ked", .name = ti_ked },
	{ .name_str = "kel", .name = ti_kel },
	{ .name_str = "kend", .name = ti_kend },
	{ .name_str = "kent", .name = ti_kent },
	{ .name_str = "kext", .name = ti_kext },
	{ .name_str = "kf0", .name = ti_kf0 },
	{ .name_str = "kf1", .name = ti_kf1 },
	{ .name_str = "kf10", .name = ti_kf10 },
	{ .name_str = "kf11", .name = ti_kf11 },
	{ .name_str = "kf12", .name = ti_kf12 },
	{ .name_str = "kf13", .name = ti_kf13 },
	{ .name_str = "kf14", .name = ti_kf14 },
	{ .name_str = "kf15", .name = ti_kf15 },
	{ .name_str = "kf16", .name = ti_kf16 },
	{ .name_str = "kf17", .name = ti_kf17 },
	{ .name_str = "kf18", .name = ti_kf18 },
	{ .name_str = "kf19", .name = ti_kf19 },
	{ .name_str = "kf2", .name = ti_kf2 },
	{ .name_str = "kf20", .name = ti_kf20 },
	{ .name_str = "kf21", .name = ti_kf21 },
	{ .name_str = "kf22", .name = ti_kf22 },
	{ .name_str = "kf23", .name = ti_kf23 },
	{ .name_str = "kf24", .name = ti_kf24 },
	{ .name_str = "kf25", .name = ti_kf25 },
	{ .name_str = "kf26", .name = ti_kf26 },
	{ .name_str = "kf27", .name = ti_kf27 },
	{ .name_str = "kf28", .name = ti_kf28 },
	{ .name_str = "kf29", .name = ti_kf29 },
	{ .name_str = "kf3", .name = ti_kf3 },
	{ .name_str = "kf30", .name = ti_kf30 },
	{ .name_str = "kf31", .name = ti_kf31 },
	{ .name_str = "kf32", .name = ti_kf32 },
	{ .name_str = "kf33", .name = ti_kf33 },
	{ .name_str = "kf34", .name = ti_kf34 },
	{ .name_str = "kf35", .name = ti_kf35 },
	{ .name_str = "kf36", .name = ti_kf36 },
	{ .name_str = "kf37", .name = ti_kf37 },
	{ .name_str = "kf38", .name = ti_kf38 },
	{ .name_str = "kf39", .name = ti_kf39 },
	{ .name_str = "kf4", .name = ti_kf4 },
	{ .name_str = "kf40", .name = ti_kf40 },
	{ .name_str = "kf41", .name = ti_kf41 },
	{ .name_str = "kf42", .name = ti_kf42 },
	{ .name_str = "kf43", .name = ti_kf43 },
	{ .name_str = "kf44", .name = ti_kf44 },
	{ .name_str = "kf45", .name = ti_kf45 },
	{ .name_str = "kf46", .name = ti_kf46 },
	{ .name_str = "kf47", .name = ti_kf47 },
	{ .name_str = "kf48", .name = ti_kf48 },
	{ .name_str = "kf49", .name = ti_kf49 },
	{ .name_str = "kf5", .name = ti_kf5 },
	{ .name_str = "kf50", .name = ti_kf50 },
	{ .name_str = "kf51", .name = ti_kf51 },
	{ .name_str = "kf52", .name = ti_kf52 },
	{ .name_str = "kf53", .name = ti_kf53 },
	{ .name_str = "kf54", .name = ti_kf54 },
	{ .name_str = "kf55", .name = ti_kf55 },
	{ .name_str = "kf56", .name = ti_kf56 },
	{ .name_str = "kf57", .name = ti_kf57 },
	{ .name_str = "kf58", .name = ti_kf58 },
	{ .name_str = "kf59", .name = ti_kf59 },
	{ .name_str = "kf6", .name = ti_kf6 },
	{ .name_str = "kf60", .name = ti_kf60 },
	{ .name_str = "kf61", .name = ti_kf61 },
	{ .name_str = "kf62", .name = ti_kf62 },
	{ .name_str = "kf63", .name = ti_kf63 },
	{ .name_str = "kf7", .name = ti_kf7 },
	{ .name_str = "kf8", .name = ti_kf8 },
	{ .name_str = "kf9", .name = ti_kf9 },
	{ .name_str = "kfnd", .name = ti_kfnd },
	{ .name_str = "khlp", .name = ti_khlp },
	{ .name_str = "khome", .name = ti_khome },
	{ .name_str = "khts", .name = ti_khts },
	{ .name_str = "kich1", .name = ti_kich1 },
	{ .name_str = "kil1", .name = ti_kil1 },
	{ .name_str = "kind", .name = ti_kind },
	{ .name_str = "kll", .name = ti_kll },
	{ .name_str = "kmous", .name = ti_kmous },
	{ .name_str = "kmov", .name = ti_kmov },
	{ .name_str = "kmrk", .name = ti_kmrk },
	{ .name_str = "kmsg", .name = ti_kmsg },
	{ .name_str = "knp", .name = ti_knp },
	{ .name_str = "knxt", .name = ti_knxt },
	{ .name_str = "kopn", .name = ti_kopn },
	{ .name_str = "kopt", .name = ti_kopt },
	{ .name_str = "kpp", .name = ti_kpp },
	{ .name_str = "kprt", .name = ti_kprt },
	{ .name_str = "kprv", .name = ti_kprv },
	{ .name_str = "krdo", .name = ti_krdo },
	{ .name_str = "kref", .name = ti_kref },
	{ .name_str = "kres", .name = ti_kres },
	{ .name_str = "krfr", .name = ti_krfr },
	{ .name_str = "kri", .name = ti_kri },
	{ .name_str = "krmir", .name = ti_krmir },
	{ .name_str = "krpl", .name = ti_krpl },
	{ .name_str = "krst", .name = ti_krst },
	{ .name_str = "ksav", .name = ti_ksav },
	{ .name_str = "kslt", .name = ti_kslt },
	{ .name_str = "kspd", .name = ti_kspd },
	{ .name_str = "ktbc", .name = ti_ktbc },
	{ .name_str = "kund", .name = ti_kund },
	{ .name_str = "lf0", .name = ti_lf0 },
	{ .name_str = "lf1", .name = ti_lf1 },
	{ .name_str = "lf10", .name = ti_lf10 },
	{ .name_str = "lf2", .name = ti_lf2 },
	{ .name_str = "lf3", .name = ti_lf3 },
	{ .name_str = "lf4", .name = ti_lf4 },
	{ .name_str = "lf5", .name = ti_lf5 },
	{ .name_str = "lf6", .name = ti_lf6 },
	{ .name_str = "lf7", .name = ti_lf7 },
	{ .name_str = "lf8", .name = ti_lf8 },
	{ .name_str = "lf9", .name = ti_lf9 },
	{ .name_str = "ll", .name = ti_ll },
	{ .name_str = "lpi", .name = ti_lpi },
	{ .name_str = "mc0", .name = ti_mc0 },
	{ .name_str = "mc4", .name = ti_mc4 },
	{ .name_str = "mc5", .name = ti_mc5 },
	{ .name_str = "mc5p", .name = ti_mc5p },
	{ .name_str = "mcub", .name = ti_mcub },
	{ .name_str = "mcub1", .name = ti_mcub1 },
	{ .name_str = "mcud", .name = ti_mcud },
	{ .name_str = "mcud1", .name = ti_mcud1 },
	{ .name_str = "mcuf", .name = ti_mcuf },
	{ .name_str = "mcuf1", .name = ti_mcuf1 },
	{ .name_str = "mcuu", .name = ti_mcuu },
	{ .name_str = "mcuu1", .name = ti_mcuu1 },
	{ .name_str = "mgc", .name = ti_mgc },
	{ .name_str = "mhpa", .name = ti_mhpa },
	{ .name_str = "minfo", .name = ti_minfo },
	{ .name_str = "mrcup", .name = ti_mrcup },
	{ .name_str = "mvpa", .name = ti_mvpa },
	{ .name_str = "nel", .name = ti_nel },
	{ .name_str = "oc", .name = ti_oc },
	{ .name_str = "op", .name = ti_op },
	{ .name_str = "pad", .name = ti_pad },
	{ .name_str = "pause", .name = ti_pause },
	{ .name_str = "pctrm", .name = ti_pctrm },
	{ .name_str = "pfkey", .name = ti_pfkey },
	{ .name_str = "pfloc", .name = ti_pfloc },
	{ .name_str = "pfx", .name = ti_pfx },
	{ .name_str = "pfxl", .name = ti_pfxl },
	{ .name_str = "pln", .name = ti_pln },
	{ .name_str = "porder", .name = ti_porder },
	{ .name_str = "prot", .name = ti_prot },
	{ .name_str = "pulse", .name = ti_pulse },
	{ .name_str = "qdial", .name = ti_qdial },
	{ .name_str = "rbim", .name = ti_rbim },
	{ .name_str = "rc", .name = ti_rc },
	{ .name_str = "rcsd", .name = ti_rcsd },
	{ .name_str = "rep", .name = ti_rep },
	{ .name_str = "reqmp", .name = ti_reqmp },
	{ .name_str = "rev", .name = ti_rev },
	{ .name_str = "rf", .name = ti_rf },
	{ .name_str = "rfi", .name = ti_rfi },
	{ .name_str = "ri", .name = ti_ri },
	{ .name_str = "rin", .name = ti_rin },
	{ .name_str = "ritm", .name = ti_ritm },
	{ .name_str = "rlm", .name = ti_rlm },
	{ .name_str = "rmacs", .name = ti_rmacs },
	{ .name_str = "rmam", .name = ti_rmam },
	{ .name_str = "rmclk", .name = ti_rmclk },
	{ .name_str = "rmcup", .name = ti_rmcup },
	{ .name_str = "rmdc", .name = ti_rmdc },
	{ .name_str = "rmicm", .name = ti_rmicm },
	{ .name_str = "rmir", .name = ti_rmir },
	{ .name_str = "rmkx", .name = ti_rmkx },
	{ .name_str = "rmln", .name = ti_rmln },
	{ .name_str = "rmm", .name = ti_rmm },
	{ .name_str = "rmp", .name = ti_rmp },
	{ .name_str = "rmpch", .name = ti_rmpch },
	{ .name_str = "rmsc", .name = ti_rmsc },
	{ .name_str = "rmso", .name = ti_rmso },
	{ .name_str = "rmul", .name = ti_rmul },
	{ .name_str = "rmxon", .name = ti_rmxon },
	{ .name_str = "rs1", .name = ti_rs1 },
	{ .name_str = "rs2", .name = ti_rs2 },
	{ .name_str = "rs3", .name = ti_rs3 },
	{ .name_str = "rshm", .name = ti_rshm },
	{ .name_str = "rsubm", .name = ti_rsubm },
	{ .name_str = "rsupm", .name = ti_rsupm },
	{ .name_str = "rum", .name = ti_rum },
	{ .name_str = "rwidm", .name = ti_rwidm },
	{ .name_str = "s0ds", .name = ti_s0ds },
	{ .name_str = "s1ds", .name = ti_s1ds },
	{ .name_str = "s2ds", .name = ti_s2ds },
	{ .name_str = "s3ds", .name = ti_s3ds },
	{ .name_str = "sbim", .name = ti_sbim },
	{ .name_str = "sc", .name = ti_sc },
	{ .name_str = "scesa", .name = ti_scesa },
	{ .name_str = "scesc", .name = ti_scesc },
	{ .name_str = "sclk", .name = ti_sclk },
	{ .name_str = "scp", .name = ti_scp },
	{ .name_str = "scs", .name = ti_scs },
	{ .name_str = "scsd", .name = ti_scsd },
	{ .name_str = "sdrfq", .name = ti_sdrfq },
	{ .name_str = "setab", .name = ti_setab },
	{ .name_str = "setaf", .name = ti_setaf },
	{ .name_str = "setb", .name = ti_setb },
	{ .name_str = "setcolor", .name = ti_setcolor },
	{ .name_str = "setf", .name = ti_setf },
	{ .name_str = "sgr", .name = ti_sgr },
	{ .name_str = "sgr0", .name = ti_sgr0 },
	{ .name_str = "sgr1", .name = ti_sgr1 },
	{ .name_str = "sitm", .name = ti_sitm },
	{ .name_str = "slength", .name = ti_slength },
	{ .name_str = "slines", .name = ti_slines },
	{ .name_str = "slm", .name = ti_slm },
	{ .name_str = "smacs", .name = ti_smacs },
	{ .name_str = "smam", .name = ti_smam },
	{ .name_str = "smcup", .name = ti_smcup },
	{ .name_str = "smdc", .name = ti_smdc },
	{ .name_str = "smgb", .name = ti_smgb },
	{ .name_str = "smgbp", .name = ti_smgbp },
	{ .name_str = "smgl", .name = ti_smgl },
	{ .name_str = "smglp", .name = ti_smglp },
	{ .name_str = "smglr", .name = ti_smglr },
	{ .name_str = "smgr", .name = ti_smgr },
	{ .name_str = "smgrp", .name = ti_smgrp },
	{ .name_str = "smgt", .name = ti_smgt },
	{ .name_str = "smgtb", .name = ti_smgtb },
	{ .name_str = "smgtp", .name = ti_smgtp },
	{ .name_str = "smicm", .name = ti_smicm },
	{ .name_str = "smir", .name = ti_smir },
	{ .name_str = "smkx", .name = ti_smkx },
	{ .name_str = "smln", .name = ti_smln },
	{ .name_str = "smm", .name = ti_smm },
	{ .name_str = "smpch", .name = ti_smpch },
	{ .name_str = "smsc", .name = ti_smsc },
	{ .name_str = "smso", .name = ti_smso },
	{ .name_str = "smul", .name = ti_smul },
	{ .name_str = "smxon", .name = ti_smxon },
	{ .name_str = "snlq", .name = ti_snlq },
	{ .name_str = "snrmq", .name = ti_snrmq },
	{ .name_str = "sshm", .name = ti_sshm },
	{ .name_str = "ssubm", .name = ti_ssubm },
	{ .name_str = "ssupm", .name = ti_ssupm },
	{ .name_str = "subcs", .name = ti_subcs },
	{ .name_str = "sum", .name = ti_sum },
	{ .name_str = "supcs", .name = ti_supcs },
	{ .name_str = "swidm", .name = ti_swidm },
	{ .name_str = "tbc", .name = ti_tbc },
	{ .name_str = "tone", .name = ti_tone },
	{ .name_str = "tsl", .name = ti_tsl },
	{ .name_str = "u0", .name = ti_u0 },
	{ .name_str = "u1", .name = ti_u1 },
	{ .name_str = "u2", .name = ti_u2 },
	{ .name_str = "u3", .name = ti_u3 },
	{ .name_str = "u4", .name = ti_u4 },
	{ .name_str = "u5", .name = ti_u5 },
	{ .name_str = "u6", .name = ti_u6 },
	{ .name_str = "u7", .name = ti_u7 },
	{ .name_str = "u8", .name = ti_u8 },
	{ .name_str = "u9", .name = ti_u9 },
	{ .name_str = "uc", .name = ti_uc },
	{ .name_str = "vpa", .name = ti_vpa },
	{ .name_str = "wait", .name = ti_wait },
	{ .name_str = "wind", .name = ti_wind },
	{ .name_str = "wingo", .name = ti_wingo },
	{ .name_str = "xoffc", .name = ti_xoffc },
	{ .name_str = "xonc", .name = ti_xonc },
	{ .name_str = "zerom", .name = ti_zerom },
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
	if (!ti_load(getenv("TERM")))
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

	dprintf(outfile, "%s,\n", ti_getname());
	for (i = 0; i < TI_BOOLEAN_CAPS; i++) {
		ival = ti_getflag(bool_caps[i].name);
		if (ival)
			dprintf(outfile, "\t%s,\n", bool_caps[i].name_str);
	}
	for (i = 0; i < TI_NUMERIC_CAPS; i++) {
		ival = ti_getnum(num_caps[i].name);
		if (ival != (i32)TI_ABS_NUM) {
			if (ival <= 100)
				dprintf(outfile, "\t%s#%d,\n", num_caps[i].name_str, ival);
			else
				dprintf(outfile, "\t%s#%#x,\n", num_caps[i].name_str, ival);
		}
	}
	for (i = 0; i < TI_STRING_CAPS; i++) {
		sval = ti_getstr(str_caps[i].name);
		if (sval != TI_ABS_STR) {
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
