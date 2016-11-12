/* user.c : User processes
 */

#include <xeroskernel.h>



//------------------------------------------------------------------------
//THIS SECTION FOR ASSIGNMENT 3.8

void process1(void) {

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
	int procPID1, procPID2, procPID3, procPID4, root_pid;
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
//
//	int proc3 = procPID3;
//	int proc4 = procPID4;
//	unsigned long *msg1=0;
//	unsigned long *msg2=0;
//
//	int ret4 = sysrecv(&proc4,&msg2);
//	kprintf("process 4 return:%d\n", ret4);
//
//	int ret3 = sysrecv(&proc3,&msg1);
//	kprintf("process 3 return:%d\n", ret3);



	int i;
	for (i = 0; i;) {
		sysyield();
	}
	sysstop();
}

//------------------------------------------------------------------------
/*

//Uncomment these three processes, and comment out above for testing

void testProcess1(void){ //3
    while(1)kprintf("1");
    sysstop();
}

void testProcess2(void){ //4
	while(1)kprintf("2");
	sysstop();
}

void root(void) { //1
    int t1, t2, t0;
    t0 = sysgetpid();
    int msg = 0;
	int pid = 3;

    t1 = syscreate(&testProcess1, 4096);
    t2 = syscreate(&testProcess2, 4096);

    kprintf("rootPID:%d, t1PID:%d, t2PID:%d\n", t0,t1,t2);
    //rootPID:1, t1PID:3, t2PID:4

    int i;
    for (i = 0; i;) {
        sysyield();
    }
    sysstop();
}
*/
