#define STATE
#include "../include/kernel.h"
#include "../include/io.h"
#include "../include/i386/gdt.h"

// schedule ready process list

int get_tswitch(){ 
	return _tswitch;
}

void scheduler(unsigned int* reg){
	if(_tswitch){
		kprintf("scheduler /... %d\n", running->pid);
		if(running->status == READY){
	    	enqueue(&readyQueue, running);
		}
	    running = dequeue(&readyQueue);
	    running->lock = 0;
	    kprintf("scheduler /... %d, %x\n", running->pid, tss.esp0);
	    _tswitch = 0;
	   	goUmode((unsigned int*)running->kesp);;
	}
	return;
}

// reschedule ready process list if rflag is 1
int reschedule(){
	proc* p, *tmp = 0;
	while( p = dequeue(&readyQueue) ){
		enqueue(&tmp, p);
	}
	readyQueue = tmp;
	rflag = 0;
	if(running->priority < readyQueue->priority)
		rflag = 1;
}
