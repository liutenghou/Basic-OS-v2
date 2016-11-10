/* user.c : User processes
 */

#include <xeroskernel.h>

/* Your code goes here */
void producer(void) {
	/****************************/

	int i;

	for (i = 0; i < 5; i++) {
		kprintf("P:%d\n", i);

		sysyield();
	}
	sysstop();
}

void consumer(void) {
	/****************************/

	int i;

	for (i = 0; i < 5; i++) {
		//sysputs("consume\n");
		kprintf("C:%d\n", i);
		sysyield();
	}

	sysstop();
}

void producer2(void) {
	/****************************/

	
	syssleep(1000);
	kprintf("-P2:");
	sysstop();
}

void producer3(void) {
	/****************************/
	
	syssleep(3000);
	kprintf("-P3:");
	sysstop();
}

void producer4(void) {
	/****************************/
	
	syssleep(5000);
	kprintf("-P4:");
	sysstop();
}

void producer5(void) {
	/****************************/
	
	syssleep(7000);
	kprintf("-P5:");
	sysstop();
}
void producer6(void) {
	/****************************/
	
	syssleep(9000);
	kprintf("-P6:");
	sysstop();
}

void root(void) {
	/****************************/
	int proc_pid, con_pid, root_pid, proc_pid2, proc_pid3, proc_pid4, proc_pid5, proc_pid6;
	root_pid = sysgetpid();

	//why 2 yields here?
//	sysyield();
//	sysyield();
//	proc_pid = syscreate(&producer, 4096);
//	con_pid = syscreate(&consumer, 4096);
	proc_pid2 = syscreate(&producer2, 4096);
	proc_pid3 = syscreate(&producer3, 4096);
	proc_pid4 = syscreate(&producer4, 4096);
	proc_pid5 = syscreate(&producer5, 4096);
	proc_pid6 = syscreate(&producer6, 4096);
	kprintf(
			"RootPID = %d, P1PID = %d, ConPID:%d, P2PID:%d, P3PID:%d, P4PID:%d\n, P5PID:%d, P6PID:%d.",
			root_pid, proc_pid, con_pid, proc_pid2, proc_pid3, proc_pid4, proc_pid5, proc_pid6);
	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
