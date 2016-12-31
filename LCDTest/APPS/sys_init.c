#include <stdio.h>

#include "bsp.h"
#include "sys.h"
#include "drv.h"
#include "time1.h"
#include "LPC23xx.h"
//#include "param.h"
//#include "appinfotype.h"
//#include "at24c16.h"

/*等待函数，等待用户输入WAIT_SE时间*/
#define TICKS_PERSEC 	1000

/*初始态用户命令集*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void	tty_setting(void){
	
	vt100Init();
	vt100StrollScreen(10, 25);
	vt100SetCursorPos(6,1);
	vt100SetCursorPos(9,1);
	vt100SetCursorPos(10, 1);
	vt100StrollUp();

}



INT32 	wait_state	(void){
	
	
	printf("OSC FREQ = %d(kHz)\n", CPU_OSC_FREQ);
	printf("HCLK FREQ = %d(kHz)\n", CPU_HCLKFREQ());
	printf("CPU FREQ = %d(kHz)\n", CPU_CCLKFREQ());
	
	return 1;
}


 void	sys_init(void){
	wait_state();
	printf("main routine running.\nCommand:>");
}


