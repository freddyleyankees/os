#include "../include/kernel.h"
#include "../include/string.h"
#include "../include/io.h"
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
	cli;
	proc* p = kfork(0);
	if(!p)
		return -1;
	
	//*** change ds, es, fs, gs, cs, eflags, eax in child's kstack
	unsigned int uflag = 0;
	asm volatile("pushfl; popl %%eax;movl %%eax, %0":"=r"(uflag)::"%eax");
	unsigned int *esp;
	asm volatile("movl (%%ebp), %%eax; movl %%eax, %0":"=r"(esp)::"%eax");
	for(int i=0;i<20;i++) 
		kprintf("%x|", esp[i]);				// set cs
	return p->pid;
}

int do_fork(){
	cli;
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

	kprintf("kfork task num %x\n", p->pid);
	for (int i = 1; i <= SSIZE; ++i)
		p->kstack[SSIZE-i] = running->kstack[SSIZE-i];
	p->kmode = 1;
	p->kstack[SSIZE-20]=p->kstack[SSIZE-19]=p->kstack[SSIZE-18]=p->kstack[SSIZE-17]=p->kstack[SSIZE-1]=0x10;
	p->kstack[SSIZE-2] = &p->kstack[SSIZE-1];
	p->kstack[SSIZE-3] = 0x200;
	p->kstack[SSIZE-4] = 0x8;
	p->kstack[SSIZE-5] = &goUmode;
	p->kstack[SSIZE-9] = 0;
	p->kstack[SSIZE-14] = 0;
	p->kstack[SSIZE-15] = 0;
	p->kesp = running->kesp;
	return p->pid;
}

int do_exit(int exitValue){
	return kexit(exitValue);
}

int do_stop(int pid){
	return kstop(pid);
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
