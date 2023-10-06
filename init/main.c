#include "../include/kernel.h"
#include "../include/io.h"
#include "../include/string.h"
#include "../include/con.h"
#include "../include/syscall.h"
#include "../include/i386/gdt.h"
#include "../include/i386/int.h"
#include "../include/disk.h"
#include "../drivers/kbd/kbd.h"


void init(){
    init_();
    init_gdt();
    inidt();
    trap_init();
    initTimer(100);
    init_kbd();
    i_syscall();
    map_i386_active_irq();
    do_tswitch();
}

void main(struct boot_desc* sys)
{
    cursorEnable(0xE,0xF);
    clearCon();
    asm volatile("movl %0, %%esp; addl %1, %%esp"::"r"(process),"r"(procSize));
    kprintf("kernel %p\n", sys->startMemoryKernel);
    kprintf("initProc %p\n", sys->startMemoryInitProc);
    kprintf("idt %p\n", sys->gdt);
    kprintf("gdt %p\n", sys->idt);
    idt = (struct idt_desc*) sys->idt;
    gdt = (struct gdt_desc*) sys->gdt;
    
    init();
    kprintf("\nreturn to main %x %x\n", tss.ss0,  tss.esp0);

    return;
}