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
#include "libc/keyboard.h"

// Array for non-shifted characters
char scan_code_chars_lower[128] = {
    '\0', 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0',
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0', '*', '\0', ' ',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

isr_t keyboard_handler(registers_t *regs, void * /* context */)
{
    int status;
    int scancode = 0;
    while (1)
    {
        status = inb(0x64);
        if (status & 0x01)
        {
            scancode = inb(0x60);

            // Check if this is a key press or release
            int key_release = scancode & 0x80;

            // Normalize the scancode
            scancode &= ~0x80;

            // If this is a key release, ignore it
            if (key_release)
            {
                return 0;
            }

            monitor_put(scan_code_chars_lower[scancode]);
            // TODO: call prontf instead write_string(0x0F, "\n");
            break;
        }
    }
}

void init_keyboard()
{
    register_interrupt_handler(33, keyboard_handler);

    /*
        Enable the IRQ1, then enable the hardware interrupts
        when using only 'asm volatile("int $0x21")' the program reads only one key press
        when using only 'asm volatile("sti")' the program reads nothing
        when using both the program functions as intended and reading the interrupts as long as the program runs.
    */
    asm volatile("int $0x21");
    asm volatile("sti");
}
