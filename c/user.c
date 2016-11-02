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
        kprintf( "Produce %d \n", i );
        syssend(1,101);
//        sysputs("(testingsysputs)");
        sysyield();
    }
    //syssend(2, 55);
    sysstop();
}

 void consumer( void ) {
/****************************/

    int         i;

    for( i = 0; i < 5; i++ ) {
    	//sysputs("consume\n");
        kprintf( " Consume %d \n", i );
        sysyield();
    }

    sysstop();
}

 void     root( void ) {
/****************************/
   int proc_pid, con_pid, root_pid;
   root_pid = sysgetpid();
   
   sysyield();
   sysyield();
   proc_pid = syscreate( &producer, 4096 );
   con_pid =  syscreate( &consumer, 4096 );
   kprintf("Root pid = %d Prod pid = %d Con pid:%d\n", root_pid, proc_pid, con_pid);
   for( ;; ) {
     sysyield();
   }
 }
