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

extern uint32_t end; // This is defined in arch/i386/linker.ld

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

// Declare the play_song function
void play_song(Song *song);
void sleep(uint32_t milliseconds); // Declare sleep function
void stop_sound(); // Declare stop_sound function

void sleep(uint32_t milliseconds) {
    pit_sleep(milliseconds);
}

void test_speaker_control() {
    printf("Testing speaker control: Enabling speaker for 1 second...\n");

    // Enable speaker
    enable_speaker();
    // Play a sound at a fixed frequency (e.g., 440 Hz for 1 second)
    play_sound(440); // A4 note
    sleep(1000); // 1 second

    // Disable speaker
    stop_sound();
    disable_speaker();
    printf("Speaker should now be off.\n");

    // Wait for 2 seconds to observe silence
    sleep(2000);
}

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    // Initialize the GDT
    init_gdt();

    // Initialize the IDT
    init_descriptor_tables();

     // Initialize the PIT
    init_pit();

    // Disable the speaker before the song starts
    //stop_sound();

    //test speakers: play a sound
    test_speaker_control();


    // Test the music player
    //Song starwars_song = {starwars_theme, sizeof(starwars_theme) / sizeof(Note)};
    //play_song_impl(&starwars_song);
    
    // Delay for 5 seconds
    //sleep(5000);
    //stop_sound();


    // Initialize the ISR
    //init_keyboard();

    // Initialize the memory manager
    init_kernel_memory(&end);

    // Initialize the paging
    init_paging();

    // test
    print_memory_layout();

    while (true)
    {
    };

    return kernel_main();
}
// Call cpp kernel_main (defined in kernel.cpp)
