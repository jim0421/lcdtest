#ifndef _LCD_240128_2_H
#define _LCD_240128_2_H

#ifdef __cplusplus 
extern "C" {
#endif

#define LCD_XY_Switch		0				//显示时X轴和Y由交换
#define LCD_X_Rev			0				//显示时X轴反转
#define LCD_Y_Rev			0				//显示时Y轴反转

#define LCD_X_MAX			240-1			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			128-1			//屏幕的Y轴的物理宽度
			
#define LCD_XY_Switch		0				//显示时X轴和Y由交换
#define LCD_X_Rev			0				//显示时X轴反转
#define LCD_Y_Rev			0				//显示时Y轴反转

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		LCD_X_MAX
	#define Dis_Y_MAX		LCD_Y_MAX	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		LCD_Y_MAX
	#define Dis_Y_MAX		LCD_X_MAX	
#endif

#define LCD_INITIAL_COLOR	0xf000			//定义LCD屏初始化时的背景色

#define LCD_DIS_CIRCLE		1				//定义是否需要绘制圆形的功能

#define Pos_Switch(x,y)	
#define Writ_Dot(x,y,c)			Write_Dot_LCD(x,y,c)
#define Get_Dot(x,y)			Get_Dot_LCD(x,y)
#define Set_Dot_Addr(x,y)		Set_Dot_Addr_LCD(x,y)	

/************************************************************************************************
*LCD Command
*/

#define Match_xy(s,e)	if(s>e){s = s+e;e = s-e;s = s-e;}


void	lcd_init_2(void);
INT16 	LCD_TestStatus(UINT8 bitMatch);
void Write_Dot_LCD(UINT16 x,UINT16 y,UINT16 Color);
void 	Set_Dot_Addr_LCD(UINT8 x,UINT8 y);
void 	LCD_Fill(UINT16 Data);

void LCD_Read(void);
void	Set_Txt(UINT8 x, UINT8 y, UINT8 data);
void	Lcd_Clr_Txt(void);

void 	Lcd_DispOff(void);
void 	Lcd_DispOn(void);
#ifdef __cplusplus 
}
#endif

#endif /*_LCD_240128_2_H*/

