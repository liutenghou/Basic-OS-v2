/* sleep.c : sleep device 
   This file does not need to modified until assignment 2
 */

#include <xeroskernel.h>
#include <xeroslib.h>

unsigned int milliToTicks(unsigned int millisec);

unsigned int sleep(unsigned int millisec){

	int tick = millisec/10 + 1;


	return 0;
}

//notifies sleep device that another time slice has occurred
//wakes up any processes that should be woken up
//places them on ready queue
void tick(void){

}

