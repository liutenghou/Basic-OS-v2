/* user.c : User processes
 */

#include <xeroskernel.h>

int root_pid;

void process1(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 2000;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);

	sysrecv(&senderpid, &msg);
	//receive from root process

	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep done, exit\n", sysgetpid());
	sysstop();
}

void process2(void) {
	/****************************/

	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 4000;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	//receive from root process

	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep done, exit\n", sysgetpid());
	sysstop();
}

void process3(void) {
	/****************************/

	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 6;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	//receive from root process

	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep done, exit\n", sysgetpid());
	sysstop();
}

void process4(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 8;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	//receive from root process

	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep done, exit\n", sysgetpid());
	sysstop();
}

void root(void) {
	/****************************/
	int proc_pid1, proc_pid2, proc_pid3, proc_pid4;
	root_pid = sysgetpid();
	char *str;

	proc_pid1 = syscreate(&process1, 4096);
	proc_pid2 = syscreate(&process2, 4096);
	proc_pid3 = syscreate(&process3, 4096);
	proc_pid4 = syscreate(&process4, 4096);

	kprintf("RootPID:%d, P1PID:%d, P2PID:%d, P3PID:%d, P4PID:%d\n", root_pid,
			proc_pid1, proc_pid2, proc_pid3, proc_pid4);

	kprintf("Process:%d alive\n", root_pid);

	int j;
	for (j = 0; j < 10; j++) {
		sysyield();
	}

//	syssend(2, 1000);
//	syssend(3, 2000);
//	syssend(4, 3000);
//	syssend(5, 4000);

	syssleep(9000);
	sysputs("back at kernel\n");

	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
