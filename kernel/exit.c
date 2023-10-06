#define STATE
#include "../include/kernel.h"


int kexit (int e){
	proc*p; int wakeupp1 = 0;
	if(running->pid == 1 && numproc > 2){
		kprintf("\nother procs still, p1 can't die yet\n");
		return -1;
	}
	for(int i = 1; i<NPROC;i++){
		p = &process[i];
		if(p->status != FREE && p->ppid == running->pid){
			p->ppid = 1;
			p->parent = &process[1];
			wakeupp1++;
		}
	}
	running->exitCode = e;
	running->status = ZOMBIE;
	kwakeup((int)running->parent);
	if(wakeupp1){
		kwakeup((int)&process[1]);
	}
	do_tswitch();
	running->lock++;
	while(running->lock);
	return 0;
}
