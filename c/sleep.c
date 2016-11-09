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
	unsigned long ticks = millisec/10 + 1;
	unsigned long diffTicks = ticks;
	unsigned long prevTicks = ticks;
	currentProcess->sleeptime = ticks;

	//place current process in list of sleeping processes, delta list
	if(temp == NULL){
		sleepers = currentProcess; //easy, done
		sleepers->state = STATE_STOPPED;
		sleepers->nextSleeper = NULL;
		sleepers->sleeptime = ticks;
		kprintf("+AS");
	}else{
		//find the sleeper the process is suppose to go after
		while(temp->nextSleeper!=NULL){
			prevTicks = diffTicks;
			diffTicks = (diffTicks - temp->sleeptime);
			if(diffTicks <= 0){
				diffTicks = prevTicks;
				break;
			}
			temp = temp->nextSleeper;
		}

		if(temp->nextSleeper == NULL){ //just add to end
			temp->nextSleeper = currentProcess;
			currentProcess->sleeptime = diffTicks;
			currentProcess->nextSleeper = NULL;
			currentProcess->state = STATE_STOPPED;
			kprintf("+ZS");
		}else{
			currentProcess->nextSleeper = temp->nextSleeper;
			temp->nextSleeper = currentProcess;
			currentProcess->sleeptime = diffTicks;
			currentProcess->state = STATE_STOPPED;
			decrementTimes(currentProcess->nextSleeper, diffTicks);
			kprintf("+MS");
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
			kprintf("ERROR IN decrementTime!\n");
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
		kprintf("S:temp->pid.");
	}
}

