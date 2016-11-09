/* user.c : User processes
 */

#include <xeroskernel.h>

/* Your code goes here */
void producer(void) {
	/****************************/

	int i;

	for (i = 0; i < 5; i++) {
		kprintf("P:%d\n", i);
		syssend(1, 101);

		sysyield();

	}
	syssleep(10);
	syssleep(15);
	syssleep(12); //should go to M
	syssleep(22);
	syssleep(3);
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

	kprintf("-P2-");
	sysstop();
}

void producer3(void) {
	/****************************/
	kprintf("-P3-");
	sysstop();
}

void producer4(void) {
	/****************************/
	kprintf("-P4-");
	sysstop();
}

void root(void) {
	/****************************/
	int proc_pid, con_pid, root_pid, proc_pid2, proc_pid3, proc_pid4;
	root_pid = sysgetpid();

	//why 2 yields here?
	sysyield();
	sysyield();
	proc_pid = syscreate(&producer, 4096);
	con_pid = syscreate(&consumer, 4096);
	proc_pid2 = syscreate(&producer2, 4096);
	proc_pid3 = syscreate(&producer3, 4096);
	proc_pid4 = syscreate(&producer4, 4096);
	kprintf(
			"RootPID = %d, P1PID = %d, ConPID:%d, P2PID:%d, P3PID:%d, P4PID:%d\n",
			root_pid, proc_pid, con_pid, proc_pid2, proc_pid3, proc_pid4);
	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}
