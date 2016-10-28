/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>


int syscall( int req, ... ) {
/**********************************/

    va_list     ap;
    int         rc;

    va_start( ap, req );

    __asm __volatile( " \
        movl %1, %%eax \n\
        movl %2, %%edx \n\
        int  %3 \n\
        movl %%eax, %0 \n\
        "
        : "=g" (rc)
        : "g" (req), "g" (ap), "i" (KERNEL_INT)
        : "%eax" 
    );
 
    va_end( ap );
    //kprintf("req: %d. rc: %d ", req, rc);
    return( rc );
}

int syscreate( funcptr fp, size_t stack ) {
/*********************************************/

    return( syscall( SYS_CREATE, fp, stack ) );
}

void sysyield( void ) {
/***************************/
  syscall( SYS_YIELD );
}

 void sysstop( void ) {
/**************************/

   syscall( SYS_STOP );
}

//A2 additions

//gets PID of the current process
extern int sysgetpid(void){
	return syscall(SYS_GETPID);
}

/*synchronizes output to screen between processes
input: null terminated string, to be displayed by kernel
kernel uses kprintf to display string
*/
extern void sysputs(char *str){
	syscall(SYS_PUTS, str);
}

//takes PID as int, kills process
extern int syskill(int pid){
	return syscall(SYS_KILL, pid);
}

//3.3 IPC system calls

/*
 * Sends a message to another process, syssend is blocked until msg is received
 * parameters: destination pid, msg to send
 * returns: 0 on succcess, -1 if dest is dead, -2 if send to self, -3 if other err
 *
 */
extern int syssend(int dest_pid, unsigned long msg){
	//dest is dead, not found
	if(getPCBbyPID(dest_pid) == NULL){
		kprintf("syssend: dest not exist\n");
		return -1;
	}
	//destination is self
	if(dest_pid == sysgetpid()){
		kprintf("syssend: dest same as self\n");
		return -2;
	}
	//other error?


	return syscall(SYS_SEND, dest_pid, msg);
}
extern int sysrecv(unsigned int *from_pid, unsigned long *msg){
	return -1;
}

extern pcb* getPCBbyPID(int pid){
	pcb *p;
	int i;
	for(i=0; i<MAX_PROC; i++){
		if(proctab[i].pid == pid){
			return &proctab[i];
		}
	}
	return NULL;
}
