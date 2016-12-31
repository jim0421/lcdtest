
#include <rtl.h>
#include "bsp.h"
#include <stdarg.h>
#include <stdio.h>
#include "sys.h"


void sleep(INT16 duration){
 	os_dly_wait(duration);
}


void	viewdat(UINT8 *dat ,INT16 size){
	INT16 i,j;

	INT8	asciidat[17];
	asciidat[16] = 0;
	DebugPrint (0,"Size of dat is %d bytes.\n", size);
	if(size <= 0) return;

	for(i=0; i<size; i++){
		if(i%16==0)
			DebugPrint (0,"[%4x] ", i);
		DebugPrint (0,"%2X ", dat[i]);
		if(dat[i]>0x10 && dat[i]<0x80)
			asciidat[i%16] = dat[i];
		else
			asciidat[i%16] = '.';
		if((i+1)%16== 7){
			DebugPrint (0,"- ");
		}
		if((i+1)%16==0) {
			DebugPrint (0,"   %s\n", asciidat);
			for(j=0; j<16; j++)
				asciidat[j]=0;
		}
	}

	if((i)%16 == 0) return;
	asciidat[i%16] = 0;
	while(1){
		if((i++)%16 == 0){
			DebugPrint (0,"   %s\n", asciidat);
			break;
		}
		DebugPrint (0,"   ");

	}

}



void	rs232send(UINT8 *pdat, INT16 size){
	
	INT16	i, cnt=0;

	for(i=0; i<size; i++){
		while (device_write(ID_UART1, 0, pdat[i]) != 0) {
			DebugPrint (DB_LVL,"s(%d)", cnt++);
			
			sleep(10);
		}
  	}
	return;
}

void	rs232init(void){
	UINT8 inidat[] = {0xAA, 0xE0, 0x55, 0xAA, 0x5A, 0xA5, 
			0x03, 0x06, 0x00,
			0xCC, 0x33, 0xC3, 0x3C};

	UINT32	baud = 19200;
	
	rs232send(inidat, sizeof(inidat));
	sleep(20);		 
	//Switch baudrate
	device_ioctl(ID_UART1,BAUDSET, &baud);
	sleep(1);	
	return;
}

UINT32	get_sysclk(void){
	UINT32 tick;
	sys_call(SYS_TICK, &tick);
	return tick;

}



