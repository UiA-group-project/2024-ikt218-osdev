//code is based on examples from the course materials - especially session 6 and copilot. 

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

//defines memory adress the kernel memory will start at
extern uint32_t end; // This is defined in arch/i386/linker.ld

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

// Check the magic number
    /*if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        // Magic number is invalid
        //write_string(0x0F, "Invalid magic number");
    } else {*/

    // Initialize the GDT
    init_gdt();
    
    // defines the parameters in the print function above and prints "Hello, World!" in white text on a black background
    
    monitor_clear();
    monitor_write("Hello, Yngvar!"); 
    monitor_put('\n');

    //const char *hello_world = "Hello, World!";
    //int colour = 0x0F; // White text, black background
    //write_string(colour, hello_world);    
    
    // Initialize the IDT
    init_descriptor_tables();

    // Test the interrupts
    //asm volatile ("int $0x3");
    //asm volatile ("int $0x0");

    init_keyboard();

    while(1)
    {

    }

        // Initialize the kernel's memory manager using the end address of the kernel.
    init_kernel_memory(&end); // <------ THIS IS PART OF THE ASSIGNMENT

    // Initialize paging for memory management.
    init_paging(); // <------ THIS IS PART OF THE ASSIGNMENT

    // Print memory information.
    print_memory_layout(); // <------ THIS IS PART OF THE ASSIGNMENT

		// Initialize PIT
    init_pit(); // <------ THIS IS PART OF THE ASSIGNMENT

    // test assignment 4 code.
    monitor_write("test assignment 4");
    monitor_put('\n');

    return kernel_main();
    }    
    // Call cpp kernel_main (defined in kernel.cpp)
    