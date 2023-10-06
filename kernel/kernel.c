#include "../include/kernel.h"
#include "../include/string.h"
#include "../include/i386/gdt.h"
static unsigned char bufkbd[1024] = {0,};
extern unsigned char eventKeypressAscii();
int kgetpid(){
	return running->pid;
}

int kgetppid(){
	return running->ppid;
}


void do_tswitch(){
	kprintf("\nswitch !\n");
    _tswitch++;
}

int do_vfork(){
	proc* p = kfork(0);
	if(!p)
		return -1;
	
	//*** change ds, es, fs, gs, cs, eflags, eax in child's kstack
	unsigned int uflag = 0;
	asm volatile("pushfl; popl %%eax;movl %%eax, %0":"=r"(uflag)::"%eax");
	unsigned int *esp;
	asm volatile("movl (%%ebp), %%eax; movl %%eax, %0":"=r"(esp)::"%eax");
	for(int i=0;i<20;i++) 
		kprintf("%x|", esp[i]);
	p->kstack[SSIZE-5] = esp;						// set eip address return [=> body] from kstack 
	p->kstack[SSIZE-4] = 0x23;						// set cs
	p->kstack[SSIZE-3] = (uflag|0x200)&0xFFFFBFFF;	// set eflags
	p->kstack[SSIZE-2] = running->kesp;				// set uesp
	p->kstack[SSIZE-1] = 0x2B;						// set uss
	p->kstack[SSIZE-9] = 0x0;						// set eax
	p->kstack[SSIZE-20] = 0x2B;						// set ds
	p->kstack[SSIZE-19] = 0x2B;						// set es
	p->kstack[SSIZE-18] = 0x2B;						// set fs
	p->kstack[SSIZE-17] = 0x2B;						// set gs
	return p->pid;
}

int do_fork(){
	proc* p = kfork(0);
	if(!p){
		kprintf("kfork failed \n");
		return -1;
	}
	// load segment
	kprintf("kfork copy image from segment %x to %x\n", running->offset, p->offset);
	memcpy(p->offset, running->offset, 0x10000);		// copy 64K bytes
	//*** change ds, es, fs, gs, cs, eflags, eax in child's kstack
	unsigned int uflag = 0;
	asm volatile("pushfl; popl %%eax;movl %%eax, %0":"=r"(uflag)::"%eax");
	unsigned int *esp, *r;
	/*|ss|esp|eflags|cs|rpc|ebp|rpc|ebp|d|c|b|a|rpc2|ebp2|20-regs|esp|irqh|ebp|reg|h|ebp|reg|do_fork|ebp|*/
	asm volatile("movl %%ebp, %%eax; movl %%eax, %0":"=r"(esp)::); 
	r = *(esp+4);
	kprintf("kfork resume to %x, %x and %x\n", esp, r, tss.esp0);
	kprintf("kstack resume to %x\n", *(esp+4));
	for (int i = 0; i < 20; ++i)
	{
		kprintf("%x|", *r++);
	}
	for (int i = 1; i <= SSIZE; ++i)
	{
		p->kstack[SSIZE-i] = running->kstack[SSIZE-i];
	}
	p->kstack[SSIZE-5] = *(r+15);						// set eip address return [=> body] from kstack 
	p->kstack[SSIZE-4] = 0x23;						// set cs
	p->kstack[SSIZE-3] = (uflag|0x200)&0xFFFFBFFF;	// set eflags
	p->kstack[SSIZE-2] = *(r+18);		// set uesp
	p->kstack[SSIZE-1] = 0x2B;						// set uss
	p->kstack[SSIZE-9] = 0x0;						// set eax
	p->kstack[SSIZE-14] = *(r+6);						// set eax
	p->kstack[SSIZE-13] = *(r+7);						// set eax
	p->kstack[SSIZE-20] = 0x2B;						// set ds
	p->kstack[SSIZE-19] = 0x2B;						// set es
	p->kstack[SSIZE-18] = 0x2B;						// set fs
	p->kstack[SSIZE-17] = 0x2B;						// set gs
	return p->pid;
}

int do_exit(int exitValue){
	return kexit(exitValue);
}

int do_stop(int pid){
	kstop(pid);
}

int do_continu(){
	int pid;
	kprintf("\ninput pid: ");
	pid = (int) do_geti();
	kcontinu(pid);
}
int do_geti(){
	unsigned char c;
	c = eventKeypress();
	if(c>0x30 && c<0x39){
		c = c-'0';
		kprintf("%c",c);
		return c;
	}
	c = 0;
	return c;
}
char do_getc(){
	unsigned char c;
	c = eventKeypress();
	if(c>0x20 && c<0xFF){
		kprintf("%c",c);
		return c;
	}
	c = 0;
	return c;
}

char* do_gets(char* s){
	char *_s = s;
	do{
		*_s = eventKeypress();
		kprintf("%c",*_s);
	}while(*_s++ != '\n');
	*s = 0;
	return s;
}

int do_sleep(){
	ksleep(123);
}

int do_wakeup(){
	kwakeup(123);
}

int do_wait(int *status){
	kwait((int*)proc_off);
}

int do_lock(){
	klock();
}

int do_chPri(){
	int pid, priority;
	kprintf("\ninput pid: ");
	pid = (int) do_geti();
	kprintf(" input new priority ");
	priority = (int) do_geti();
	if(priority<1)
		priority = 1;
	return kchPri(pid, priority);
}
