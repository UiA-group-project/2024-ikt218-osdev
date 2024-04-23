[EXTERN isr_handler_0]
[EXTERN isr_handler_1]
[EXTERN isr_handler_2]
[EXTERN isr_handler_3]
[EXTERN isr_handler_4]

[EXTERN irq_handler]

[GLOBAL isr0]
isr0:
  cli                 ; Disable interrupts
  push byte 0         ; Push a dummy error code (if ISR0 doesn't push it's own error code)
  push byte 0         ; Push the interrupt number (0)
  call isr_handler_0
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr1]
isr1:
  cli                 ; Disable interrupts
  push byte 1         ; Push a dummy error code (if ISR1 doesn't push it's own error code)
  push byte 1         ; Push the interrupt number (1)
  call isr_handler_1
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr2]
isr2:
  cli                 ; Disable interrupts
  push byte 2         ; Push a dummy error code (if ISR2 doesn't push it's own error code)
  push byte 2         ; Push the interrupt number (2)
  call isr_handler_2
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr3]
isr3:
  cli                 ; Disable interrupts
  push byte 3         ; Push a dummy error code (if ISR3 doesn't push it's own error code)
  push byte 3         ; Push the interrupt number (3)
  call isr_handler_3
  jmp isr_common_stub ; Go to our common handler.

[GLOBAL isr4]
isr4:
  cli                 ; Disable interrupts
  push byte 4         ; Push a dummy error code (if ISR4 doesn't push it's own error code)
  push byte 4         ; Push the interrupt number (4)
  call isr_handler_4
  jmp isr_common_stub ; Go to our common handler.

 ; In isr.c
[EXTERN isr_handler]

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
   pusha                    ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

   mov ax, ds               ; Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10             ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   mov eax, [esp + 36]      ; Move the interrupt number (on stack after pusha, error code, interrupt number) into eax
   push eax                 ; Push the interrupt number onto the stack for the isr_handler

   ;call isr_handler

   pop eax                  ; Cleanup the interrupt number from stack after isr_handler returns

   pop eax                  ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; Pops edi, esi, ebp...
   add esp, 8               ; Cleans up the pushed error code and pushed ISR number
   sti
   iret                     ; Returns from the interrupt

[GLOBAL IRQ0]
irq0:
  cli
  push byte 0
  push byte 0
  jmp irq_common_stub
  
[GLOBAL IRQ1]
irq1:
  cli
  push byte 1
  push byte 1
  jmp irq_common_stub

[GLOBAL IRQ2]
irq2:
  cli
  push byte 2
  push byte 2
  jmp irq_common_stub

[GLOBAL IRQ3]
irq3:
  cli
  push byte 3
  push byte 3
  jmp irq_common_stub
  
[GLOBAL IRQ4]
irq4:
  cli
  push byte 4
  push byte 4
  jmp irq_common_stub

irq_common_stub:
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ; Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call irq_handler

   pop ebx        ; reload the original data segment descriptor
   mov ds, bx
   mov es, bx
   mov fs, bx
   mov gs, bx

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   sti
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

