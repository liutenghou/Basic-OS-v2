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
int receive(pcb* this_process, int* srcPID, void* buffer, int len){
	
	pcb *src_process;
	int buff_limit;

	// can receive from any process 
	if (*srcPID == 0) {
		if (this_process->msg_queue != NULL) {
			kprintf("The send queue is not empty!");
			*buffer = this_process->msg_queue->msg;
		}
		kprintf("No matching pid");
		return -1;
	}

	// if recv() is called before matching send, the receiving process is blocked
	// until matching send occurs
	if (*srcPID != 0) {
		*src_process = getProcessFromPID(*srcPID);
		kprintf("receiving from %d to %d\n", *srcPID, this_process->pid);
	}

	// there is no matching pid
	if (scr_process == -1) {
		return -1;
	}

	// receiving process cannot send to itself
	if (this_process->pid == src_process->pid) {
		return -2;
	}

	// receiving from process that is receiving from you
	if (src_process) {
		if (src_process->state == RECV_BLOCKED) {
			if (src_process->buf->ipc_pid == this_process->pid) {
				return -4;
			}
		}
	}


	if (src_process->state == SEND_BLOCKED && src_process->buf->ipc_pid == this_process->pid && *srcPID != 0){

		// there is a match found, so proceed with the data transfer
		if (src_process->buf->size > len) {
			buff_limit = len;
		} else {
			buff_limit = src_process->buf->size;
		}

		// bcopy from xeros kernel header file
		// copy data
		//void bcopy(const void *src, void *dest, unsigned int n);
		bcopy(*buf, src_process->buf->addr, buff_limit);

		// unblock sending process
		ready(src_process);
		// then return received bytes indiciating the receiving process received something
		return buff_limit;

	 } else {
		kprintf("no senders\n");
		this_process->buf->ipc_pid = -1;
	}

	this_process->buf->addr = buffer;
	this_process->buf->size = len;
	this_process->state = RECV_BLOCKED;

	// source process not yet sending
	return -3;
}









