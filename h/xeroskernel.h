/* xeroskernel.h - disable, enable, halt, restore, isodd, min, max */

#ifndef XEROSKERNEL_H
#define XEROSKERNEL_H

/* Symbolic constants used throughout Xinu */

typedef char Bool; /* Boolean type                  */
typedef unsigned int size_t; /* Something that can hold the value of
 * theoretical maximum number of bytes
 * addressable in this architecture.
 */
#define	FALSE   0       /* Boolean constants             */
#define	TRUE    1
#define	EMPTY   (-1)    /* an illegal gpq                */
#define	NULL    0       /* Null pointer for linked lists */
#define	NULLCH '\0'     /* The null character            */

#define CREATE_FAILURE -1  /* Process creation failed     */

/* Universal return constants */

#define	OK            1         /* system call ok               */
#define	SYSERR       -1         /* system call failed           */
#define	EOF          -2         /* End-of-file (usu. from read)	*/
#define	TIMEOUT      -3         /* time out  (usu. recvtim)     */
#define	INTRMSG      -4         /* keyboard "intr" key pressed	*/
/*  (usu. defined as ^B)        */
#define	BLOCKERR     -5         /* non-blocking op would block  */

/* Functions defined by startup code */

void bzero(void *base, int cnt);
void bcopy(const void *src, void *dest, unsigned int n);
void disable(void);
unsigned short getCS(void);
unsigned char inb(unsigned int);
void init8259(void);
int kprintf(char * fmt, ...);
void lidt(void);
void outb(unsigned int, unsigned char);

/* Some constants involved with process creation and managment */

/* Maximum number of processes */
#define MAX_PROC        64           
/* Kernel trap number          */
#define KERNEL_INT      80
/* Minimum size of a stack when a process is created */
#define PROC_STACK      (4096 * 4)    

//A2 3.5 Timer entry point
#define TIMER_INT		32
#define SYSCALL_INT		33

/* Constants to track states that a process is in */
#define STATE_STOPPED   0
#define STATE_READY     1
#define STATE_BLOCKED	2


/* System call identifiers */
#define SYS_STOP        10
#define SYS_YIELD       11
#define SYS_CREATE      22

//a message package struct, hold message as well as process that is sending/receiving
//typedef struct msgPackage msgP;
//struct msgPackage{
//	pcb *p;
//	unsigned long msg;
//	pcb *nextPackage;
//};

/* Structure to track the information associated with a single process */
typedef struct struct_pcb pcb;
struct struct_pcb {
	int sending; //1 if sending, 0 if not
	int receiving; //1 if receiving, 0 if not
	unsigned long *esp; /* Pointer to top of saved stack           */
	pcb *next; /* Next process in the list, if applicable */
	int state; /* State the process is in, see above      */
	unsigned int pid; /* The process's ID                        */
	int ret; /* Return value of system call             */
	/* if process interrupted because of system*/
	/* call                                    */
	unsigned long msg; //received message from another process
	int *msg2;
	long args;
	pcb *sender;
	pcb *nextSender;
	pcb *receiver;
	pcb *nextReceiver;
	//NOTE: in order to check negative value errors, this must be signed
	long sleeptime; //counter for sleep on delta list
	pcb *nextSleeper; //next process on sleep queue
};

/* The actual space is set aside in create.c */
extern pcb proctab[MAX_PROC];

#pragma pack(1)

/* What the set of pushed registers looks like on the stack */
typedef struct context_frame {
	unsigned long edi;
	unsigned long esi;
	unsigned long ebp;
	unsigned long esp;
	unsigned long ebx;
	unsigned long edx; //20: va_list ap
	unsigned long ecx;
	unsigned long eax; //28: sys_req
	unsigned long iret_eip;
	unsigned long iret_cs;
	unsigned long eflags;
	unsigned long stackSlots[];
} context_frame;

/* Memory mangement system functions, it is OK for user level   */
/* processes to call these.                                     */

int kfree(void *ptr);
void kmeminit(void);
void *kmalloc(size_t);

/* A typedef for the signature of the function passed to syscreate */
typedef void (*funcptr)(void);

/* Internal functions for the kernel, applications must never  */
/* call these.                                                 */
void dispatch(void);
void dispatchinit(void);
void ready(pcb *p);
pcb *next(void);
void contextinit(void);
int contextswitch(pcb *p);
int create(funcptr fp, size_t stack);
void set_evec(unsigned int xnum, unsigned long handler);
void printCF(void * stack); /* print the call frame */
int syscall(int call, ...); /* Used in the system call stub */

//A2 sys call identifiers
#define SYS_GETPID 	34
#define SYS_PUTS 	35
#define SYS_KILL 	36
#define SYS_SEND 	37
#define SYS_RECEIVE	38
#define SYS_TIMER	39
#define SYS_SLEEP 	40

extern int sysgetpid(void);
extern void sysputs(char *str);
extern int syskill(int pid);

//3.3 IPC
extern int syssend(int dest_pid, unsigned long msg);
extern int sysrecv(int *from_pid, unsigned long *msg);
extern pcb* getProcessFromPID(int pid);
extern void blockedReady(pcb *p);

void killprocess(int pid);

int send(int toPID, unsigned long msg);
int receive(int *sender_pid, unsigned long *msg);
void putOnBlockedArray(pcb *p);
pcb* takeOffBlockedArray(int pid);

//helpers
int getCurrentPID(void);
pcb* getIdleProcPCB(void);
int getIdlePID(void);

//Idle Process Declaration
extern void idleproc(void);

//3.7 syssleep
extern unsigned int syssleep( unsigned long milliseconds );
unsigned int sleep(unsigned long millisec);
void tick(void);
pcb* getCurrentProcess(void); //helper function
void printSleepQueue(void); //testing function

/* Function prototypes for system calls as called by the application */
int syscreate(funcptr fp, size_t stack);
void sysyield(void);
void sysstop(void);

/* The initial process that the system creates and schedules */
void root(void);
void set_evec(unsigned int xnum, unsigned long handler);

/* Anything you add must be between the #


 and this comment */
#endif

