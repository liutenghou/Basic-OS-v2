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
	kprintf("p1sleep1done\n");
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p1msg:%d\n", msg);
	syssleep(msg);

	sysrecv(&senderpid, &msg);

	syssleep(msg);
	kprintf("p1msg2:%d\n", msg);
	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
	sysstop();
}

void process2(void) {
	/****************************/

	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("p2sleep1done\n");
	//receive from root process

	sysrecv(&senderpid, &msg);
	kprintf("p2msg:%d\n", msg);
	syssleep(msg);

	sysrecv(&senderpid, &msg);
	kprintf("p2msg2:%d\n", msg);
	//syssleep(msg);

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
	sysstop();
}

void process3(void) {
	/****************************/

	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("p3sleep1 done\n");
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p3msg:%d\n", msg);
	syssleep(msg);

	sysrecv(&senderpid, &msg);
	kprintf("3msg2:%d\n", msg);
	syssleep(msg);

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
	sysstop();
}

void process4(void) {
	/****************************/
	int senderpid = 1; //need this to get the sender_pid
	unsigned long msg = 0;
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("p4sleep1done\n");
	//receive from root process
	sysrecv(&senderpid, &msg);
	kprintf("p4msg:%d\n", msg);
	syssleep(msg);

	sysrecv(&senderpid, &msg);
	kprintf("p4msg2:%d\n", msg);
	syssleep(msg);

	kprintf("Process:%d sleep2done, exit\n", sysgetpid());
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

	sysputs("END");
	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
