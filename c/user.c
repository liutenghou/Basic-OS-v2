/* user.c : User processes
 */

#include <xeroskernel.h>

/* Your code goes here */
 void producer( void ) {
/****************************/

    int         i;

    for( i = 0; i < 5; i++ ) {
    	//TODO: why doesn't sysputs repeat 5 times?
    	//sysputs("produce?");
        kprintf( "Produce %d . ", i );
        sysyield();
    }

    sysstop();
}

 void consumer( void ) {
/****************************/

    int         i;

    for( i = 0; i < 5; i++ ) {
    	//sysputs("consume\n");
        kprintf( "Consume %d \n", i );
        sysyield();
    }

    sysstop();
}

 void     root( void ) {
/****************************/
   int proc_pid, con_pid;
//   int gotPID = sysgetpid();
//   kprintf("Proc pid = %d Con pid = %d\n gotpid:%d\n", proc_pid, con_pid, gotPID);
//
//   kprintf("Root has been called\n");
   
   sysyield();
   sysyield();
   proc_pid = syscreate( &producer, 4096 );
   con_pid =  syscreate( &consumer, 4096 );

   for( ;; ) {
     sysyield();
   }
 }
