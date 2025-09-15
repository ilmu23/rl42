// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<utf8.c>>

#include "internal/_utils.h"

#define _BYTE_START	0x80U
#define _LENGTH_2B	(_BYTE_START | 0x40U)
#define _LENGTH_3B	(_BYTE_START | 0x60U)
#define _LENGTH_4B	(_BYTE_START | 0x70U)

#define decode_start_2(c)	((c & 0x1FU) << 6)
#define decode_start_3(c)	((c & 0x0FU) << 12)
#define decode_start_4(c)	((c & 0x07U) << 18)
#define decode_cont(c, r)	((c & 0x3FU) << (6 * r))

size_t	strlen_utf8(const char *s) {
	size_t	len;
	size_t	i;

	for (i = len = 0; s[i]; len++)
		i += charsize_utf8(s[i]);
	return len;
}

u8	charsize_utf8(const char c) {
	if (c & 0x80) {
		switch (c & 0x70) {
			case 0x70:
				return 4;
			case 0x60:
				return 3;
			case 0x40:
				return 2;
		}
	}
	return 1;
}

u32	utf8_decode(const char *c) {
	u32	out;

	if (!(*c & 0x80))
		out = *c;
	else switch (*c & 0x70U) {
		case 0x70U:
			out = decode_start_4(c[0]) | decode_cont(c[1], 2) | decode_cont(c[2], 1) | decode_cont(c[3], 0);
			break ;
		case 0x60U:
			out = decode_start_3(c[0]) | decode_cont(c[1], 1) | decode_cont(c[2], 0);
			break ;
		case 0x40U:
			out = decode_start_2(c[0]) | decode_cont(c[1], 0);
			break ;
		default:
			out = UINT32_MAX;
	}
	return out;
}

const char	*utf8_encode(const u32 ucp, char buf[5]) {
	u8	len;

	if (ucp <= 0x7FU)
		len = 1;
	else if (ucp <= 0x7FFU)
		len = 2;
	else if (ucp <= 0xFFFFU)
		len = 3;
	else if (ucp <= 0x10FFFFU)
		len = 4;
	else
		return NULL;
	// TODO: codepoint validation
	if (!buf)
		buf = malloc((len + 1) * sizeof(*buf));
	if (!buf)
		return NULL;
	switch (len) {
		case 1:
			buf[0] = ucp;
			break ;
		case 2:
			buf[0] = (_LENGTH_2B | ((ucp & 0x07C0U) >> 6));
			buf[1] = (_BYTE_START | (ucp & 0x3FU));
			break ;
		case 3:
			buf[0] = (_LENGTH_3B | ((ucp & 0xF000U) >> 12));
			buf[1] = (_BYTE_START | ((ucp & 0x0FC0U) >> 6));
			buf[2] = (_BYTE_START | (ucp & 0x003FU));
			break ;
		case 4:
			buf[0] = (_LENGTH_4B | ((ucp & 0x1C0000) >> 18));
			buf[1] = (_BYTE_START | ((ucp & 0x03F000U) >> 12));
			buf[2] = (_BYTE_START | ((ucp & 0x000FC0U) >> 6));
			buf[3] = (_BYTE_START | (ucp & 0x00003FU));
	}
	buf[len] = '\0';
	return buf;
}
