#define STATE
#include "../include/kernel.h"

int kwait(int* status){
	proc* p; int hasChild = 0;
	kprintf("\nprocess %d wait childs\n", running->pid);
	while(1){
		for(int i=0;i<NPROC;i++){
			p = &process[i];
			if(p->status != FREE && p->ppid == running->pid){
				hasChild = 1;
				if(p->status == ZOMBIE){
					*status = p->exitCode;
					p->status = FREE;
					enqueue(&freeQueue, p);
					kprintf("\nprocess %d is died side her father\n", p->pid);
					numproc--;
					return p->pid;
				}
			}
		}
		if(!hasChild){
			kprintf("\nprocess %d not have child \n", running->pid);
			return -1;
		}
		ksleep((int)running);
	}
}
