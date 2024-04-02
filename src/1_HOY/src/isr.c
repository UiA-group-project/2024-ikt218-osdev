#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/isr.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   write_string( 7,"recieved interrupt: ");
}

