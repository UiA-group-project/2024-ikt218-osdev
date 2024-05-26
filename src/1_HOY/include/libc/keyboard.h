/*

        https://blog.igorw.org/2015/03/04/scancode-to-ascii/
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/idt.h"
#include "libc/isr.h"

isr_t keyboard_handler(registers_t *regs, void * /* context */);

void init_keyboard();

#endif