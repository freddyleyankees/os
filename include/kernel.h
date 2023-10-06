#include "../include/types.h"
#define SSIZE 1024
#define NPROC 9
#define SPROC 0x200000
typedef struct proc 
{
    struct proc* next;
    unsigned int *kesp;
    unsigned int lock;
    unsigned int kmode;
    unsigned int offset;
    unsigned int ktimer;
    int pid;							// process ID number
    int ppid;							// parent ID process
    int event;							// sleep event 
    int status;							// state process = FREE|READY|STOP|DEAD|RUNNING|ZOMBIE
    struct proc* parent;						// pointer to parent process
    int priority;
    int exitCode;						// exit code
    int kstack[SSIZE];                  // utack kernel of the process
} __attribute__ ((packed)) proc;


#ifdef STATE
#define FREE	0
#define READY	1
#define STOP	2
#define DEAD	3
#define SLEEP	4
#define ZOMBIE	5
#endif

proc* get_proc(proc**);					// return a FREE process pointer from list
int put_proc(proc**, proc*);			// Enter process pointer into list
int enqueue(proc**, proc*);				// Enter process pointer into queue by priority
proc* dequeue(proc**);					// return first process pointer remove from queue

#ifdef PROC
int numproc, xmode = 0, rflag=0;
unsigned int proc_off = 0;
unsigned char _tswitch = 0;
unsigned int procSize = sizeof(proc);
proc process[NPROC], *running, *readyQueue, *freeQueue, *sleepQueue;
#else
extern unsigned char _tswitch;
extern int xmode, numproc, rflag;
extern unsigned int proc_off, procSize;
extern proc process[NPROC],*running,*running, *readyQueue, *freeQueue, *sleepQueue;
#endif
int kprintf(const char *fmt, ...);
int geti();
char getc();
int do_geti();
char do_getc();
void init();
int chkmode();
void printList (proc* list);
void scheduler(unsigned int*);
int reschedule();
int body();
void init();
void initTimer(unsigned int freq);
void init_();
unsigned int get_t();
int kgetpid();
int kgetppid();
proc *kfork(char*);
int kexit (int);
int kstop (int);
int kwakeup (int);
int ksleep (int);
int kwait (int*);
int kcontinu (int);
int klock ();
int kchPri(int, int);

void do_tswitch(void);
int do_fork();
int do_exit(int);
int do_stop();
int do_continu();
int do_sleep();
int do_wakeup();
int do_wait();
int do_chPri();
int do_lock();

#define setss(a) asm volatile("movw %0, %%ax; movw %%ax, %%ss"::"r"(a));
#define getss() ({      \
    unsigned int _a;    \
    asm volatile("movw %%ss, %%ax; movl %%eax, %0":"=r"(_a):);  \
    _a;     \
    })
#define getcs() ({      \
    unsigned int _a;    \
    asm volatile("movw %%cs, %%ax; movl %%eax, %0":"=r"(_a):);  \
    _a;     \
    })
#define setesp(a) asm volatile("movl %0, %%eax; movl %%eax, %%esp"::"r"(a));
#define getesp() ({     \
    unsigned int _a;    \
    asm volatile("movl %%esp, %%eax; movl %%eax, %0":"=r"(_a):);    \
    _a;     \
    })
extern int geteip(void);
extern void goUmode(unsigned int*);