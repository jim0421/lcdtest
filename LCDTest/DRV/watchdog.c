#include <LPC23xx.H>              /* LPC21xx definitions                      */
#include "bsp.h"
#include "sys.h"




void init_watchdog(void){

#if WATCHDOGEN
//	WDTC = 3125000; 	//看门狗计数，PCLK=50MHz/4, Watchdogclk = PCLK/4=50MHz/16=3.125Mhz.	 WATCHDOG=1s.
//	WDMOD = 0x3;	//设置看门狗模式
#endif
}


void feed_dog(void){
#if WATCHDOGEN
//	WDFEED = 0xAA;
//	WDFEED = 0x55;
#endif
}



