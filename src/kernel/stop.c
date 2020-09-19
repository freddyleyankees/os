#define STATE
#include "../include/kernel.h"


int kstop (int pid){
	if(!pid)
		return 0;
	kprintf("\nprocess %d is stopping !\n", pid);
	process[pid].status = STOP;
	do_tswitch();

}