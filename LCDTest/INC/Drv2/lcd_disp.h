#ifndef _LCD_DISP_H_
#define _LCD_DISP_H_

#include "bsp.h"
#ifdef __cplusplus 
extern "C" {
#endif

enum FONTYPE {
	ASCII16=0,
	ASCII10,
	HZ48,
	HZ32,
};


extern void	SetDColor(UINT16 FrColor, UINT16 FColor, UINT16 BColor);
//LCD 初始化函数，一般用于LCD屏的寄存器初始化
extern void LCD_Init(void);	//在LCD_Driver_User.c中定义的函数，如有必要该函数还会包含端口初始化、复位等操作
//画直线函数，s_x、s_y为起始点，e_x、e_y为结束点
extern void Line(UINT8 s_x,UINT8 s_y,UINT8 e_x,UINT8 e_y);
//绘图模式、当前绘图颜色设置
extern void SetPaintMode(BYTE Mode,UINT8 Color);

//支持汉字以及ASCII的写入
extern void PutStringAll(UINT8 x,UINT8 y,char *p);

//清屏函数，执行全屏幕清除或填充前景色
extern void ClrScreen(BYTE Mode);

void putpixel(int x, int y, int color);

#ifdef __cplusplus 
}
#endif

#endif /*_LCD_DISP_H_*/

