/* msg.c : messaging system 
 This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>

//TODO: change msg to be buffer in sender instead of receiver, makes more sense

/* kernel side: sends a message from process to another process
 * arguments: dest_pid: destination process pid, msg: an int to send
 * return: 0 on sucess, negative number on error
 *
 */
int send(int dest_pid, int msg) {
	kprintf("frompid:%d - destpid:%d - msg:%d\n", sysgetpid(), dest_pid, msg);

	pcb *dest_p = getProcessFromPID(dest_pid);
	int sender_pid = sysgetpid();
	pcb *sender_p = getProcessFromPID(sender_pid);

	//dest is dead, not found
	if (dest_p == NULL) {
		kprintf("send: dest not exist\n");
		return -1;
	}
	if (dest_p->state == STATE_STOPPED) {
		kprintf("send: dest cannot receive\n");
		return -1;
	}
	//kprintf(" sysgetpid():%d ", sysgetpid());
	//destination is self
	if (dest_pid == sender_pid) {
		kprintf("send: dest same as self\n");
		return -2;
	}

	//if receiver is not blocked on matched receive, block send until matched receive made
	if (dest_p->state != STATE_BLOCKED) {
		//add p to receiver senderqueue
		if (dest_p->sender != NULL) {
			pcb *temp = dest_p->nextSender;
			while (temp != NULL) {
				temp = temp->nextSender;
			}
			temp = sender_p;
		} else {
			dest_p->sender = sender_p;
		}
		//block send
		sender_p->state = STATE_BLOCKED;
		//if receiver is blocked and therefore ready to receive message,
		//then message copied to receive buffer, place both processes both on ready queue
	} else {

		// dont you actually want to put the process both on the queue and than change name as well?
		dest_p->msg = msg;
		//ready(dest_p);
		//ready(sender_p);
		dest_p->state = STATE_READY;
		sender_p->state = STATE_READY;
	}
	return 0;
}

//process receive message
//arguments: sender pid pointer, message pointer in user space
//returns -4 if receiver blocked, waiting on sender
int receive(int *senderPID, unsigned long *msg) {

	kprintf("|sender_pid:%d, msg:%d |", *senderPID, *msg);
	pcb *receiverProcess = getCurrentProcess();

	// receiving process cannot receive from itself
	if (receiverProcess->pid == *senderPID) {
		kprintf("error receiver():cannot receive from self\n");
		return -2;
	}
	//check if sender process exists
	pcb *senderProcess;
	if(*senderPID != 0){
		senderProcess = getProcessFromPID(*senderPID);
		if (senderProcess == NULL) {
			kprintf("error receiver(): no process found for pid\n");
			return -1;
		}
	}

	//other errors return -3
	if(*senderPID<0){
		kprintf("error receiver()\n");
		return -3;
	}

	//check senderqueue, if nothing on it, block self

	//receive from any,
	if (*senderPID == 0) {
		kprintf("receive() any\n");
		//check sender queue, if nothing on it block
		if (receiverProcess->sender == NULL) {
			receiverProcess->state = STATE_BLOCKED;
			return -4;
			//do receive again
		//NOTE: if on sending queue, sender must be blocked
		//sending queue not empty, transfer message immediately
		} else {
			//receive message
			*msg = receiverProcess->sender->msg;
			*senderPID = receiverProcess->sender->pid;
			//sending process has to be blocked, unblock it, back on readyqueue
			ready(receiverProcess->sender);
			//remove sender from senderqueue
			receiverProcess->sender = NULL;

			//move next sender if exists
			if (receiverProcess->nextSender != NULL) {
				receiverProcess->sender = receiverProcess->nextSender;
				receiverProcess->nextSender =
						receiverProcess->nextSender->nextSender;
			}
			ready(receiverProcess);
		}

	}else{
		kprintf("receive() pid:%d - ", *senderPID);
		//check sender, see if on senderqueue, if so receive it
		if(receiverProcess->sender->pid == *senderPID){
			*msg = receiverProcess->sender->msg;
			*senderPID = receiverProcess->sender->pid;


		}else if(receiverProcess->nextSender != NULL){ //check rest of queue
			kprintf("hasNextSender ");
			pcb *temp = receiverProcess->nextSender;
			while(temp != NULL){
				if(temp->pid == *senderPID){
					*msg = receiverProcess->sender->msg;
					*senderPID = receiverProcess->sender->pid;
					//sending process has to be blocked, unblock it, back on readyqueue
					ready(receiverProcess->sender);
					//remove sender from senderqueue
					receiverProcess->sender = NULL;

					//move next sender if exists
					if (receiverProcess->nextSender != NULL) {
						receiverProcess->sender = receiverProcess->nextSender;
						receiverProcess->nextSender =
								receiverProcess->nextSender->nextSender;
					}
					ready(receiverProcess);
					return 0;
				}else{
					temp = temp->nextSender;
				}
			}
			if(temp->pid != *senderPID){ //no sender found in next queue
				kprintf("noSenderFoundInNextQueue ");
				//put receiver on sender's receiver queue
				senderProcess->receiver = receiverProcess;
				//block receiver
				receiverProcess->state = STATE_BLOCKED;
				return -4;
			}



		}else{
			kprintf("noSender ");
			//put receiver on sender's receiver queue
			senderProcess->receiver = receiverProcess;
			//block receiver
			receiverProcess->state = STATE_BLOCKED;
			return -4;
		}
		//check also if sender is blocked/waiting to send, else block self
		//put self on sender's receiverqueue

		//otherwise block self, put sender on receivingqueue


	}

	return 0;
}

