#define STATE
#include "../include/kernel.h"

int ksleep(int evt){
	running->event = evt;
	running->status = SLEEP;
	enqueue(&sleepQueue, running);
	kprintf("\nproc id %d sleep wiht event %d \n", running->pid, evt);
	do_tswitch();
}