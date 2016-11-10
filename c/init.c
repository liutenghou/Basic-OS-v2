/* initialize.c - initproc */

#include <i386.h>
#include <xeroskernel.h>
#include <xeroslib.h>

extern int entry(void); /* start of kernel image, use &start    */
extern int end(void); /* end of kernel image, use &end        */
extern long freemem; /* start of free memory (set in i386.c) */
extern char *maxaddr; /* max memory address (set in i386.c)	*/

/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED.  The     ***/
/***   interrupt table has been initialized with a default handler    ***/
/***								      ***/
/***								      ***/
/************************************************************************/

/*------------------------------------------------------------------------
 *  The init process, this is where it all begins...
 *------------------------------------------------------------------------
 */

int idlePID;
int rootPID;
void initproc(void) /* The beginning */
{
	kprintf("\n\nCPSC 415, 2016W1 \n32 Bit Xeros 0.01 \nLocated at: %x to %x\n",
			&entry, &end);

	/* Your code goes here */

	kprintf("Max addr is %d %x\n", maxaddr, maxaddr);

	kmeminit();
	kprintf("memory inited\n");

	dispatchinit();
	kprintf("dispatcher inited\n");
	//TODO: continue here, sends junk to dispatcher

	contextinit();
	initPIT(100);
	kprintf("context inited\n");

	rootPID = create(root, PROC_STACK);
	idlePID = create(idleproc, PROC_STACK);
	kprintf("create inited\n");

	dispatch();

	kprintf("Returned to init, you should never get here!\n");

	/* This code should never be reached after you are done */
	for (;;)
		; /* loop forever */

}

//idle process to accept interrupts when nothing else is running
void idleproc(void) {
	int i;
	for(i=1; i;){
		//kprintf("idle.");
		sysyield();
	}
	sysstop();
}

int getIdlePID(void){
	return idlePID;
}

