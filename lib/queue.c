#define LIST
#include "../include/kernel.h"

// return a FREE process pointer from list
proc* get_proc(proc** list){		// list->freeList->p0->p1->p2.....->pN
	return	dequeue(list);
}

// Enter process pointer into list
int put_proc(proc** list, proc* p){
	enqueue(list, p);
}

// Enter process pointer into queue by priority
int enqueue(proc** queue, proc* p){
	proc** head = queue;
	proc* tmp;
	if(!*head){
		if(!p){
			return 0;
		}
		else{
			*head = p;
			p->next = 0;
		}
	}
	else{
		
		proc* tmp = 0;
		for(proc* i = *head;i;i = i->next){
			if(i->priority < p->priority){
				p->next = i;
				if(!tmp)
					*head = p;
				else
					tmp->next = p;
				return 1;
			}
			tmp = i;
		}
		tmp->next = p;
		p->next = 0;
		return 1;
	}
}

// return first process pointer remove from queue
proc* dequeue(proc** queue){
	proc** head = queue, *p;
	if(!*head)
		return 0;
	p = *head;
	*head = p->next;
	return p;
}