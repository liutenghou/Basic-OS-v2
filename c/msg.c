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
int receive(int *srcPID, void *buffer, int len){
	kprintf("Receiving message from sender");

	// why pointer to srcPID? from the struct?
	pcb->srcPID = sysgetpid();
	// receiving process sender
	pcb *receiving_p = getProcessFromPID(receiving_p);

	// if srcPID does not exist
	if (srcPID == NULL) {
		return -1;
	}

	// if sender is waiting for msg from receiver (so basically it is blocked)
	if (srcPID->state == STATE_BLOCKED) {
		// sender's msg copied into buffer from receiving process
		buffer = srcPID->msg;
		// sender than put onto the ready queue
		srcPID->state = STATE_READY;
	}

	// if no process abailable to send
	else { 
		if (srcPID->state != STATE_READY) {
			receiving_p->state = STATE_BLOCKED;

	}
	// kill sender process
	else { if (srcPID->state != STATE_BLOCKED) {
		pcb->sender = killprocess(srcPID);
		pcb->msg = 0;
		kprintf("Message will never arrived");
		return -1;
	}

	}

	}


	return len;
}


