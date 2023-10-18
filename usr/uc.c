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
    
    int pid = fork();
    
    printf("i am a process %d\n", getpid());
    exit();
}
