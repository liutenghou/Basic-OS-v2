/* sleep.c : sleep device 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>
#include <xeroslib.h>

void decrementTimes(pcb *p, unsigned long t);

pcb* sleepers = NULL; //is a delta list

/*

NOTE: this does NOT handle the case where the same process
is added on the queue multiple times.
TODO: return error if process already on sleepqueue

 */
unsigned int sleep(unsigned long millisec){

	//TODO: handle weird input (neg num, out of bounds)
	pcb *currentProcess = getCurrentProcess();
	//kprintf("\nmillisec:%d", millisec);
	pcb *temp = sleepers;
	pcb *tempPrev = sleepers;
	//unsigned long ticks = millisec/10 + (millisec%10)?(1):(0);
	unsigned long ticks = millisec;
	unsigned long tick = millisec;
	kprintf("\nticks:%d\n", millisec);
	long diffTicks = ticks;
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
		tempPrev = temp;
		while(temp!=NULL){

			prevTicks = diffTicks;
			diffTicks = (diffTicks - temp->sleeptime);
			//kprintf("-NXT-DT:%d.PT:%d.", prevTicks, diffTicks);
			if(diffTicks <= 0){
				diffTicks = prevTicks;
				//kprintf("diffTicks:%d.tempPrevTicks:%d ", diffTicks, tempPrev->sleeptime);
				break;
			}
			tempPrev = temp;
			temp = temp->nextSleeper;
		}
		//kprintf("\ntempPrevST:%d.CP:%d.dt:%d\n", tempPrev->sleeptime, currentProcess->pid, diffTicks);

		if(temp == sleepers){ //add to beginning
			sleepers = currentProcess;
			sleepers->nextSleeper = temp;
			sleepers->sleeptime = diffTicks;
			sleepers->state = STATE_STOPPED;
			decrementTimes(sleepers->nextSleeper, diffTicks);
			kprintf("+B+");
			printSleepQueue();
		}else if(temp == NULL){ //add to end
			//kprintf("tempPrevVal:%d.",tempPrev->sleeptime);

			tempPrev->nextSleeper = currentProcess;
			tempPrev->nextSleeper->sleeptime = diffTicks;
			tempPrev->nextSleeper->nextSleeper = NULL;
			tempPrev->nextSleeper->state = STATE_STOPPED;
			//kprintf("tempNextVal:%d.",tempPrev->nextSleeper->sleeptime);
			kprintf("+E+");
			printSleepQueue();
		}else if(temp != NULL){ //add to middle
			//kprintf("NEXTSLEEPER:%d ", tempPrev->nextSleeper->sleeptime);
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

//decrement all sleeptimes from p in sleep queue by input amount
//NOTE: if value in middle of list becomes negative, set to 0
//too much work to reshuffle.
void decrementTimes(pcb *p, unsigned long t){
	pcb *temp = p;
	while(temp != NULL){
		temp->sleeptime = temp->sleeptime-t;
		if(temp->sleeptime < 0){
			temp->sleeptime = 0;
			//kprintf("NEGATIVE IN decrementTime");
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

