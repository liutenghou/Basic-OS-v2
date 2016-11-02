/* msg.c : messaging system 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>

/* Your code goes here */
int send(int dest_pid, int msg){
	kprintf(" frompid:%d - destpid:%d - msg:%d ", sysgetpid(), dest_pid, msg);
	//dest is dead, not found
	pcb *p = getPCBbyPID(dest_pid);

	if(p == NULL){
		kprintf("send: dest not exist\n");
		return -1;
	}
	if(p->state == STATE_STOPPED){
		kprintf("send: dest cannot receive\n");
		return -1;
	}
	//kprintf(" sysgetpid():%d ", sysgetpid());
	//destination is self
	if(dest_pid == sysgetpid()){
		kprintf("send: dest same as self\n");
		return -2;
	}
	//blocked
	if(p->state == STATE_BLOCKED){
		return -3;
	}



	return 0;
}

extern pcb* getPCBbyPID(int pid){
	pcb *p;
	int i;
	for(i=0; i<MAX_PROC; i++){
		if(proctab[i].pid == pid){
			//kprintf(" TOPIDINgetPCBbyPID:%d ",proctab[i].pid);
			return &proctab[i];
		}
	}
	return NULL;
}
