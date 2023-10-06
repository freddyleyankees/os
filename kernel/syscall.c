#define IRQ
#include "../include/i386/int.h"
#include "../include/kernel.h"
#include "../include/syscall.h"
#include "../include/con.h"

static void sc(reg_t* reg){
	unsigned int r;
	        unsigned int a,b;
        asm volatile("movl %%ebp, %%eax; movl 8(%%eax), %0; movl 4(%%eax), %1":"=r"(a),"=r"(b):);
        kprintf("stack int %x %x %x|",reg, a, b);
	if(reg->eax == 0)
		r = kgetpid();
	else if(reg->eax == 1)
		r = kgetppid();
	else if(reg->eax == 3)
		r = do_fork();
	else if(reg->eax == 4)
		r = do_stop(reg->ebx);
	else if(reg->eax == 5)
		r = do_continu();
	else if(reg->eax == 6)
		r = do_sleep(reg->ebx);
	else if(reg->eax == 7)
		r = do_wakeup(reg->ebx);
	else if(reg->eax == 8)
		r = do_wait((int*)reg->ebx);
	else if(reg->eax == 9)
		r = do_chPri();
	else if(reg->eax == 10)
		r = do_exit(reg->ebx);
	else if(reg->eax == 11)
		do_tswitch();
	else if(reg->eax == 12)
		r = do_lock();
	else if(reg->eax == 13)
		r = do_getc();
	else if(reg->eax == 14)
		r = do_geti();
	else if(reg->eax == 15)
		r = reg->cs;
	else if(reg->eax == 16)
		r = do_gets(reg->ebx);
	else if(reg->eax == 17)
		r = getesp();
	else if(reg->eax == 2){
		print_con((const char*)reg->ebx);
	}
	return reg->eax = r;
}

void i_syscall(){
	reg_handler(IRQ16, &sc);
	return;
}
