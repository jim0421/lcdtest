#include "bsp.h"
#include <stdio.h>
#include "LCD050I64BIO.h"
#include "ioport.h"
#include "sys.h"

#include "lpc23xx.h"

#include "LCD050I64.h"
#include "drv2\lcd_disp.h"
#include "time1.h"

#define	LCD_TRYCNT 			5000 	//定义LCD等待标志位尝试次数

#define TEST_LCDERR if(LCD_ERR > 16) return ERROR;
//#define TEST_LCDERR 

static	INT16	LCD_ERR=false;


/**
 *获得LCD的错误状态
 */
INT16	get_lcderr(void){
	return LCD_ERR;
}	

/**************************************************************************************************************
*LCD的基本操作(TC6963C)
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT8  LcdBusy()  // 状态位STA1,STA0判断（读写指令和读写数据）
{
	INT16	retrycnt = LCD_TRYCNT;

//	return OK;	//For lcd debug
	
	TEST_LCDERR
	do
	{
		retrycnt--;
		if(retrycnt <0) {
			LCD_ERR ++;
			printf("LCD ST01 Error.(%d)\n",LCD_ERR);
			return ERROR;
		}

	}
	while((lcdbusystate()));
	LCD_ERR = 0;
	return OK;
}





/*初始化LCD*/

void	lcd_init_2(void){

//	printf("Lcd Init.\n");

	device_ioctl(ID_LCD, LCD_RST, NULL);
	device_ioctl(ID_LCD, LCD_LGTON, NULL);

	delay(20);


	
	LCD_Fill(0xffff);
//	Lcd_Clr_Txt();
}

//========================================================================
// 函数: void Write_Dot_LCD(int x,int y,unsigned int i)
// 描述: 在LCD的真实坐标系上的X、Y点绘制填充色为i的点
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
//		 i 		要填充的点的颜色 
// 返回: 无
// 备注: Mz 通用版LCD驱动程序 标准子函数
// 版本:
//      2006/10/15      First version
//		2007/01/09		V1.2 
//========================================================================
void Write_Dot_LCD(UINT16 x,UINT16 y,UINT16 Color)
{
	
	set_xy_add(x,y);
	set_reg1(LCDREG_PIX,Color);

}

//========================================================================
// 函数: void Set_Dot_Addr_LCD(int x,int y)
// 描述: 设置在LCD的真实坐标系上的X、Y点对应的RAM地址
// 参数: x 		X轴坐标
//		 y 		Y轴坐标
// 返回: 无
// 版本:
//      2006/10/15      First version
//		2007/01/09		V1.2 
//========================================================================
void Set_Dot_Addr_LCD(UINT8 x,UINT8 y)
{

}



void 	Lcd_DispOff(void){

}
void 	Lcd_DispOn(void){

}


void	Lcd_Clr_Txt(void){


}


//========================================================================
// 函数: void LCD_Fill(unsigned int Data)
// 描述: 会屏填充以Data的数据至各点中
// 参数: Data   要填充的颜色数据
// 返回: 无
// 备注: Mz 通用版LCD驱动程序 标准子函数
// 版本:
//      2006/10/15      First version
//		2007/01/09		V1.2 
//========================================================================
void LCD_Fill(UINT16 color)
{
	set_sys_cmd((1<<Y_INC)|(1<<X_INC));
//	set_reg1(LCDREG_SCMD, 0x0);
	set_reg1(LCDREG_FILL, color&0xff);
	LcdBusy();

	set_sys_cmd((1<<Y_INC)|(1<<X_INC)|7);
}


void	lcd_rst(void){
 	//lcd_init_2();
	INT16	x,y;

	for(x = 0; x<640; x++) {
		for(y=0; y<480; y++){
			Write_Dot_LCD(x,y, 0x1f);	
		}
	}

}



