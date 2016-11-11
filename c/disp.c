/* disp.c : dispatcher
 */

#include <xeroskernel.h>
#include <xeroslib.h>
#include <stdarg.h>

static pcb *head = NULL;
static pcb *tail = NULL;
// next process in the queue
pcb *p;

void dispatch(void) {
	/********************************/

	int r;
	funcptr fp;
	// size of the process
	int stack;
	// variable length list of arguments
	va_list ap;
	//char *s;
	int toPID;
	unsigned long msg;
	
	int *senderPID;
	unsigned long *msgP;
	
	unsigned int tosleeptime;

	for (p = next(); p;) {
		//      kprintf("Process %x selected stck %x\n", p, p->esp);
		if(p == NULL){
//			kprintf("pNULL");
			p = getIdleProcPCB();
		}else{
//			kprintf("pOK:%d.", p->pid);
		}
//		if(head == NULL){
//			kprintf("headNULL");
//		}else{
//			kprintf("headOK:%d,", head->pid);
//		}

		r = contextswitch(p);
		switch (r) {
		case (SYS_CREATE):
			ap = (va_list) p->args;
			fp = (funcptr)(va_arg( ap, int ) );
			stack = va_arg( ap, int );
			p->ret = create(fp, stack);
			break;
		case (SYS_YIELD):
			ready(p);
			p->ret = 0; //arbitrary value
			p = next();

			if(p->sending  || p->receiving){
				ready(p);
				p = next();
			}

			break;
		case (SYS_STOP):
			p->state = STATE_STOPPED;
			p = next();
			break;
			//cases for A2
		case (SYS_GETPID):
			p->ret = p->pid;
			break;
		case (SYS_PUTS):
			ap = (va_list) p->args;
			kprintf("%s", va_arg(ap, char *));
			break;
		case (SYS_KILL):
			ap = (va_list) p->args;
			toPID = va_arg(ap, int);
			killprocess(toPID);
			p = next();
			break;
		case (SYS_SEND):
			ap = (va_list) p->args;
			//dest_pid, then msg
			toPID = va_arg(ap, int);
			msg = va_arg(ap, unsigned long);
			//kprintf("SYS_SEND, toPID:%d, msg:%d", toPID, msg);

			p->ret = send(toPID, msg);
			if(p->ret < 0)break;

			if (p->state == STATE_BLOCKED) { //message not sent
				kprintf("-message send FAIL-senderonqueue:%d-",
						getProcessFromPID(toPID)->sender->pid);

			} else { //msg sent successfully, place both on ready queue
				kprintf("-message sent OK-");
				kprintf("-message:%d-", getProcessFromPID(toPID)->msg);
				ready(p);
				ready(getProcessFromPID(toPID));
			}
			break;
		case (SYS_RECEIVE):
			ap = (va_list) p->args;
			//int receive(int* srcPID, void* buffer, int len)
			senderPID = va_arg(ap, int*);
			msgP = va_arg(ap, unsigned long*);
			p->ret = receive(senderPID, msgP);
			
			if(p->ret < 0 && p->ret!=-4 ){
				kprintf("error:%d ", p->ret);
				break;
			}else if (p->ret == -4) { //receiver blocked
				kprintf("msg not recvd senderqueue:%d\n",p->nextSender->pid);
				blockedReady(p);
				p=next();
			} else if(p->ret == 0) {
				kprintf("-messeage has been received-\n");
			} else{
				kprintf("error on receive\n");
			}
					
			break;
		case (SYS_TIMER):
			tick();
			ready(p);
			p = next();
			end_of_intr();
			break;
		case(SYS_SLEEP):
			ap = (va_list)p->args;
			tosleeptime = va_arg(ap, unsigned int);
			sleep(tosleeptime);
			p = next(); //need to get the next process, current process stopped
			break;
		default:
			kprintf("Bad Sys request %d, pid = %d\n", r, p->pid);
		}
	}

	kprintf("Out of processes: dying\n");

	for (;;);
}

extern void dispatchinit(void) {
	/********************************/

	//bzero( proctab, sizeof( pcb ) * MAX_PROC );
	memset(proctab, 0, sizeof(pcb) * MAX_PROC);
}

//sets a process p
//puts it in back of readyqueue
extern void ready(pcb *p) {
	/*******************************/
	p->next = NULL;
	p->state = STATE_READY;

	if (tail) {
		tail->next = p; //put at back of ready queue
	} else {
		head = p; //only process, put itself at head
	}
	tail = p;
}

//puts in back of readyqueue without unblocking
extern void blockedReady(pcb *p){
	p->next = NULL;
	if (tail) {
		tail->next = p; //put at back of ready queue
	} else {
		head = p; //only process, put itself at head
	}
	tail = p;

}

//pull first process from readyqueue
//set head as second process on readyqueue
//if no more processes, return idleproc
extern pcb* next(void) {
	/*****************************/
	pcb *p = NULL;
	p = head;
	if (p) {
		head = p->next;
		if (!head) { //if no other processes beside head
			tail = NULL;
		}
	}
	if(p == NULL){
		kprintf("pNULL");
	}
	return (p);
}

void killprocess(int pid) {
	int i;
	for (i = 0; i < MAX_PROC; i++) {
		if (proctab[i].pid == pid) {
			proctab[i].state = STATE_STOPPED;
		}
		break;
	}
}

//TODO: use this for all kernel calls
int getCurrentPID(void) {
	return p->pid;
}

pcb* getCurrentProcess(void){
	return p;
}

pcb* getIdleProcPCB(void){
	return getProcessFromPID(getIdlePID());
}


extern pcb* getProcessFromPID(int pid){
	int i;
	for(i=0; i<MAX_PROC; i++){
		if(proctab[i].pid == pid){
			//kprintf(" TOPIDINgetPCBbyPID:%d ",proctab[i].pid);
			return &proctab[i];
		}
	}
	return NULL;
}

