#include "bsp.h"
#include "drv2.h"
#include "stdio.h"
//#include "LCD_Driver_User.h"					//LCD�ײ���������ͷ�ļ�
//#include "LCD_Config.h"							//LCD�������ļ��������������Ƿ��õȵĶ���


//extern unsigned int Asii8[];					//6X8��ASII�ַ���
extern FLASH Asii0610[];			//6X10��ASII�ַ���
extern FLASH Asii0816[];			//8X16��ASII�ַ���
extern FLASH GB1716[];				//17*16�Զ���ĺ��ֿ�
extern FLASH GB_16[];				//17*16�Զ���ĺ��ֿ�

UINT8 X_Witch;									//�ַ�д��ʱ�Ŀ��
UINT8 Y_Witch;									//�ַ�д��ʱ�ĸ߶�
UINT8 Font_Wrod;									//�����ÿ����ģռ�ö��ٸ��洢��Ԫ��
FLASH *Char_TAB;									//�ֿ�ָ��

static BYTE	Font_type=0;								//��ʶ�ַ�����
static BYTE	Font_Cover=0;								//�ַ���ʾ��ģʽ

static UINT16 FrontColor;
static UINT16 BackColor;
static UINT16 FontColor;

//========================================================================
// ����: void	SetColor(UINT16 FrColor, UINT16 FColor, UINT16 BColor)
// ����: �ı���������
// ����: FrColor ͼ��ǰ��ɫ
//		 FColor ������ɫ
//		 BColor������ɫ
//
// ����: ��
// ��ע: 
// �汾:
//      2006/10/15      First version
//========================================================================
void	SetDColor(UINT16 FrColor, UINT16 FColor, UINT16 BColor){
	FrontColor = FrColor;
	BackColor = BColor;
	FontColor = FColor;	
}


//========================================================================
// ����: void FontSet(int Font_NUM)
// ����: �ı���������
// ����: Font_NUM ����ѡ��,�������������ֿ�Ϊ׼
//
// ����: ��
// ��ע: 
// �汾:
//      2006/10/15      First version
//========================================================================
void FontSet(UINT8 Font_NUM)
{
	switch(Font_NUM)
	{
		case ASCII16: Font_Wrod = 16;	//ASII�ַ�A
				X_Witch = 8;
				Y_Witch = 16;
			
				Font_type = 1;
				Char_TAB = Asii0816;
		break;
		case ASCII10: Font_Wrod = 10;	//ASII�ַ�B
				X_Witch = 6;
				Y_Witch = 10;
		
				Font_type = 1;
				Char_TAB = Asii0610;
		break;		
		case HZ48: Font_Wrod = 48;	//����A
				X_Witch = 17;
				Y_Witch = 16;
			
				Font_type = 0;
				Char_TAB = GB1716;
		break;
		case HZ32: Font_Wrod = 32;	//����B
				X_Witch = 16;
				Y_Witch = 16;
		
				Font_type = 0;
				Char_TAB = GB_16;
		break;
		default: break;
	}
}
//========================================================================
// ����: void FontMode(BYTE Mode) 
// ����: �����ַ���ʾ��ģʽ,�����ñ���ɫ
// ����: Mode	0: �ַ���ʾʱ�����ַ�����Ч�������ʾ����,Ҳ���ǲ�����������
//					Ϊ����ģʽ
//				1: ����ģʽ
//
// ����: ��
// ��ע: �����ַ���ʾ��Ч
// �汾:
//		2008/06/12		V2.0
//========================================================================
void FontMode(BYTE Mode)
{
	Font_Cover = Mode;
}
//========================================================================
// ����: void PutChar(unsigned char x,unsigned char y,char a)  
// ����: д��һ����׼�ַ�
// ����: x  X������     y  Y������
//		 a  Ҫ��ʾ���ַ����ֿ��е�ƫ����  
//		 Color�������ɫ
// ����: ��
// ��ע: ASCII�ַ���ֱ������ASCII�뼴��
// �汾:
//      2006/10/15      First version
//		2007/01/11		V1.1
//		2008/06/12		V2.0
//========================================================================
void PutChar(UINT8 x,UINT8 y,char a)       
{
	UINT8 i,j;		//�����ݴ�
	FLASH *p_data;
	UINT8 Temp=0;
	BYTE Index = 0;
	if(Font_type==1)
		p_data = Char_TAB + (a-32)*Font_Wrod;
	else
		p_data = Char_TAB + a*Font_Wrod;	//Ҫд�ַ����׵�ַ
	j = 0;
	while((j ++) < Y_Witch)
	{
		if(y > Dis_Y_MAX) break;
		i = 0;
		while(i < X_Witch)
		{
			if((i&0x07)==0)
			{
//				Temp = *(p_data + (Index>>1));
//				if((Index&0x01)==0)Temp = Temp>>8; 
				Temp = *(p_data+Index);
				Index++;
			}
			if((Temp & 0x80) > 0) Writ_Dot/*Write_Dot_LCD*/((x+i),y,FontColor);
			else if(Font_Cover) Writ_Dot/*Write_Dot_LCD*/((x+i),y,BackColor);
			Temp = Temp << 1;
			if((x+i) >= Dis_X_MAX) 
			{
				Index += (X_Witch-i)>>3;
				break;
			}
			i++;
		}
		y ++;
	}
}

