/* sleep.c : sleep device 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>
#include <xeroslib.h>

void decrementTimes(pcb *p, unsigned long t);

pcb* sleepers = NULL; //is a delta list

unsigned int sleep(unsigned int millisec){

	pcb *currentProcess = getCurrentProcess();
	pcb *temp = sleepers;
	pcb *tempPrev = sleepers;
	//TODO: uncomment this, after testing
	//unsigned long ticks = millisec/10 + 1;
	unsigned long ticks = millisec;
	unsigned long diffTicks = ticks;
	unsigned long prevTicks = ticks;

	//place current process in list of sleeping processes
	if(sleepers == NULL){ //initial
		sleepers = currentProcess;
		sleepers->state = STATE_STOPPED;
		sleepers->nextSleeper = NULL;
		sleepers->sleeptime = ticks;
		kprintf("+0+");
		printSleepQueue();
	}else{
		//find the sleeper the process is suppose to go after or before
		while(temp!=NULL){
			prevTicks = diffTicks;
			diffTicks = (diffTicks - temp->sleeptime);
			kprintf("-NXT-");
			if(diffTicks <= 0){
				diffTicks = prevTicks;
				break;
			}
			tempPrev = temp;
			temp = temp->nextSleeper;
		}


		if(temp == sleepers){ //goes before everything

			sleepers = currentProcess;
			sleepers->nextSleeper = temp;
			sleepers->sleeptime = diffTicks;
			sleepers->state = STATE_STOPPED;
			decrementTimes(sleepers->nextSleeper, diffTicks);
			kprintf("+B+");
			printSleepQueue();
		}else if(temp == NULL){ //add to end
			tempPrev->nextSleeper = currentProcess;
			currentProcess->sleeptime = diffTicks;
			currentProcess->nextSleeper = NULL;
			currentProcess->state = STATE_STOPPED;
			kprintf("+E+");
			printSleepQueue();
		}else if(temp != NULL){ //add to middle
			currentProcess->nextSleeper = tempPrev->nextSleeper;
			tempPrev->nextSleeper = currentProcess;
			currentProcess->sleeptime = diffTicks;
			currentProcess->state = STATE_STOPPED;
			decrementTimes(currentProcess->nextSleeper, diffTicks);
			kprintf("+M+");
			printSleepQueue();
		}else{
			kprintf("SLEEP ERROR\n");
		}
	}

	return 0;
}

//decrement all sleeptimes in p sleep queue by input amount
void decrementTimes(pcb *p, unsigned long t){
	pcb *temp = p;
	while(temp != NULL){
		temp->sleeptime = temp->sleeptime-t;
		if(temp->sleeptime < 0){
			kprintf("\nERROR IN decrementTime!\n");
		}
		temp = temp->nextSleeper;
	}
}
/*
every tick decrements head of delta list
notifies sleep device that another time slice has occurred
wakes up any processes that should be woken up
places them on ready queue
update internal counters
*/
void tick(void){
	//kprintf(".");
}

//testing code, prints all processes on sleepqueue
void printSleepQueue(void){
	pcb *temp = sleepers;
	while(temp != NULL){
		kprintf("<S:%d>",temp->sleeptime);
		temp=temp->nextSleeper;
	}
}

