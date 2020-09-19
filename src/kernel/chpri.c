#define STATE
#include "../include/kernel.h"

// change priority of a process
int kchPri(int pid, int priority){
	proc* p; int ok=0, req=0;
	if(pid == running->pid){
		running->priority = priority;
		if(priority < readyQueue->priority)
			rflag = 1;
		return 1;
	}
	for(int i = 0;i<NPROC; i++){
		p = &process[i];
		if(p->pid == pid && p->status != FREE){
			p->priority = priority;
			ok = 1;
			if(p->status == READY)
				req = 1;
		}
	}
	if(!ok){
		kprintf("changePriority failed\n");
		return -1;
	}
	if(req)
		reschedule();
	return priority;
}
