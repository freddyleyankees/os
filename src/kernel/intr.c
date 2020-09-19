#include "../include/kernel.h"
#include "../include/i386/int.h"
#include "../include/io.h"

isr_t handler[INTMAX];

void reg_handler(unsigned int n, isr_t h){
	handler[n] = h;
	return;
}

void isr_handler(reg_t *reg){
	kprintf("interrupt isr handler int %x", reg->num);
	
	if (handler[reg->num] != 0)
    {
        isr_t h = handler[reg->num];
        h(reg);
    }
    return;
}

void irq_handler(reg_t *reg){
	if (handler[reg->num] != 0)
	{
		isr_t h = handler[reg->num];
		h(reg);
	}
	if (reg->num >= 40)
	{
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
	return;
}