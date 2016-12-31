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
//LCD ��ʼ��������һ������LCD���ļĴ�����ʼ��
extern void LCD_Init(void);	//��LCD_Driver_User.c�ж���ĺ��������б�Ҫ�ú�����������˿ڳ�ʼ������λ�Ȳ���
//��ֱ�ߺ�����s_x��s_yΪ��ʼ�㣬e_x��e_yΪ������
extern void Line(UINT8 s_x,UINT8 s_y,UINT8 e_x,UINT8 e_y);
//��ͼģʽ����ǰ��ͼ��ɫ����
extern void SetPaintMode(BYTE Mode,UINT8 Color);

//֧�ֺ����Լ�ASCII��д��
extern void PutStringAll(UINT8 x,UINT8 y,char *p);

//����������ִ��ȫ��Ļ��������ǰ��ɫ
extern void ClrScreen(BYTE Mode);

void putpixel(int x, int y, int color);

#ifdef __cplusplus 
}
#endif

#endif /*_LCD_DISP_H_*/

