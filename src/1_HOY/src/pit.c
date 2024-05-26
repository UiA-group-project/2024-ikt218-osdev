#include <libc/pit.h>
#include <libc/common.h>
#include <libc/isr.h>

// Reference: http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html

volatile u32int tick = 0;
volatile u32int sleep_end_tick = 0;
volatile int is_sleeping = 0;

// Define the timer callback function
void timer_callback(registers_t regs)
{
    tick++;
    if (is_sleeping && tick >= sleep_end_tick)
    {
        is_sleeping = 0;
    }

    outb(0x20, 0x20); // Send EOI to master PIC
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20); // Send EOI to slave PIC if necessary
    }
}

void sleep_busy(uint32_t milliseconds)
{
    uint32_t start_tick = tick;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;

    while (tick - start_tick < ticks_to_wait)
    {
        // Busy wait...
    }
}

void yield()
{
    // Simple yield function: halt the CPU until the next interrupt
    asm volatile("hlt");
}

void sleep_interrupt(uint32_t milliseconds)
{
    sleep_end_tick = tick + milliseconds * TICKS_PER_MS;
    is_sleeping = 1;

    while (is_sleeping)
    {
        // Yield control by halting the CPU until the next interrupt
        yield();
    }
}

// Initialize the PIT (Programmable Interval Timer)
void init_pit()
{
    // Register the timer callback function for the timer interrupt
    register_interrupt_handler(32, timer_callback); // IRQ0 is at interrupt number 32

    u32int divisor = DIVIDER;

    // Send the command byte
    outb(PIT_CMD_PORT, 0x36);

    // Split the divisor into two bytes: lower and upper. This is because the divisor is 16 bits and has to be sent bytewise.
    u8int low = (u8int)(divisor & 0xFF);
    u8int high = (u8int)((divisor >> 8) & 0xFF);

    // Finally send the divisor bytes
    outb(PIT_CHANNEL0_PORT, low);
    outb(PIT_CHANNEL0_PORT, high);
}
