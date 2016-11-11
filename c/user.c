/* user.c : User processes
 */

#include <xeroskernel.h>

int root_pid;

void process1(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("sleep1done\n");
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p1msg:%d ", msg);
	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
	sysstop();
}

void process2(void) {
	/****************************/

	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("sleep1done\n");
	//receive from root process
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p2msg:%d ", msg);
	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
	sysstop();
}

void process3(void) {
	/****************************/

	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("sleep1done\n");
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p3msg:%d ", msg);
	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
	sysstop();
}

void process4(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("sleep1done\n");
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p4msg:%d ", msg);
	//prints a message that msg received, how long left to sleep

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
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

	syssend(proc_pid1, 1000);
	syssend(proc_pid2, 2000);
	syssend(proc_pid3, 3000);
	syssend(proc_pid4, 4000);

	syssleep(9000);
	sysputs("back at kernel\n");

	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
