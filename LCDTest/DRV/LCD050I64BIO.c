#include <lpc23xx.h>

#include "bsp.h"
#include <stdio.h>
#include "lcd050i64bio.h"
#include "ioport.h"
#include "sys.h"


void	lcd050i64bio_init		(void){
  EMC_STA_CFG2 = 0x1<<7;
   EMC_STA_WAITWEN2 = 0xff;
   EMC_STA_WAITOEN2 = 0x2f;
   EMC_STA_WAITRD2 = 0x1f;
   EMC_STA_WAITPAGE2 = 0x0;
   EMC_STA_WAITWR2 = 0xff;
   EMC_STA_WAITTURN2 = 0x0;
   EMC_STA_EXT_WAIT = 0;
   PINSEL4 |= 0x1<<28;
}

UINT8	lcdbusystate(void){
	
	return lcdbusyreg & 0x1;

}

void	set_xy_add(UINT16 x, UINT16 y){
	lcd_addr(LCDREG_X);
	lcd_data(x>>8);
	lcd_data(x&0xff);

	lcd_addr(LCDREG_Y);
	lcd_data(y>>8);
	lcd_data(y&0xff);
}

void	set_reg1(UINT8 regaddr, UINT16 dat){
	lcd_addr(regaddr);
	lcd_data(dat&0xff);
}

void	set_reg2(UINT8 regaddr, UINT16 dat){

	lcd_addr(regaddr);
	lcd_data(dat>>8);
	lcd_data(dat&0xff);

}

void	set_sys_cmd(UINT16 cmd){

	lcd_addr(LCDREG_SCMD);
	lcd_data(cmd>>8);
	lcd_data(cmd&0xff) ;
}



int		lcd050i64bio_read		(int addr, int* data){
	
	return DEVICE_OK;
}


int		lcd050i64bio_write	(int addr, int data){

	return DEVICE_OK;
}


int		lcd050i64bio_ioctl	(int io_cmd, 	void* parameter){

	switch(io_cmd){
	 	case LCD_RST:
			device_write(ID_IOPORT, PORT_LCDRST, 1);
			device_write(ID_IOPORT, PORT_LCDRST, 0);
			delay(100);
			device_write(ID_IOPORT, PORT_LCDRST, 1);
	
			break;
	}
	return DEVICE_OK;
}





/***********************************************************************************************/
/*int		lcd050i64bio_test(){

	UINT8	lcd_status;

	device_ioctl(ID_LCD, LCD_RST, NULL);
	device_ioctl(ID_LCD, LCD_LGTON, NULL);

	delay(20);
	lcd_status = device_read(ID_LCD, 1, NULL);
	printf("Lcd satus = %x \n", lcd_status);

	


	return 1;
}
*/
