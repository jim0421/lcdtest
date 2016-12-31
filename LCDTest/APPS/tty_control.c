//#include	"pwmeter.h"


#include 	<math.h>


#include <stdio.h>

#include "bsp.h"
#include "sys.h"
//#include "ad7888.h"
#include "time1.h"
#include "ioport.h"
//#include "lcd240128.h"
#include "drv2.h"
#include "drv.h"
#include "rtl.h"

void 	bustest(INT16 pos);
void 	bustestr(INT16 pos);

//void	ADRslt_ConDisp(void);

void	lcd_portr(void){
	printf("R=%x\n",device_read(ID_LCD, 1, NULL));	
}

void 	LcdTest(void);
void	cmd_LcdTest(void){
	U32 ticks;
	ticks = os_time_get ();
	printf("Start tick=%d\n",ticks);
	LcdTest();
	ticks = os_time_get ();
	printf("End tick=%d\n",ticks);
}

void	lcd_rst(void);

void	draw_line(void){
	int x;
	for(x=0; x<300; x++){
		
		putpixel(x, 50,LCD_RED );
	}
	
}

static const usrcmd	usercommand[] = {
	{"H",				&help,						"Help message."},
	{"LP",			&lcd_portr,				"lcd port."},
	{"LCDRST",	&lcd_rst,					"Reset lcd."},	 
	{"LCDTEST",	&cmd_LcdTest,			"Lcd Test."},	 
	{"DLINE",		&draw_line,				"Draw line."},


};



void	ttycmd_init(void){
	 set_usrcmd(usercommand, sizeof(usercommand)/sizeof(usrcmd));
}

INT32 	command(void);

void	tty_control(void){
   
	command();

}


