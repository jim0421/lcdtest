/******************************************************************************/
/* BRAKE_MAIN.C: Brake Control                                                */
/******************************************************************************/
/******************************************************************************/

#include <RTL.h>
#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions               */

extern "C"{
#include "bsp.h"
#include "ioport.h"
#include "sys.h"
#include "drv2.h"
#include "time1.h"
//#include "appinfotype.h"

}

//#define 	SIM_DEBUG	1

extern "C"{

extern void sys_init(void);
extern void	ttycmd_init(void);
extern void tty_control(void);

	//void	lcd_test(void);

}



void	GUI_Init(void);
void	GUI_Run(void);
void	EquipmentInfo_Print(void);




/****************/
/* main program */
/****************/
__task void	system_run(void);

int main (void)  {                /* execution starts here*/

	device_initial();

#ifndef SIM_DEBUG
	Lcd_DispOff();
#endif
	

	sys_init();

	os_sys_init (system_run);
	while(1);   

}


/*
 * New main task framework
 */
__task void	tasktty(void){
//Init command stack.
	ttycmd_init();
	while(1){
		tty_control();
   	os_tsk_pass();
	}
}


__task void	taskdisplay(void){
	os_itv_set (10);
   	
  	while (1) {
    	os_itv_wait ();

//			os_dly_wait (10);
		
	}
//	return;
}


__task void	system_run(void){
	OS_TID t_tasktty;                        /*Task ID for tasktty */
	OS_TID t_taskmeasure;                    /* Task ID for taskmeasure */
	OS_TID t_taskdisplay;                    /* Task ID for taskdisplay */


	drv2_init();
	Lcd_DispOn();


	/*Start task. os_tsk_create(task, priority). Bigger the priority high */
//	printf("Strat task tty.\n");
//	printf("Strat task display.\n");
	t_tasktty = os_tsk_create (tasktty, 1);  /* start task tty, lowest priority */
	t_taskdisplay = os_tsk_create (taskdisplay, 8);  /* start task display */
	
	//To ele errors.
	t_tasktty=t_tasktty;
	t_taskmeasure=t_taskmeasure;
	t_taskdisplay=t_taskdisplay;
	
	os_tsk_delete_self();
}

