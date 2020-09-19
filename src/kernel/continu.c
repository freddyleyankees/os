#define STATE
#include "../include/kernel.h"

int kcontinu (int pid){
	proc* p;
	for(int i = 0;i<NPROC; i++){
		p = &process[i];
		if(i == pid)
			if(process[i].status == STOP || process[i].status == DEAD){
				process[i].status = READY;
				enqueue(&readyQueue, &process[i]);
				kprintf("\nproc id %d continue \n", pid);
			}
	}
}
