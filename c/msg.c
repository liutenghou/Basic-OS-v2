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
int receive(int sender_pid, unsigned long *msg){
	
	pcb *receiving_process=getCurrentProcess();
	pcb *sending_process=getProcessFromPID(sender_pid);
	int dest_pid = getCurrentPID();


	// can receive from any process 
	if (sender_pid == 0) {
		if (receiving_process->nextSender != NULL) {
			kprintf("The send queue is not empty!");
			*msg = receiving_process->msg;
		}
		kprintf("No matching pid");
		return -1;
	}

	// there is no matching pid
	if (sender_pid == -1) {
		return -1;
	}

	// if recv() is called before matching send, the receiving process is blocked
	// until matching send occurs
	if (sending_process->state != STATE_BLOCKED) {
		receiving_process->state == STATE_BLOCKED;
		return -1;
	}



	// receiving process cannot send to itself
	if (receiving_process->pid == sending_process->pid) {
		return -2;
	}

	if (sending_process->state == STATE_BLOCKED && receiving_process->sender->pid == sending_process->pid && sender_pid != 0){

		// bcopy from xeros kernel header file
		// copy data
		//void bcopy(const void *src, void *dest, unsigned int n);
		//bcopy(src_process->buf, buffer, src_process->msg);
		msg = receiving_process->msg;
		// unblock sending process
		ready(sending_process);
		// then return received bytes indiciating the receiving process received something
	

	 } else {
		kprintf("no senders\n");
		return -1;
	}

	//this_process->buf->addr = buffer;
	//this_process->buf->size = len;
	receiving_process->state = RECV_BLOCKED;

	// source process not yet sending
	return -3;
}









