#ifndef _DRV2_H_
#define _DRV2_H_


#define 	LCD_BLACK	0x00
#define		LCD_RED		0xe0
#define		LCD_GREEN	0x1c
#define		LCD_BLUE	0x03
#define		LCD_WHITE	0xff


#include "..\DRV2\LCD050I64.h"

#include "DRV2\lcd_disp.h"


//#include "DRV2\Interface.h"




void	drv2_init(void);


#endif /*_DRV2_H_*/

