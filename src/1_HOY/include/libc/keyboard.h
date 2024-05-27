/*

        https://blog.igorw.org/2015/03/04/scancode-to-ascii/
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/monitor.h"
#include "libc/common.h"

void keyboard_handler(registers_t regs);

void init_keyboard();

#endif