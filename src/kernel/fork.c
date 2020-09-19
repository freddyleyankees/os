#define STATE
#include "../include/kernel.h"


proc* kfork(char* filename){
	int i;
	proc* p = get_proc(&freeQueue);							// get a new process from free process
	if(!p)
		return 0;
	numproc++;
	p->status = READY;										// set process as ready
	p->priority = 1;										// set child's process priority to 1
	p->kmode = 0;											// set child's process priority to 1
	p->lock = 0;											// set child's process priority to 1
	p->ktimer = 5;											// set parent process
	p->ppid = running->pid;									// set parent process
	p->parent = running;									// set parent process
	for(i = 1;i<=SSIZE;i++)
		p->kstack[SSIZE-i] = 0;
	p->kesp = &p->kstack[SSIZE-20];
	enqueue(&readyQueue, p);								// enter p into readyQueue by priority
	return p;
}
