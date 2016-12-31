#include "bsp.h"
#include "drv2.h"
#include "stdio.h"
//#include "LCD_Driver_User.h"					//LCD底层驱动声明头文件
//#include "LCD_Config.h"							//LCD的配置文件，比如坐标轴是否倒置等的定义


//extern unsigned int Asii8[];					//6X8的ASII字符库
extern FLASH Asii0610[];			//6X10的ASII字符库
extern FLASH Asii0816[];			//8X16的ASII字符库
extern FLASH GB1716[];				//17*16自定义的汉字库
extern FLASH GB_16[];				//17*16自定义的汉字库

UINT8 X_Witch;									//字符写入时的宽度
UINT8 Y_Witch;									//字符写入时的高度
UINT8 Font_Wrod;									//字体的每个字模占用多少个存储单元数
FLASH *Char_TAB;									//字库指针

static BYTE	Font_type=0;								//标识字符类型
static BYTE	Font_Cover=0;								//字符显示的模式

static UINT16 FrontColor;
static UINT16 BackColor;
static UINT16 FontColor;

//========================================================================
// 函数: void	SetColor(UINT16 FrColor, UINT16 FColor, UINT16 BColor)
// 描述: 文本字体设置
// 参数: FrColor 图形前景色
//		 FColor 字体颜色
//		 BColor背景颜色
//
// 返回: 无
// 备注: 
// 版本:
//      2006/10/15      First version
//========================================================================
void	SetDColor(UINT16 FrColor, UINT16 FColor, UINT16 BColor){
	FrontColor = FrColor;
	BackColor = BColor;
	FontColor = FColor;	
}


