/* user.c : User processes
 */

#include <xeroskernel.h>

int root_pid;

void process1(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	int pid = sysgetpid();
	kprintf("Process:%d alive\n", pid);
	syssleep(5000);
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep1 done, slept:%d ms exit\n", pid, msg);

	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep2 done, slept:%d ms exit\n", pid, msg);
	sysstop();
}

void process2(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	int pid = sysgetpid();
	kprintf("Process:%d alive\n", pid);
	syssleep(5000);
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep1 done, slept:%d ms exit\n", pid, msg);

	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep2 done, slept:%d ms exit\n", pid, msg);
	sysstop();
}

void process3(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	int pid = sysgetpid();
	kprintf("Process:%d alive\n", pid);
	syssleep(5000);
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep1 done, slept:%d ms exit\n", pid, msg);

	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep2 done, slept:%d ms exit\n", pid, msg);
	sysstop();
}

void process4(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	int pid = sysgetpid();
	kprintf("Process:%d alive\n", pid);
	syssleep(5000);
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid, msg);
	syssleep(msg);
	kprintf("Process:%d sleep1 done, slept:%d ms exit\n", pid, msg);

	sysrecv(&senderpid, &msg);
	kprintf("Process:%d message received sleep for: %d ms\n", pid,msg);
	syssleep(msg);
	kprintf("Process:%d sleep2 done, slept:%d ms exit\n", pid, msg);
	sysstop();
}

void root(void) {
	/****************************/
	int procPID1, procPID2, procPID3, procPID4;
	root_pid = sysgetpid();

	procPID1 = syscreate(&process1, 4096);
	procPID2 = syscreate(&process2, 4096);
	procPID3 = syscreate(&process3, 4096);
	procPID4 = syscreate(&process4, 4096);

	kprintf("RootPID:%d, P1PID:%d, P2PID:%d, P3PID:%d, P4PID:%d\n", root_pid,
			procPID1, procPID2, procPID3, procPID4);

	kprintf("Process:%d (root) alive\n", root_pid);

	int j;
	for (j = 0; j < 10; j++) {
		sysyield();
	}

	syssend(procPID1, 1000);
	syssend(procPID2, 2000);
	syssend(procPID3, 3000);
	syssend(procPID4, 4000);

	syssleep(13000);
	sysputs("kernel: done sleep\n");
	syssend(procPID3, 10000);
	syssend(procPID2, 7000);
	syssend(procPID1, 20000);
	syssend(procPID4, 27000);


	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
