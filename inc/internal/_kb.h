// ███████╗████████╗     ██████╗ ██╗   ██╗████████╗ ██████╗██╗  ██╗ █████╗ ██████╗
// ██╔════╝╚══██╔══╝     ██╔══██╗██║   ██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗██╔══██╗
// █████╗     ██║        ██████╔╝██║   ██║   ██║   ██║     ███████║███████║██████╔╝
// ██╔══╝     ██║        ██╔═══╝ ██║   ██║   ██║   ██║     ██╔══██║██╔══██║██╔══██╗
// ██║        ██║███████╗██║     ╚██████╔╝   ██║   ╚██████╗██║  ██║██║  ██║██║  ██║
// ╚═╝        ╚═╝╚══════╝╚═╝      ╚═════╝    ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝
//
// <<_kb.h>>

#pragma once

#ifndef __RL42_INTERNAL
# define __RL42_INTERNAL
#endif

#include "internal/_data.h"

#define KB_MOD_SHIFT	0x01U
#define KB_MOD_ALT		0x02U
#define KB_MOD_CTRL		0x04U
#define KB_MOD_SUPER	0x08U
#define KB_MOD_HYPER	0x10U
#define KB_MOD_META		0x20U

// Legacy functional key codes

#define KB_KEY_LEGACY_F1				0x80000001U
#define KB_KEY_LEGACY_F2				0x80000002U
#define KB_KEY_LEGACY_F3				0x80000003U
#define KB_KEY_LEGACY_F4				0x80000004U
#define KB_KEY_LEGACY_F5				0x80000005U
#define KB_KEY_LEGACY_F6				0x80000006U
#define KB_KEY_LEGACY_F7				0x80000007U
#define KB_KEY_LEGACY_F8				0x80000008U
#define KB_KEY_LEGACY_F9				0x80000009U
#define KB_KEY_LEGACY_F10				0x8000000AU
#define KB_KEY_LEGACY_F11				0x8000000BU
#define KB_KEY_LEGACY_F12				0x8000000CU
#define KB_KEY_LEGACY_UP				0x80000040U
#define KB_KEY_LEGACY_DOWN				0x80000041U
#define KB_KEY_LEGACY_LEFT				0x80000042U
#define KB_KEY_LEGACY_RIGHT				0x80000043U
#define KB_KEY_LEGACY_INSERT			0x80000044U
#define KB_KEY_LEGACY_HOME				0x80000045U
#define KB_KEY_LEGACY_PAGE_UP			0x80000046U
#define KB_KEY_LEGACY_DELETE			0x80000047U
#define KB_KEY_LEGACY_END				0x80000048U
#define KB_KEY_LEGACY_PAGE_DOWN			0x80000049U
#define KB_KEY_LEGACY_SHIFT_UP			0x90000040U
#define KB_KEY_LEGACY_SHIFT_DOWN		0x90000041U
#define KB_KEY_LEGACY_SHIFT_LEFT		0x90000042U
#define KB_KEY_LEGACY_SHIFT_RIGHT		0x90000043U
#define KB_KEY_LEGACY_SHIFT_INSERT		0x90000044U
#define KB_KEY_LEGACY_SHIFT_HOME		0x90000045U
#define KB_KEY_LEGACY_SHIFT_PAGE_UP		0x90000046U
#define KB_KEY_LEGACY_SHIFT_DELETE		0x90000047U
#define KB_KEY_LEGACY_SHIFT_END			0x90000048U
#define KB_KEY_LEGACY_SHIFT_PAGE_DOWN	0x90000049U

/** @brief Listens for a keyboard event
 *
 * @param timeout Listen timeout in milliseconds, -1 for no timeout
 * @returns @c <b>rl42_fn_info</b> Keyboard event information
 */
rl42_kb_event	*kb_listen(const i32 timeout);

/** @brief Frees all keyboard listener data
 */
void			clean_kb_listener(void);

/** @brief Initializes all keyboard listener data
 *
 * @returns @c <b>u8</b> 1 on success,
 * 0 on failure
 */
u8				init_kb_listener(void);
