#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include <libc/string.h>
#include "libc/isr.h"

extern void idt_flush(uint32_t);
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();

static void init_idt();
static void idt_set_gate(uint8_t,uint32_t,uint16_t,uint8_t);

idt_entry_t idt_entries[256];
gdt_ptr_t   idt_ptr;

void init_descriptor_tables()
{
  init_gdt();
  init_idt();
}

static void init_idt()
{
   idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
   idt_ptr.base  = (uint32_t)&idt_entries;

   //memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
   
  //remap the irq table
   outb(0x20, 0x11);
   outb(0xA0, 0x11);
   outb(0x21, 0x20);
   outb(0xA1, 0x28);
   outb(0x21, 0x04);
   outb(0xA1, 0x02);
   outb(0x21, 0x01);
   outb(0xA1, 0x01);
   outb(0x21, 0x0);
   outb(0xA1, 0x0); 


   idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
   idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
   idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
   idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
   idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
   
   idt_flush((uint32_t)&idt_ptr);
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;

} 