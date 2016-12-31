#include <LPC23xx.h>
#include <stdio.h>
#include "bsp.h"
#include "sys.h"

#include "irq.h"

static UINT32	system_tick = 0;
/* Timer2 Interrupt function.*/
//Timer2 的定时是1ms。

void 	Timer2_Interrupt (void) __irq{

	system_tick	++;

	T2IR = 1;                           /* Clear interrupt flag  */
  	VICVectAddr = 0;                    /* Acknowledge Interrupt */
}



//系统调用入口
UINT32	syscall_systick (void* parameter){
	*((UINT32*)parameter) = system_tick;
	return (UINT32)system_tick;
}


/*
Timer intital function
*/
void 		Timer2_Initial(void)
{
	
	/*initialize the Timer 0*/
	PCONP |= (1<<22);						   	 /*Turn on the power of Timer2.*/
	T2MR0 =  CPU_PCLKFREQ(PCLK_TIMER2);		      /* ms    */
 	T2MCR = 3;                                   /* Interrupt and Reset on MR0  */
	T2TCR = 1;                                   /* Timer2 Enable               */

	
	install_irq( TIMER2_INT, (void *)Timer2_Interrupt, 1 ); 

}


void sys_delay(INT32 ticks){
	UINT32 old_ticks, cur_ticks, delay_ticks;
	sys_call(SYS_TICK, &old_ticks);
	
	while(1){
		sys_call(SYS_TICK, &cur_ticks);
		cur_ticks = system_tick;
	
		delay_ticks = cur_ticks - old_ticks;
		
		if(delay_ticks>=ticks)
			break;
	}
}

void delay	(UINT32 ticks){
	
	UINT32 cur_ticks,pre_ticks; 
	
	sys_call(SYS_TICK, &pre_ticks);

	sys_call(SYS_TICK, &cur_ticks);
	while(ticks > (cur_ticks - pre_ticks)){
		sys_call(SYS_TICK, &cur_ticks);
	}
}




void Timer2_init			(void)
{
	Timer2_Initial();
}
int	Timer2_read			(int addr, int* data){
	return DEVICE_CMDNOEXIST;
}
int	Timer2_write		(int addr, int data){
	return DEVICE_CMDNOEXIST;
}
int Timer2_ioctl		(int io_cmd, 	void* parameter){
	return DEVICE_CMDNOEXIST;
}

/////////////////////////////////////////////////////////////////////////////////
//ticks 调用例程


typedef struct tick_call_tag{
	INT8 valid;
	INT32 tick_interval;
	INT32 tick_remain;
	void (*tick_routine)(void);
} tick_call;

static tick_call tick_call_buff[16]={
	{FALSE,0,0,NULL},/*0*/
	{FALSE,0,0,NULL},/*1*/
	{FALSE,0,0,NULL},/*2*/
	{FALSE,0,0,NULL},/*3*/
	{FALSE,0,0,NULL},/*4*/
	{FALSE,0,0,NULL},/*5*/
	{FALSE,0,0,NULL},/*6*/
	{FALSE,0,0,NULL},/*7*/
	{FALSE,0,0,NULL},/*8*/
	{FALSE,0,0,NULL},/*9*/
	{FALSE,0,0,NULL},/*10*/
	{FALSE,0,0,NULL},/*11*/
	{FALSE,0,0,NULL},/*12*/
	{FALSE,0,0,NULL},/*13*/
	{FALSE,0,0,NULL},/*14*/
	{FALSE,0,0,NULL}/*15*/
};
	
void sys_tick_call(void){
	int i;
	static UINT32 old_ticks;
	UINT32 cur_ticks, delay_ticks; 
	
	tick_call *tick_call_ptr;
	
	sys_call(SYS_TICK, &cur_ticks);
	
	delay_ticks = cur_ticks - old_ticks;
	old_ticks = cur_ticks;
	for(i=0; i<16; i++){
		tick_call_ptr = &(tick_call_buff[i]);
		if(tick_call_ptr->valid==TRUE){					//判断该位置的数据是否有效
			tick_call_ptr->tick_remain -= delay_ticks; 	//得到剩余的Tick数
			if(tick_call_ptr->tick_remain <=0){			//Tick数达到要求
				(*tick_call_ptr->tick_routine)();		//调用函数
				tick_call_ptr->tick_remain += tick_call_ptr->tick_interval;	//复位剩余Tick数
				if(tick_call_ptr->tick_remain<0) tick_call_ptr->tick_remain = tick_call_ptr->tick_interval;	//防止剩余Tick数为负数
			}
		}
			
	}

}


//增加Ticks调用例程
INT32 add_tickroutine(INT32 tick_interval, void(*routine)(void)){
	int i;
	
	for(i=0; i<16; i++){
		if(tick_call_buff[i].valid == FALSE){
			tick_call_buff[i].valid = TRUE;
			tick_call_buff[i].tick_interval = tick_interval;
			tick_call_buff[i].tick_remain = tick_interval;
			tick_call_buff[i].tick_routine = routine;
			return i;
		}
		
	}
	return -1;		

}


