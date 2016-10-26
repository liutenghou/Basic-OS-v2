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
	return syscall(SYS_KILL);
}

//3.3 IPC system calls

extern int syssend(int dest_pid, unsigned long num){
	return -1;
}
extern int sysrecv(unsigned int *from_pid, unsigned long *num){
	return -1;
}
