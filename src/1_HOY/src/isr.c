#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/monitor.h"
#include "libc/common.h"

// This gets called from our ASM interrupt handler stub.
//based on AI code: 

#define NUM_HANDLERS 256
isr_t interrupt_handlers[NUM_HANDLERS];


void isr_handler_0() {
    monitor_write("Received interrupt: 0");
    monitor_put('\n');
}

void isr_handler_1() {
    monitor_write("Received interrupt: 1");
    monitor_put('\n');
}

void isr_handler_2() {
    monitor_write("Received interrupt: 2");
    monitor_put('\n');
}

void isr_handler_3() {
    monitor_write("Received interrupt: 3");
    monitor_put('\n');
}

void isr_handler_4() {
    monitor_write("Received interrupt: 4");
    monitor_put('\n');
}
//code from jamesM's kernel development tutorials part 4: 
void irq_handler(registers_t regs)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
} 

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler)
{
  interrupt_handlers[n] = handler;
} 


//void isr_handler(registers_t regs) {
//    monitor_write("recieved interrupt: ");
//    monitor_put('\n');
//}



// This gets called from our ASM interrupt handler stub.
//void isr_handler(registers_t regs)
//{
   //write_string( 7,"recieved interrupt: ");
//}

