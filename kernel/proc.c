#define LIST
#define STATE
#define PROC

#include "../include/kernel.h"
#include "../include/string.h"
#include "../include/con.h"

void printList (proc* list){
	for(proc* j = list;j;j=j->next)
    	if(j == list)
    		kprintf("P%d ",j->pid);
    	else
    		kprintf("-> P%d ",j->pid);
    kprintf("\n");
}

int C(int z){
	int c = 4+z;
	kprintf("in B function\n");
	int pid = do_fork();
	if(pid == 0)
		kprintf(" i am a son \n");
	else
		kprintf(" i am a father\n");
	kprintf(" result is %d", c);
	do_tswitch();
	while(1);
}

int ign(){
	kprintf("all processs die kernel panic !\n");
	while(1);
}

proc* getrunning(){
    return running;
}

void init_(){
    proc *p;
    int i;
    kprintf("init ....\n");
    for(i=0;i<NPROC;i++){									// initialize all process
        p = &process[i];									// 
        p->pid = i;											// set process's pid
        p->status = FREE;									// mark process as FREE
        p->priority = 0;
        if(i>0)
        	p->offset = (p->pid-1)*0x10000 + SPROC;
        if(i<NPROC)											// set process's priority to 0
        	p->next = &process[i+1];
    }

    process[NPROC-1].next = 0;
    freeQueue = &process[0];
    readyQueue = 0;
    sleepQueue = 0;

    /****** create first process as running ****/
    p = get_proc(&freeQueue);
    p->status = READY;										// set init priority to 0
	p->ppid = 0;											// init's parent is itself
    running = p;											// init is now ready
    proc* init = kfork(0);
    unsigned int uflag = 0;
	asm volatile("pushfl; popl %%eax;movl %%eax, %0":"=r"(uflag)::"%eax");
	init->kstack[SSIZE-5] = SPROC;						// set eip address return [=> body] from kstack 
	init->kstack[SSIZE-4] = 0x23;						// set cs
	init->kstack[SSIZE-3] = (uflag|0x200)&0xFFFFBFFF;	// set eflags
	init->kstack[SSIZE-2] = init->pid*0x10000-1+SPROC;	// set uesp
	init->kstack[SSIZE-1] = 0x2B;						// set uss
	init->kstack[SSIZE-17] = 0x2B;						// set ds
	init->kstack[SSIZE-18] = 0x2B;						// set es
	init->kstack[SSIZE-19] = 0x2B;						// set fs
	init->kstack[SSIZE-20] = 0x2B;						// set gs
}
