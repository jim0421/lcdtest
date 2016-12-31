#include "bsp.h"

#include "sys.h"

#include <stdio.h>
//各处定义的系统调用入口

int		syscall_systick (void* parameter);		/*4*/


static sys_call_routine sys_call_buffer[16]={
	NULL,
	NULL,
	NULL,
	NULL,
	syscall_systick,
	NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,NULL,
	NULL,NULL,NULL};

static void *sys_call_p = NULL;

INT32 __swi(8) sys_call_swi (int command);
INT32 __SWI_8               (int command) {
	
	if (sys_call_buffer[command] != NULL)
		return (*(sys_call_buffer[command]))(sys_call_p);
	else	
		return -1; 
	
}


INT32 sys_call(int command, void *p)
{
	INT32 ret;
	command = command & 0xf;
	sys_call_p = p;
	
	//ret = sys_call_critical(command);	
	ret = sys_call_swi(command);	
	return ret;
	
}

void bsp_delayms(INT32 delay){
	while(delay!=0){
		sys_delay(1000);
		delay --;
	}
}


//INT32 add_syscall(sys_call_routine sys_routine){
//	for(int i=0 ; i<16; i++) {
//		if(sys_call_buffer[i] == NULL){
//			sys_call_buffer[i] = sys_routine;
//			return i;
//		}
//	}
//	
//	return -1;	//没有剩余的位置。
//
//}


