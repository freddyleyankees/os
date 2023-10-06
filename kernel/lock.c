#define STATE
#include "../include/kernel.h"


int klock(){
	running->lock = 1;
	while(running->lock);
}