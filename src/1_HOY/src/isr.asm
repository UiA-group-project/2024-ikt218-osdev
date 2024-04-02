
[GLOBAL isr0]
isr0:
  cli                 ; Disable interrupts
  push byte 0         ; Push a dummy error code (if ISR0 doesn't push it's own error code)
  push byte 0         ; Push the interrupt number (0)
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr1]
isr1:
  cli                 ; Disable interrupts
  push byte 1         ; Push a dummy error code (if ISR1 doesn't push it's own error code)
  push byte 1         ; Push the interrupt number (1)
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr2]
isr2:
  cli                 ; Disable interrupts
  push byte 2         ; Push a dummy error code (if ISR2 doesn't push it's own error code)
  push byte 2         ; Push the interrupt number (2)
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr3]
isr3:
  cli                 ; Disable interrupts
  push byte 3         ; Push a dummy error code (if ISR3 doesn't push it's own error code)
  push byte 3         ; Push the interrupt number (3)
  jmp isr_common_stub ; Go to our common handler.

  [GLOBAL isr4]
isr4:
  cli                 ; Disable interrupts
  push byte 4         ; Push a dummy error code (if ISR4 doesn't push it's own error code)
  push byte 4         ; Push the interrupt number (4)
  jmp isr_common_stub ; Go to our common handler.

 ; In isr.c
[EXTERN isr_handler]

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ; Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call isr_handler

   pop eax        ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   sti
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP 