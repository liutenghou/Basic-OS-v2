/* sleep.c : sleep device 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>
#include <xeroslib.h>

/* Your code goes here */

static int global_ticker;

// add process to sleep queue
void sleep(struct pcb* process, unsigned int milliseconds) {
	return;
}

void tick() {
	kprintf("ticker= %d\n", global_ticker);
	global_ticker++;

	
}


