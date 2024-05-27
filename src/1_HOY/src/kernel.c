// code is based on examples from the course materials - especially session 6 and copilot.
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/monitor.h"
#include "libc/common.h"
#include "libc/keyboard.h"
#include "libc/malloc.h"
#include "libc/pit.h"
#include "libc/memory.h"
#include "libc/frequencies.h"
#include "libc/song.h"

extern int kernel_main();

extern uint32_t end; // This is defined in arch/i386/linker.ld

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    // Initialize the GDT and IDT
    init_descriptor_tables();

    // Initialize the memory manager
    init_kernel_memory(&end);

    // Initialize the paging
    init_paging();

    // Initialize the PIT
    init_pit();

    // Print the memory layout
    print_memory_layout();

    // Initialize the keyboard
    init_keyboard();

    return kernel_main();
}
// Call cpp kernel_main (defined in kernel.cpp)
