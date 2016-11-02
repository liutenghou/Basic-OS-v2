/* disp.c : dispatcher
 */

#include <xeroskernel.h>
#include <xeroslib.h>
#include <stdarg.h>

static pcb *head = NULL;
static pcb *tail = NULL;
pcb *p;

void dispatch(void) {
	/********************************/


	int r;
	funcptr fp;
	int stack;
	va_list ap;
	char *s;
	int toPID;
	int msg;

	for (p = next(); p;) {
		//      kprintf("Process %x selected stck %x\n", p, p->esp);

		r = contextswitch(p);
		switch (r) {
		case ( SYS_CREATE):
			ap = (va_list) p->args;
			fp = (funcptr)(va_arg( ap, int ) );
			stack = va_arg( ap, int );
			p->ret = create(fp, stack);
			break;
		case ( SYS_YIELD):
			ready(p);
			p->ret = 0; //arbitrary value
			p = next();
			break;
		case ( SYS_STOP):
			p->state = STATE_STOPPED;
			p = next();
			break;
		//cases for A2
		case(SYS_GETPID):
			p->ret = p->pid; //TODO: why doesn't this work?
			break;
		case(SYS_PUTS):
			ap = (va_list)p->args;
			kprintf("%s", va_arg(ap, char *));
			break;
		case(SYS_KILL):
			ap = (va_list)p->args;
			toPID = va_arg(ap, int);
			killprocess(toPID);
			p = next();
			break;
		case(SYS_SEND):
			ap = (va_list)p->args;
			//dest_pid, then msg
			toPID = va_arg(ap, int);
			msg = va_arg(ap, unsigned long);
			//kprintf("SYS_SEND, toPID:%d, msg:%d", toPID, msg);

			p->ret = send(toPID, msg);
			break;
		default:
			kprintf("Bad Sys request %d, pid = %d\n", r, p->pid);
		}
	}

	kprintf("Out of processes: dying\n");

	for (;;)
		;
}

extern void dispatchinit(void) {
	/********************************/

	//bzero( proctab, sizeof( pcb ) * MAX_PROC );
	memset(proctab, 0, sizeof(pcb) * MAX_PROC);
}

extern void ready(pcb *p) {
	/*******************************/

	p->next = NULL;
	p->state = STATE_READY;

	if (tail) {
		tail->next = p;
	} else {
		head = p; //only process, put itself at head
	}

	tail = p;
}

extern pcb *next(void) {
	/*****************************/

	pcb *p;

	p = head;

	if (p) {
		head = p->next;
		if (!head) {
			tail = NULL;
		}
	}

	return (p);
}

void killprocess(int pid){
	int i;
	for(i=0; i<MAX_PROC; i++){
		if(proctab[i].pid == pid){
			proctab[i].state = STATE_STOPPED;
		}
		break;
	}
}

int getCurrentPID(){
	return p->pid;
}
