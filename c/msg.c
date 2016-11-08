/* msg.c : messaging system 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>

/* Your code goes here */

/* kernel side: sends a message from process to another process
 * arguments: dest_pid: destination process pid, msg: an int to send
 * return: 0 on sucess, negative number on error
 *
 */
int send(int dest_pid, int msg){
	kprintf("frompid:%d - destpid:%d - msg:%d\n", sysgetpid(), dest_pid, msg);

	pcb *dest_p = getProcessFromPID(dest_pid);
	int sender_pid = sysgetpid();
	pcb *sender_p = getProcessFromPID(sender_pid);

	//dest is dead, not found
	if(dest_p == NULL){
		kprintf("send: dest not exist\n");
		return -1;
	}
	if(dest_p->state == STATE_STOPPED){
		kprintf("send: dest cannot receive\n");
		return -1;
	}
	//kprintf(" sysgetpid():%d ", sysgetpid());
	//destination is self
	if(dest_pid == sender_pid){
		kprintf("send: dest same as self\n");
		return -2;
	}

	//if receiver is not blocked on matched receive, block send until matched receive made
	if(dest_p->state != STATE_BLOCKED){
		//add p to receiver senderqueue
		if(dest_p->sender != NULL){
			pcb *temp = dest_p->nextSender;
			while(temp != NULL){
				temp = temp->nextSender;
			}
			temp = sender_p;
		}else{
			dest_p->sender = sender_p;
		}
		//block send
		sender_p->state = STATE_BLOCKED;
	//if receiver is blocked and therefore ready to receive message,
	//then message copied to receive buffer, place both processes both on ready queue
	}else{
		dest_p->msg = msg;
		dest_p->state = STATE_READY;
		sender_p->state = STATE_READY;
	}
	return 0;
}

// kernel side: receives call by dispatcher upon sysreceive request from user process
// arguments: srcPID: source proess pid, buffer: msg copied from sender into buffer, len: message length
// return: returns length 
int receive(int receiving_pid, int msg){
	kprintf("frompid:%d - srcpid:%d - msg:%d\n", sysgetpid(), srcPID, msg);
	
	// kernel should send the original sender's id from which the process receives from
	// and where it is sending to
	pcb *receiving_p = getProcessFromPID(receiving_p);	
	pcb *sender_p = getProcessFromPID(srcPID);
	// kernel needs to know where to send too
	int receiving_pid = sysgetpid();

	// if receiving pid does not exist
	if (receiving_pid == NULL) {
		return -1;
	}
	
	if (srcPID == receiving_pid) {
	kprintf("receive: cannot receive from itself \n");
	return -2;
	}
	
	// if receiver is called before the matching send, put it on block until matching sends occur
	if (receiving_p->state != STATE_BLOCKED) {
		if (receiving_p->sender != STATE_READY) {
		receiving_p->state = STATE_BLOCKED;
		// dont know what to do for it to keep waiting for send to be ready...
	} else {
		receiving_p->msg = msg;
		receiving_p->state = STATE_READY;
		sender_p->state = STATE_READY;
	}
	}
	// earliest outstanding send to receiving process (earliest unreceived send) is the matching send to receive... dont get this
	if (receiving_pid == 0) {
		return 0;
	}	
	
	
	return len;
}