//========================================================================
// 函数: void FontSet(int Font_NUM)
// 描述: 文本字体设置
// 参数: Font_NUM 字体选择,以驱动所带的字库为准
//
// 返回: 无
// 备注: 
// 版本:
//      2006/10/15      First version
//========================================================================
void FontSet(UINT8 Font_NUM)
{
	switch(Font_NUM)
	{
		case ASCII16: Font_Wrod = 16;	//ASII字符A
				X_Witch = 8;
				Y_Witch = 16;
			
				Font_type = 1;
				Char_TAB = Asii0816;
		break;
		case ASCII10: Font_Wrod = 10;	//ASII字符B
				X_Witch = 6;
				Y_Witch = 10;
		
				Font_type = 1;
				Char_TAB = Asii0610;
		break;		
		case HZ48: Font_Wrod = 48;	//汉字A
				X_Witch = 17;
				Y_Witch = 16;
			
				Font_type = 0;
				Char_TAB = GB1716;
		break;
		case HZ32: Font_Wrod = 32;	//汉字B
				X_Witch = 16;
				Y_Witch = 16;
		
				Font_type = 0;
				Char_TAB = GB_16;
		break;
		default: break;
	}
}
//========================================================================
// 函数: void FontMode(BYTE Mode) 
// 描述: 设置字符显示的模式,并设置背景色
// 参数: Mode	0: 字符显示时仅对字符的有效点进行显示操作,也就是不作背景覆盖
//					为叠加模式
//				1: 覆盖模式
//
// 返回: 无
// 备注: 仅对字符显示有效
// 版本:
//		2008/06/12		V2.0
//========================================================================
void FontMode(BYTE Mode)
{
	Font_Cover = Mode;
}
//========================================================================
// 函数: void PutChar(unsigned char x,unsigned char y,char a)  
// 描述: 写入一个标准字符
// 参数: x  X轴坐标     y  Y轴坐标
//		 a  要显示的字符在字库中的偏移量  
//		 Color字体的颜色
// 返回: 无
// 备注: ASCII字符可直接输入ASCII码即可
// 版本:
//      2006/10/15      First version
//		2007/01/11		V1.1
//		2008/06/12		V2.0
//========================================================================
void PutChar(UINT8 x,UINT8 y,char a)       
{
	UINT8 i,j;		//数据暂存
	FLASH *p_data;
	UINT8 Temp=0;
	BYTE Index = 0;
	if(Font_type==1)
		p_data = Char_TAB + (a-32)*Font_Wrod;
	else
		p_data = Char_TAB + a*Font_Wrod;	//要写字符的首地址
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
// 函数: void PutString(unsigned char x,unsigned char y,char *p)
// 描述: 在x、y为起始坐标处写入一串标准字符
// 参数: x  X轴坐标     y  Y轴坐标
//		 p  要显示的字符串  
// 返回: 无
// 备注: 仅能用于自带的ASCII字符串显示
// 版本:
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
// 函数: void PutPixel(int x,int y)
// 描述: 在x、y点上绘制一个前景色的点
// 参数: x  X轴坐标     y  Y轴坐标
// 返回: 无
// 备注: 使用前景色
// 版本:
//      2006/10/15      First version
//========================================================================
void PutPixel(UINT8 x,UINT8 y)
{
	Writ_Dot/*Write_Dot_LCD*/(x,y,FrontColor);
}
//========================================================================
// 函数: void Line(unsigned char s_x,unsigned char s_y,unsigned char e_x,unsigned char e_y)
// 描述: 在s_x、s_y为起始坐标，e_x、e_y为结束坐标绘制一条直线
// 参数: x  X轴坐标     y  Y轴坐标
// 返回: 无
// 备注: 使用前景色
// 版本:
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
// 函数: void W_Red_Dot(unsigned char x,unsigned char y,char a,char b,unsigned char mode)
// 描述: 绘制圆的各个像限中的点和线
// 参数: 
// 返回: 无
// 备注: 该函数对用户不可见，使用前景色
// 版本:
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
// 函数: void W_Red_Err(int *a,int *b,int *r)
// 描述: 画圆误差计算
// 参数: 
// 返回: 无
// 备注: 该函数对用户不可见
// 版本:
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
// 函数: void Circle(unsigned char x,unsigned char y,unsigned char r,unsigned char mode)
// 描述: 以x,y为圆心R为半径画一个圆(mode = 0) or 圆面(mode = 1)
// 参数: 
// 返回: 无
// 备注: 画圆函数执行较慢，如果MCU有看门狗，请作好清狗的操作
// 版本:
//      2006/10/16      First version
//========================================================================
void Circle(UINT8 x,UINT8 y,UINT8 r,BYTE mode)
{
	INT16 arx1=0,ary1,arx2,ary2=0;
//	Pos_Switch(&x,&y);						//坐标变换
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
// 函数: void Rectangle(UINT8 left, UINT8 top, UINT8 right, 
//						UINT8 bottom, BYTE Mode)
// 描述: 以x,y为圆心R为半径画一个圆(mode = 0) or 圆面(mode = 1)
// 参数: left - 矩形的左上角横坐标，范围0到118
//		 top - 矩形的左上角纵坐标，范围0到50
//		 right - 矩形的右下角横坐标，范围1到119
//		 bottom - 矩形的右下角纵坐标，范围1到51
//		 Mode - 绘制模式，可以是下列数值之一：
//				0:	矩形框（空心矩形）
//				1:	矩形面（实心矩形）
// 返回: 无
// 备注: 画圆函数执行较慢，如果MCU有看门狗，请作好清狗的操作
// 版本:
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
//		Pos_Switch(&left,&top);						// 坐标变换
//		Pos_Switch(&right,&bottom);					//坐标变换
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
// 函数: void ClrScreen(BYTE Mode)
// 描述: 清屏函数，执行全屏幕清除或填充前景色
// 参数: Mode   0:全屏除屏
//				1:全屏填充前景色
// 返回: 无
// 备注: 
// 版本:
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
 *用于和LcdPrg接口的函数
 *************************************************************************/

/*设置像素点*/
void putpixel(int x, int y, int color){
	Write_Dot_LCD(x, y,color);
}

/*图形初始化*/
void initgraph(int x, int y){
	return;
}

/*图形关闭*/
void closegraph(){
	return;
}





