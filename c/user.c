/* user.c : User processes
 */

#include <xeroskernel.h>

int root_pid;

void process1(void) {
	/****************************/

	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(5000);
	kprintf("Process:%d back\n", sysgetpid());
	sysstop();
}

void process2(void) {
	/****************************/
	
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(10000);
	kprintf("Process:%d back\n", sysgetpid());
	sysstop();
}

void process3(void) {
	/****************************/
	
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(15000);
	kprintf("Process:%d back\n", sysgetpid());
	sysstop();
}

void process4(void) {
	/****************************/
	
	kprintf("Process:%d alive\n", sysgetpid());
	syssleep(20000);
	kprintf("Process:%d back\n", sysgetpid());
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

	kprintf("RootPID:%d, P1PID:%d, P2PID:%d, P3PID:%d, P4PID:%d\n",
				root_pid, proc_pid1, proc_pid2, proc_pid3, proc_pid4);

	kprintf("Process:%d alive\n", root_pid);

	int j;
	for(j=0; j<1000; j++){
		sysyield();
	}
	syssleep(25000);
	sysputs("back at kernel");

	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
