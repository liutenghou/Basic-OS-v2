/* create.c : create a process
 */

#include <xeroskernel.h>
#include <xeroslib.h>

pcb     proctab[MAX_PROC];

/* make sure interrupts are armed later on in the kernel development  */
#define STARTING_EFLAGS         0x00003200

// PIDs can't start at 0 nor can they be negative
static int      nextpid = 1;



int create( funcptr fp, size_t stackSize ) {
/***********************************************/

    context_frame       *cf;
    pcb                 *p = NULL;
    int                 i;


    /* PID has wrapped and we can't have -ve numbers 
     * this means that the next PID we handout can't be
     * in use. To find such a number we have to propose a 
     * new PID and then scan to see if it is in the table. If it 
     * is then we have to try again. We know we will succeed 
     * because the process table size is smaller than PID space.
     * However, this code does not do that and just returns an 
     * error.
     */

    //propose net pid
    //scane to see if in table


    if (nextpid < 0) 
      return CREATE_FAILURE;

    // If the stack is too small make it larger
    // could just have it a set size?
    if( stackSize < PROC_STACK ) {
        stackSize = PROC_STACK;
    }

    //find a stopped process
    for( i = 0; i < MAX_PROC; i++ ) {
        if( proctab[i].state == STATE_STOPPED ) {
            p = &proctab[i];
            break;
        }
    }
    
    //    Some stuff to help wih debugging
    //    char buf[100];
    //    sprintf(buf, "Slot %d empty\n", i);
    //    kprintf(buf);
    //    kprintf("Slot %d empty\n", i);
    
    if( !p ) {
        return CREATE_FAILURE;
    }

    //TODO: add context frame for sysstop process below this

    cf = kmalloc( stackSize );
    if( !cf ) {
        return CREATE_FAILURE;
    }

    //TODO: explain
    cf = (context_frame *)((unsigned char *)cf + stackSize - 4);
    cf--;

    memset(cf, 0xB0, sizeof( context_frame ));

    cf->iret_cs = getCS();
    cf->iret_eip = (unsigned int)fp;
    cf->eflags = STARTING_EFLAGS;

    cf->esp = (int)(cf + 1);
    cf->ebp = cf->esp;
    p->esp = (unsigned long*)cf;
    p->state = STATE_READY;
    p->pid = nextpid++;
    p->sending = 0; //1 if sending, 0 if not
    p->receiving = 0; //1 if receiving, 0 if not
    p->msg = 0; //received message from another process
	p->sender = NULL;
	p->nextSender = NULL;
	p->receiver = NULL;
	p->nextReceiver = NULL;

    ready( p );
    return p->pid;
}
