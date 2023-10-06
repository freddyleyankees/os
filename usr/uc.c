#include "../usr/include/stdio.h"

char* cmd [] = {"ps", "chname", "fork", "switch", "wait", "exit",0};

int smenu(){
	printf("\n*******************************\n");
    printf("*        command line         *\n");
    printf("*******************************\n");
}

int findCmd(char* name){
	int i=0; char *p = cmd[0];
	while(p){
		if(strcmp(p, name))
			return i;
		i++;
		p = cmd[i];
	}
	return -1;
}

void  main()
{
	char s[16] = {0,};
	//setColorText(pid+1);
    printf("\n*******************************\n");
    printf("*        command line         *\n");
    printf("*******************************\n");
    unsigned int a;
    asm volatile("movl %%ebp, %0":"=r"(a):);
    printf("RPC: %x\n", a);
    int pid = fork();
    
    asm volatile("movl %%ebp, %0":"=r"(a):);
    printf("| RPC main: %x |", a);
    printf("i am a process %d\n", getpid());
    exit();
}
