#include <LPC23xx.h>
#include <stdio.h>
#include "bsp.h"
#include "irq.h"

#include "time1.h"
#include "drv.h"

//Disable eia485 tx
//void	delay_disabletx(void);

//void 	KeyDetect(void);

void 	Timer1_Interrupt (void) __irq {


	T1IR = 1;                           /* Clear interrupt flag  */

//	device_ioctl(ID_AD7888, START_S, NULL);
//	KeyDetect();
//	delay_disabletx();
  	VICVectAddr = 0;                    /* Acknowledge Interrupt */
}






/*
Timer intital function
*/
void 		Timer1_Initial(void)
{
	
	/*initialize the Timer 0*/
	T1MR0 =  CPU_PCLKFREQ(PCLK_TIMER2);				//ms
 	T1MCR = 3;                                   /* Interrupt and Reset on MR0  */
	T1TCR = 1;                                   /* Timer0 Enable               */
	install_irq( TIMER1_INT, (void *)Timer1_Interrupt, 6 ); 
	disable_irq(TIMER1_INT);
}



void timer1_init		(void)
{
	Timer1_Initial();
}
int	timer1_read		(int addr, int* data){
	return DEVICE_CMDNOEXIST;
}
int	timer1_write	(int addr, int data){
	return DEVICE_CMDNOEXIST;
}
int 	timer1_ioctl		(int io_cmd,	void* parameter){
	if(io_cmd == EN_ADSAMPLING)	  {
	//	T1TCR = 0x1;
		enable_irq(TIMER1_INT);
		return DEVICE_OK;
	}
	else if(io_cmd == DIS_ADSAMPLING){
	//	T1TCR = 0x0;
		disable_irq(TIMER1_INT);
		return DEVICE_OK;
	}
	else return	DEVICE_CMDNOEXIST;
}


