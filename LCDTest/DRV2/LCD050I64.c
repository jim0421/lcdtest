#include "bsp.h"
#include <stdio.h>
#include "LCD050I64BIO.h"
#include "ioport.h"
#include "sys.h"

#include "lpc23xx.h"

#include "LCD050I64.h"
#include "drv2\lcd_disp.h"
#include "time1.h"

#define	LCD_TRYCNT 			5000 	//����LCD�ȴ���־λ���Դ���

#define TEST_LCDERR if(LCD_ERR > 16) return ERROR;
//#define TEST_LCDERR 

static	INT16	LCD_ERR=false;


/**
 *���LCD�Ĵ���״̬
 */
INT16	get_lcderr(void){
	return LCD_ERR;
}	

/**************************************************************************************************************
*LCD�Ļ�������(TC6963C)
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT8  LcdBusy()  // ״̬λSTA1,STA0�жϣ���дָ��Ͷ�д���ݣ�
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





/*��ʼ��LCD*/

void	lcd_init_2(void){

//	printf("Lcd Init.\n");

	device_ioctl(ID_LCD, LCD_RST, NULL);
	device_ioctl(ID_LCD, LCD_LGTON, NULL);

	delay(20);


	
	LCD_Fill(0xffff);
//	Lcd_Clr_Txt();
}

//========================================================================
// ����: void Write_Dot_LCD(int x,int y,unsigned int i)
// ����: ��LCD����ʵ����ϵ�ϵ�X��Y��������ɫΪi�ĵ�
// ����: x 		X������
//		 y 		Y������
//		 i 		Ҫ���ĵ����ɫ 
// ����: ��
// ��ע: Mz ͨ�ð�LCD�������� ��׼�Ӻ���
// �汾:
//      2006/10/15      First version
//		2007/01/09		V1.2 
//========================================================================
void Write_Dot_LCD(UINT16 x,UINT16 y,UINT16 Color)
{
	
	set_xy_add(x,y);
	set_reg1(LCDREG_PIX,Color);

}

//========================================================================
// ����: void Set_Dot_Addr_LCD(int x,int y)
// ����: ������LCD����ʵ����ϵ�ϵ�X��Y���Ӧ��RAM��ַ
// ����: x 		X������
//		 y 		Y������
// ����: ��
// �汾:
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
// ����: void LCD_Fill(unsigned int Data)
// ����: ���������Data��������������
// ����: Data   Ҫ������ɫ����
// ����: ��
// ��ע: Mz ͨ�ð�LCD�������� ��׼�Ӻ���
// �汾:
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