INT16	GetGB16_Index(INT8 *HZ_P);

void PutHZ(UINT8 x,UINT8 y, char* HZ_P){

	INT16	Index;
	Index = GetGB16_Index(HZ_P);

	PutChar(x,y,(char)Index);

}

//========================================================================
// ����: void PutString(unsigned char x,unsigned char y,char *p)
// ����: ��x��yΪ��ʼ���괦д��һ����׼�ַ�
// ����: x  X������     y  Y������
//		 p  Ҫ��ʾ���ַ���  
// ����: ��
// ��ע: ���������Դ���ASCII�ַ�����ʾ
// �汾:
//      2006/10/15      First version
//========================================================================
void PutString(UINT8 x,UINT8 y,char *p)
{
	while(*p!=0)
	{
		PutChar(x,y,*p);
		x += X_Witch;
		if((x + X_Witch) > Dis_X_MAX)
		{
			x = 0;
			if((Dis_Y_MAX - y) < Y_Witch) break;
			else y += Y_Witch;
		}
		p++;
	}
}

void PutHZString(UINT8 x,UINT8 y,char *p)
{
	while(*p!=0){

		PutHZ(x,y,p);
		x += X_Witch;
		if((x + X_Witch) > Dis_X_MAX)
		{
			x = 0;
			if((Dis_Y_MAX - y) < Y_Witch) break;
			else y += Y_Witch;
		}
		p+=2;
	}
}

void PutStringAll(UINT8 x,UINT8 y,char *p)
{
	while(*p!=0){
		if((*p)&0x80) {
			FontSet(HZ32);
			PutHZ(x,y,p);
			p+=2;
		}
		else{
			FontSet(ASCII16);
			PutChar(x,y,*p);
			p++;
		}
		x += X_Witch;
		if((x + X_Witch) > Dis_X_MAX)
		{
			x = 0;
			if((Dis_Y_MAX - y) < Y_Witch) break;
			else y += Y_Witch;
		}
		
	}
}

