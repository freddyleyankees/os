extern isr_handler, irq_handler, getrunning, kprintf, scheduler, get_tswitch, itoa,listItems

global  isr_common_int, irq_common_int, asm_irq_ign, asm_isr_def,  \
divideZero, debug, nonMask, int3, overflow, bounds, invalidop, noCop, doubleFault, \
coSeg, invalidTSS, noSeg, stackFault, GP, pageFault, reserve, copError, goUmode

%macro   INT_ALL 0
   pushad
   push ds
   push es
   push fs
   push gs
   call getrunning
   mov dword[eax+4], esp
   inc dword[eax+8]
   mov ax,0x10
   mov ds,ax
   mov es,ax
   mov ss,ax
   mov fs,ax
   mov gs,ax
   call common_int
   jmp goUmode
%endmacro

goKmode:
   pop gs
   pop fs
   pop es
   pop ds
   popad
   add esp, 12
   iret

goUmode:
   cli
   call getrunning
   dec dword[eax+8]
   cmp dword[eax+8], 0
   jg goKmode
   call scheduler
   cmp eax, 0
   je goKmode
   mov esp, eax
   jmp goKmode


common_int:
   push ebp
   mov ebp, esp
   mov esi, ebp
   add esi, 8
   push esi
   cmp dword[esi+4*13], 0x20
   jb .isr_h
   jmp .irq_h
   .isr_h:
      call isr_handler
      jmp .done
   .irq_h:
      call irq_handler
      jmp .done
   .done:
   mov esp, ebp
   pop ebp
   ret


;routine exception

%macro ISR_HANDLER_NOERRCODE 1  
   [GLOBAL asm_isr_%1]
   asm_isr_%1:
      push byte 0
      push byte %1
      push byte 0x0
      INT_ALL
%endmacro

%macro ISR_HANDLER_ERRCODE 1
   [GLOBAL asm_isr_%1]
   asm_isr_%1:
      push byte %1
      push byte 0x0
      INT_ALL
%endmacro


%macro IRQ_HANDLER 2  
   [GLOBAL asm_irq_%1]
   asm_irq_%1:
      push byte 0
      push byte %2
      push byte 0x0
      INT_ALL
%endmacro

ISR_HANDLER_NOERRCODE 0
ISR_HANDLER_NOERRCODE 1
ISR_HANDLER_NOERRCODE 2
ISR_HANDLER_NOERRCODE 3
ISR_HANDLER_NOERRCODE 4
ISR_HANDLER_NOERRCODE 5
ISR_HANDLER_NOERRCODE 6
ISR_HANDLER_NOERRCODE 7
ISR_HANDLER_ERRCODE 8
ISR_HANDLER_NOERRCODE 9
ISR_HANDLER_ERRCODE 10
ISR_HANDLER_ERRCODE 11
ISR_HANDLER_ERRCODE 12
ISR_HANDLER_ERRCODE 13
ISR_HANDLER_ERRCODE 14
ISR_HANDLER_NOERRCODE 15
ISR_HANDLER_NOERRCODE 16
ISR_HANDLER_NOERRCODE 17
ISR_HANDLER_NOERRCODE 18
ISR_HANDLER_NOERRCODE 19
ISR_HANDLER_NOERRCODE 20
ISR_HANDLER_NOERRCODE 21
ISR_HANDLER_NOERRCODE 22
ISR_HANDLER_NOERRCODE 23
ISR_HANDLER_NOERRCODE 24
ISR_HANDLER_NOERRCODE 25
ISR_HANDLER_NOERRCODE 26
ISR_HANDLER_NOERRCODE 27
ISR_HANDLER_NOERRCODE 28
ISR_HANDLER_NOERRCODE 29
ISR_HANDLER_NOERRCODE 30
ISR_HANDLER_NOERRCODE 31

IRQ_HANDLER 0, 32
IRQ_HANDLER 1, 33
IRQ_HANDLER 2, 34
IRQ_HANDLER 3, 35
IRQ_HANDLER 4, 36
IRQ_HANDLER 5, 37
IRQ_HANDLER 6, 38
IRQ_HANDLER 7, 39
IRQ_HANDLER 8, 40
IRQ_HANDLER 9, 41
IRQ_HANDLER 10, 42
IRQ_HANDLER 11, 43
IRQ_HANDLER 12, 44
IRQ_HANDLER 13, 45
IRQ_HANDLER 14, 46
IRQ_HANDLER 15, 47
IRQ_HANDLER 16, 96

divideZero:
debug:
nonMask:
int3:
overflow:
bounds:
invalidop:
noCop:
doubleFault:
coSeg:
invalidTSS:
noSeg:
stackFault:
GP:
pageFault:
reserve:
copError:
push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push dword[edi+11*4]
   push err
   call kprintf
   hlt
   mov esp, ebp
   pop ebp
ret

[GLOBAL geteip]
geteip:
  pop eax
  jmp eax 
err: db 10,"ERROR please debug code %x",10,0
errt: db 10,"tswitch ERROR please debug",10,0
val: db 10,"int %x",10,0
pid: db 10,"proc pid %x",10,0
stack: db 10,"kstack interrupt %x",10,0
reg: times 20 dd 0