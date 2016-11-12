/* msg.c : messaging system 
 This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>

//TODO: change msg to be buffer in receiver

/* kernel side: sends a message from process to another process
 * arguments: destPid: destination process pid, msg: an int to send
 * return: 0 on sucess, negative number on error
 *
 */
int send(int destPID, unsigned long msg) {
	//kprintf("|SENDer:%d,destpid:%d|", sysgetpid(), destPID);


	int senderPID = sysgetpid();
	pcb *senderP = getProcessFromPID(senderPID);	//sender


	if(destPID <= 0){
		kprintf("invalid pid\n");
	}
	if (destPID == senderPID) {
		kprintf("send: dest same as self\n");
		return -2;
	}

	pcb *destP = getProcessFromPID(destPID);		//receiver
	//dest process not found
	if (destP == NULL) {
		kprintf("send: dest not exist\n");
		return -1;
	}
	destP->msg = msg; //store message in own pcb

//	if (destPID->state == STATE_STOPPED) {
//		kprintf("send: dest cannot receive\n");
//		return -1;
//	}
	//kprintf(" sysgetpid():%d ", sysgetpid());
	//destination is self

	//if receiver is not blocked on matched receive, block send until matched receive made
	if (destP->state != STATE_BLOCKED) {
//		kprintf("<S1>");
		if (destP->sender != NULL) { //check next
//			kprintf("<S2>");
			pcb *temp = destP->nextSender;
			while (temp->nextSender != NULL) {
				temp = temp->nextSender;
			}
			temp = senderP;
		} else { //only sender
//			kprintf("<S3>");
			//kprintf("destpid:%d, senderpid:%d ", destP->pid, senderP->pid);//add senderP to receiver senderqueue
			destP->msg = msg;
			destP->sender = senderP;
		}
		//block sender
		senderP->state = STATE_BLOCKED;
		//if receiver is blocked and therefore ready to receive message,
		//then message copied to receive buffer, place both processes both on ready queue
	} else {
//		kprintf("<S4>");
		//send message
		destP->msg = msg;
		*(destP->msg2) = msg;
		destP->sender = senderP;

		//ready(destP);
		//ready(senderP);
	}
	return 0;
}

//process receive message
//arguments: sender pid pointer, message pointer in user space
//returns -4 if receiver blocked, waiting on sender
int receive(int *senderPID, unsigned long *msg) {

	//kprintf("|RECVsenderPID:%d, msg:%d |", *senderPID, *msg);
	pcb *receiverProcess = getCurrentProcess();

	// receiving process cannot receive from itself
	if (receiverProcess->pid == *senderPID) {
		//kprintf("error receiver():cannot receive from self\n");
		return -2;
	}
	//check if sender process exists
	pcb *senderProcess;
	if(*senderPID != 0){
		senderProcess = getProcessFromPID(*senderPID);
		if (senderProcess == NULL) {
			//kprintf("error receiver(): no process found for pid\n");
			return -1;
		}
	}

	//other errors return -3
	if(*senderPID<0){
		//kprintf("error receiver()\n");
		return -3;
	}
	receiverProcess->msg2 = msg;
	//check senderqueue, if nothing on it, block self

	//receive from any,
	if (*senderPID == 0) {
		//kprintf("<R1>");
		//check sender queue, if nothing on it block
		if (receiverProcess->sender == NULL) {
			//kprintf("<R2>");
			receiverProcess->state = STATE_BLOCKED;
			return -4;
			//do receive again
		//NOTE: if on sending queue, sender must be blocked
		//sending queue not empty, transfer message immediately
		} else {
			//kprintf("<R3>");
			//receive message
			*msg = receiverProcess->msg;
			*senderPID = receiverProcess->sender->pid;
			//sending process has to be blocked, unblock it, back on readyqueue
			if(receiverProcess->sender->pid != 1){
				//kprintf("<R4>");
				ready(receiverProcess->sender);
			}
			//remove sender from senderqueue
			receiverProcess->sender = NULL;

			//move next sender if exists
			if (receiverProcess->nextSender != NULL) {
				//kprintf("<R5>");
				receiverProcess->sender = receiverProcess->nextSender;
				receiverProcess->nextSender =
						receiverProcess->nextSender->nextSender;
			}
			ready(receiverProcess);
		}

	}else{
		//kprintf("<R6>");
		//check sender, see if on senderqueue, if so receive it
		if(receiverProcess->sender->pid == *senderPID){
			//kprintf("<R7>");
			*msg = receiverProcess->msg;
			*senderPID = receiverProcess->sender->pid;
			//sending process was be blocked, unblock it, back on readyqueue
			//if root pid, don't do this
			if(receiverProcess->sender->pid != 1){
				//kprintf("<R8>");
				ready(receiverProcess->sender);
			}

			//remove sender from senderqueue
			receiverProcess->sender = NULL;
			//move next sender if exists
			if (receiverProcess->nextSender != NULL) {
				receiverProcess->sender = receiverProcess->nextSender;
				receiverProcess->nextSender =
						receiverProcess->nextSender->nextSender;
			}

		}else if(receiverProcess->nextSender != NULL){ //check rest of queue
			//kprintf("<R9>");
			//kprintf("hasNextSender ");
			pcb *temp = receiverProcess->nextSender;
			while(temp != NULL){
				if(temp->pid == *senderPID){
					//kprintf("<R12>");
					*msg = receiverProcess->msg;
					*senderPID = receiverProcess->sender->pid;
					//sending process has to be blocked, unblock it, back on readyqueue
					if(receiverProcess->sender->pid != 1){
						ready(receiverProcess->sender);
					}
					//remove sender from senderqueue
					receiverProcess->sender = NULL;

					//move next sender if exists
					if (receiverProcess->nextSender != NULL) {
						receiverProcess->sender = receiverProcess->nextSender;
						receiverProcess->nextSender =
								receiverProcess->nextSender->nextSender;
					}

					return 0;
				}else{
					temp = temp->nextSender;
				}
			}
			if(temp->pid != *senderPID){ //no sender found in next queue
				//kprintf("<R10>");
				//kprintf("noSenderFoundInNextQueue ");
				//put receiver on sender's receiver queue
				senderProcess->receiver = receiverProcess;
				//block receiver
				receiverProcess->state = STATE_BLOCKED;
				return -4;
			}
		}else{
			//kprintf("<R11>");
			//put receiver on sender's receiver queue
			senderProcess->msg = *msg;
			*(senderProcess->msg2) = *msg;
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

