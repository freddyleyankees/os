#define STATE
#include "../include/kernel.h"


int kwakeup(int evt){
	proc* p;int i;
	for (i=1;i<NPROC;i++){
		p = &process[i];
		if(p->status == SLEEP && p->event == evt){
			p = dequeue(&sleepQueue);
			p->status = READY;
			p->event = 0;
			enqueue(&readyQueue, p);
			kprintf("\nproc id %d wakeup \n", p->pid);
		}
	}
}