//========================================================================
// ����: void PutPixel(int x,int y)
// ����: ��x��y���ϻ���һ��ǰ��ɫ�ĵ�
// ����: x  X������     y  Y������
// ����: ��
// ��ע: ʹ��ǰ��ɫ
// �汾:
//      2006/10/15      First version
//========================================================================
void PutPixel(UINT8 x,UINT8 y)
{
	Writ_Dot/*Write_Dot_LCD*/(x,y,FrontColor);
}
//========================================================================
// ����: void Line(unsigned char s_x,unsigned char s_y,unsigned char e_x,unsigned char e_y)
// ����: ��s_x��s_yΪ��ʼ���꣬e_x��e_yΪ�����������һ��ֱ��
// ����: x  X������     y  Y������
// ����: ��
// ��ע: ʹ��ǰ��ɫ
// �汾:
//      2006/10/15      First version
//========================================================================
void Line(UINT8 s_x,UINT8 s_y,UINT8 e_x,UINT8 e_y)
{  
	signed short Offset_x,Offset_y,Offset_k = 0;
	signed short Err_d = 1;

//	printf("%d %d %d %d\n", s_x, s_y,  e_x,  e_y);
	if(s_y>e_y)
	{
		Offset_x = s_x;
		s_x = e_x;
		e_x = Offset_x;
		Offset_x = s_y;
		s_y = e_y;
		e_y = Offset_x;
	} 
	Offset_x = e_x-s_x;
	Offset_y = e_y-s_y;
	Writ_Dot(s_x,s_y,FrontColor);
    if(Offset_x<0)
	{
		Offset_x = s_x-e_x;
       // Err_d = s_x;
       // s_x = e_x;
       // e_x = Err_d;
		Err_d = -1;
	}
    if(Offset_x==0)
    {
        while(s_y<e_y)
        {
            s_y++;
            if(s_y>Dis_Y_MAX) return;
            Writ_Dot/*Write_Dot_LCD*/(s_x,s_y,FrontColor);
        }
        return;
    }
    else if(Offset_y==0)
    {
        while(s_x!=e_x)
        {
            s_x+=Err_d;
            if(s_x>Dis_X_MAX) return;
            Writ_Dot/*Write_Dot_LCD*/(s_x,s_y,FrontColor);
        }
        return;
    }
	if(Offset_x>Offset_y)
	{
		Offset_k += Offset_y;
		while(s_x!=e_x)
		{
			if(Offset_k>0)
			{
				s_y+=1;
				Offset_k += (Offset_y-Offset_x);
			}
			else Offset_k += Offset_y; 
			s_x+=Err_d;
			if(s_x>Dis_X_MAX||s_y>Dis_Y_MAX) break;
			Writ_Dot/*Write_Dot_LCD*/(s_x,s_y,FrontColor);
		}	
	}
	else
	{
		Offset_k += Offset_x;
		while(s_y!=e_y)
		{
			if(Offset_k>0)
			{
				s_x+=Err_d;
				Offset_k += (Offset_x-Offset_y);
			}
			else Offset_k += Offset_x;
			s_y+=1;
			if(s_x>Dis_X_MAX||s_y>Dis_Y_MAX) break;
			Writ_Dot/*Write_Dot_LCD*/(s_x,s_y,FrontColor);
		}
	}    
}
#if LCD_DIS_CIRCLE==1
//========================================================================
// ����: void W_Red_Dot(unsigned char x,unsigned char y,char a,char b,unsigned char mode)
// ����: ����Բ�ĸ��������еĵ����
// ����: 
// ����: ��
// ��ע: �ú������û����ɼ���ʹ��ǰ��ɫ
// �汾:
//      2006/10/15      First version
//========================================================================
void W_Red_Dot(UINT8 x,UINT8 y,INT16 a,INT16 b,BYTE mode)
{
    if(mode > 0)
    {
       Line(x+a,y+b,x-a,y+b);
       Line(x+a,y-b,x-a,y-b);
    }
    else
    {
       PutPixel(x+a,y+b);
       PutPixel(x-a,y+b);
       PutPixel(x+a,y-b);
       PutPixel(x-a,y-b);
    }
}
//========================================================================
// ����: void W_Red_Err(int *a,int *b,int *r)
// ����: ��Բ������
// ����: 
// ����: ��
// ��ע: �ú������û����ɼ�
// �汾:
//      2006/10/16      First version
//========================================================================
void W_Red_Err(INT16 *a,INT16 *b,UINT8 *r)
{
	INT16 R_Error;
	WORD uiTemp;
	R_Error = (*a+1)*(*a+1);
	uiTemp = (*b)*(*b);
	R_Error += uiTemp;
	uiTemp = (*r)*(*r);
	R_Error -= uiTemp;
	if(R_Error>=0)
	{
		R_Error = R_Error-*b;
		if(R_Error>=0) *b = *b-1;
	}
	*a = *a+1;
}
//========================================================================
// ����: void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: 
// ����: ��
// ��ע: ��Բ����ִ�н��������MCU�п��Ź����������幷�Ĳ���
// �汾:
//      2006/10/16      First version
//========================================================================
void Circle(UINT8 x,UINT8 y,UINT8 r,BYTE mode)
{
	INT16 arx1=0,ary1,arx2,ary2=0;
//	Pos_Switch(&x,&y);						//����任
//	x += 4;
	ary1=r;
	arx2=r;
	while(1)
	{
        W_Red_Dot(x,y,arx1,ary1,mode);
        W_Red_Err(&arx1,&ary1,&r);
		if(arx1 == arx2)
		{
			W_Red_Dot(x,y,arx1,ary1,mode);
			break; 
		}
		W_Red_Dot(x,y,arx2,ary2,mode);
		W_Red_Err(&ary2,&arx2,&r);
		if(arx1 == arx2) 
		{
			W_Red_Dot(x,y,arx2,ary2,mode);
			break;
		}
	}
}	
#endif
//========================================================================
// ����: void Rectangle(UINT8 left, UINT8 top, UINT8 right, 
//						UINT8 bottom, BYTE Mode)
// ����: ��x,yΪԲ��RΪ�뾶��һ��Բ(mode = 0) or Բ��(mode = 1)
// ����: left - ���ε����ϽǺ����꣬��Χ0��118
//		 top - ���ε����Ͻ������꣬��Χ0��50
//		 right - ���ε����½Ǻ����꣬��Χ1��119
//		 bottom - ���ε����½������꣬��Χ1��51
//		 Mode - ����ģʽ��������������ֵ֮һ��
//				0:	���ο򣨿��ľ��Σ�
//				1:	�����棨ʵ�ľ��Σ�
// ����: ��
// ��ע: ��Բ����ִ�н��������MCU�п��Ź����������幷�Ĳ���
// �汾:
//      2005/05/21      First version
//========================================================================
void Rectangle(UINT8 left, UINT8 top, UINT8 right, UINT8 bottom, BYTE Mode)
{
	UINT8 uiTemp;
	
	if(Mode==0)
	{
		Line(left,top,left,bottom);
		Line(left,top,right,top);
		Line(right,bottom,left,bottom);
		Line(right,bottom,right,top);
	}
	else
	{
//		Pos_Switch(&left,&top);						// ����任
//		Pos_Switch(&right,&bottom);					//����任
		if(left>right)
		{
			uiTemp = left;
			left = right;
			right = uiTemp;
		}
		if(top>bottom)
		{
			uiTemp = top;
			top = bottom;
			bottom = uiTemp;
		}
		for(uiTemp=top;uiTemp<=bottom;uiTemp++)
		{
			Line(left,uiTemp,right,uiTemp);
		}
	}
}
//========================================================================
// ����: void ClrScreen(BYTE Mode)
// ����: ����������ִ��ȫ��Ļ��������ǰ��ɫ
// ����: Mode   0:ȫ������
//				1:ȫ�����ǰ��ɫ
// ����: ��
// ��ע: 
// �汾:
//      2007/01/11      First version
//========================================================================
void ClrScreen(BYTE Mode)
{
	if(Mode==0)
		LCD_Fill(BackColor);
	else
		LCD_Fill(FrontColor);
}

/*************************************************************************
 *���ں�LcdPrg�ӿڵĺ���
 *************************************************************************/

/*�������ص�*/
void putpixel(int x, int y, int color){
	Write_Dot_LCD(x, y,color);
}

/*ͼ�γ�ʼ��*/
void initgraph(int x, int y){
	return;
}

/*ͼ�ιر�*/
void closegraph(){
	return;
}





