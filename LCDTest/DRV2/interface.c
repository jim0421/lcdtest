//#include "interface.h"
#include "bsp.h"
#include <stdio.h>
#include "drv2.h"
#include "DisSensor.h"
#include "ioport.h"
#include "appinfo.h"

#define		FONT_COLOR	LCD_BLUE
#define		BACK_COLOR	LCD_BLACK
#define		FRONT_COLOR	LCD_WHITE



EleInterface ELEPIC[MAXELE];
EleInterface ELETXT[MAXELE];
/**********************************************************************************************************/
DispBuf	DispEle[ELE_SIZE];

ElePos DispElePos[]={
	
	{8,4,//{111,18},{{41,24},{41,51},{41,78},{41,106},{136,24},{136,51},{136,78},{136,106}},   //Txt_Pos
	     //       {{17,19},{17,46},{17,73},{17,101},{123,19},{123,46},{123,73},{123,101}},	 //Pic_Pos;
				{{0,0},{111,18},{17,19},{17,46},{17,73},{17,101},{123,19},{123,46},{123,73},{123,101}}},		//GUI_Pos
	{6,3,//{111,18},{{41,24},{41,51},{41,78},{136,24},{136,51},{136,78},{136,106}},   //Txt_Pos
	     //       {{17,19},{17,46},{17,73},{123,19},{123,46},{123,73},{123,101}},	 //Pic_Pos;
				{{0,0},{111,18},{17,19},{17,46},{17,73},{123,19},{123,46},{123,73}}},		//GUI_Pos
	{4,2,//{111,18},{{41,24},{41,51},{136,24},{136,51},{136,78},{136,106}},   //Txt_Pos
	     //       {{17,19},{17,46},{123,19},{123,46},{123,73},{123,101}},	 //Pic_Pos;
				{{0,0},{111,18},{17,19},{17,46},{123,19},{123,46}}},		//GUI_Pos
	{2,1,//{111,18},{{41,24},{136,24},{136,51},{136,78},{136,106}},   //Txt_Pos
	     //       {{17,19},{123,19},{123,46},{123,73},{123,101}},	 //Pic_Pos;
				{{0,0},{111,18},{17,19},{123,19}}},		//GUI_Pos

};

/**************************************************************************************************/
INT16	GetStyle(void);



point_ele *Get_DispGUIPos(INT16	DispStyle, INT16 Ele_Index){

	return &(DispElePos[DispStyle].GUI_Pos[Ele_Index]);
}

INT16 Get_DispEleSize(INT16	DispStyle){

	return (DispElePos[DispStyle].Ele_Size);
}

INT16 Get_DispEleLeft_Size(INT16	DispStyle){

	return (DispElePos[DispStyle].EleLeft_Size);
}

//point_ele *Get_DispBreakPos(INT16	DispStyle){

//	return &(DispElePos[DispStyle].BreakPos);
//}

/*********************************************************************************************************/
/*********************************************************************************************************/
static INT16 	ViewMode=VIEW_BRON;
/*********************************************************************************************************/		 
/*Set View*/
void	SetView(INT16 View){
	if(ViewMode != View){
		if(View == VIEW_BRON || View == VIEW_BROFF)
			IntPic_Refresh();
	}
	ViewMode = View;
//	printf("View=%d\n",ViewMode);
}

/*Get View*/
INT16	GetViewMode(){
	return ViewMode;

}
/*********************************************************************************************************/



/*********************************************************************************************************/
INT16	RunFunKey(void){
	static	INT16	KeyCnt = 0;
	INT32	portstatus;
  	device_read(ID_IOPORT, PORT_EINT1, &portstatus);
	if(portstatus == 0)
		KeyCnt ++;
	else
		KeyCnt = 0;

	if(KeyCnt == 5)
		return 1;
	else
		return 0;
}

/*********************************************************************************************************/

void	DisplayEle(point_ele *elep, INT16	Index, INT8	*Buffer){

   	INT16	i;

	INT16	x,y;

	INT16	size;
	INT8	CharNull = ' ';

	size = (DispEle[Index].Size >DISBUFF)? DISBUFF:DispEle[Index].Size; 

   	if(Index <0 ||Index >= ELE_SIZE)
		return;

	y = elep->y;
	x = elep->x;
	
	for(i=0; i<size; i++){
		if(Buffer[i] == 0) {
			DispEle[Index].Buffer[i] = 0;
	//		PutChar(x, y , (CharNull));
			break;
		}
		if(DispEle[Index].Buffer[i] != Buffer[i])
		{
			DispEle[Index].Buffer[i] = Buffer[i];
			PutChar(x, y , (Buffer[i]));

		}
		x += 8;

	}
	for(; i<size; i++){
		
		if(DispEle[Index].Buffer[i] != 0){
			DispEle[Index].Buffer[i] = 0;
			PutChar(x, y , (CharNull));
	
			x+=8;
		}
	}
	PutChar(x, y , (CharNull));

}




void	SetElePos(INT16	Index, point_ele *p){
	DispEle[Index].Pos.y = p->y;
	DispEle[Index].Pos.x = p->x;
}
/******************************************************************************************************/
/*更新BreakState*/
/******************************************************************************************************/
static	INT16 GUI_mode = GUI_MOVE;
void 	ViewStateUpdate(INT16 Key){

   	if(Key == 1)
		GUI_mode = (GUI_mode+1) % (GUI_END-1);

	if(GUI_mode == GUI_MOVE || GUI_mode == GUI_POS){
		if(GetBrakeState()== BRAKE_CLOSE){
		  	SetView(VIEW_BRON);
		}
		else
			SetView(VIEW_BROFF);
	}else if(GUI_mode == GUI_DEBUG)
		SetView(VIEW_DEBUG);

	if(Key == 1)
		IntTxt_Refresh();
	

/*	if(GetViewMode()== VIEW_BROFF)
		SetView(VIEW_BRON);
	else
		SetView(VIEW_BROFF);
*/		
}

/*获得GUI模式*/
INT16	GetGUIMode(void){
 	return GUI_mode;
}


void	Draw_EleTxt(point_ele *elep, INT16 chan ,INT16 BuffID){
//	INT8	buffer[16];
//	static 	int i=0;

//	i++;
//	sprintf(buffer,"M %d", i);
 //   PutString(10,10,buffer);
//	
	if(GUI_mode == GUI_POS)
		DisplayEle(elep, BuffID , Get_Dis(chan));
	else if(GUI_mode == GUI_MOVE)
		DisplayEle(elep, BuffID , Get_DisMove(chan));
//	printf("AD %d %s\n", chan, Get_Dis(chan));
//	DisplayEle(elep, BuffID , buffer);

}


void	Interface_Load(INT16 style){

	INT16		i=0;
	point_ele	*ele_pos;

//	for(i=0; i<Get_DispEleLeft_Size(style); i++){
//		//Write Pic
//		ele_pos = Get_DispPicPos(style, i);
//		ELEPIC[i].is_valid = TRUE;
//		ELEPIC[i].Pos.x = ele_pos->x;
//		ELEPIC[i].Pos.y = ele_pos->y;
//		ELEPIC[i].f = Draw_ElePicLeft;

		//Write Txt
//		ele_pos = Get_DispTxtPos(style, i);
//		ELETXT[i].is_valid = TRUE;
//		ELETXT[i].Pos.x = ele_pos->x;
//		ELETXT[i].Pos.y = ele_pos->y;
//		ELETXT[i].f = Draw_EleTxt;
//		ELETXT[i].chan = i;

//	}
	
//	for(; i<Get_DispEleSize(style); i++){
//		ele_pos = Get_DispPicPos(style, i);
//		ELEPIC[i].is_valid = TRUE;
//		ELEPIC[i].Pos.x = ele_pos->x;
//		ELEPIC[i].Pos.y = ele_pos->y;
//		ELEPIC[i].f = Draw_ElePicRight;

		//Write Txt
//		ele_pos = Get_DispTxtPos(style, i);
//		ELETXT[i].is_valid = TRUE;
//		ELETXT[i].Pos.x = ele_pos->x;
//		ELETXT[i].Pos.y = ele_pos->y;
//		ELETXT[i].f = Draw_EleTxt;
//		ELETXT[i].chan = i;
//	}
   	//Label
	i=0;
	ele_pos = Get_DispGUIPos(style,i);
	ELEPIC[i].is_valid = TRUE;
	ELEPIC[i].Pos.x = ele_pos->x;
	ELEPIC[i].Pos.y = ele_pos->y;
	ELEPIC[i].f = GUI_Label_Pic;

   	ELETXT[i].is_valid = TRUE;
	ELETXT[i].Pos.x = ele_pos->x;
	ELETXT[i].Pos.y = ele_pos->y;
	ELETXT[i].f = GUI_Label_Txt;
	ELETXT[i].chan = 0;

	i++;
	//刹车盘
	ele_pos = Get_DispGUIPos(style,i);
	ELEPIC[i].is_valid = TRUE;
	ELEPIC[i].Pos.x = ele_pos->x;
	ELEPIC[i].Pos.y = ele_pos->y;
	ELEPIC[i].f = GUI_Wheel_Pic;

   	ELETXT[i].is_valid = TRUE;
	ELETXT[i].Pos.x = ele_pos->x;
	ELETXT[i].Pos.y = ele_pos->y;
	ELETXT[i].f = GUI_Wheel_Txt;
	ELETXT[i].chan = 0;

	i++;

	for(; i<Get_DispEleLeft_Size(style)+2; i++){
		//Write Pic
		ele_pos = Get_DispGUIPos(style, i);
		ELEPIC[i].is_valid = TRUE;
		ELEPIC[i].Pos.x = ele_pos->x;
		ELEPIC[i].Pos.y = ele_pos->y;
		ELEPIC[i].f = GUI_BrLeft_Pic;

		//Write Txt
//		ele_pos = Get_DispTxtPos(style, i);
		ELETXT[i].is_valid = TRUE;
		ELETXT[i].Pos.x = ele_pos->x;
		ELETXT[i].Pos.y = ele_pos->y;
		ELETXT[i].f = GUI_BrLeft_Txt;
		ELETXT[i].chan = i-2;

	}
#if 1
	for(; i<Get_DispEleSize(style)+2; i++){
		//Write Pic
		ele_pos = Get_DispGUIPos(style, i);
		ELEPIC[i].is_valid = TRUE;
		ELEPIC[i].Pos.x = ele_pos->x;
		ELEPIC[i].Pos.y = ele_pos->y;
		ELEPIC[i].f = GUI_BrRight_Pic;

		//Write Txt
//		ele_pos = Get_DispTxtPos(style, i);
		ELETXT[i].is_valid = TRUE;
		ELETXT[i].Pos.x = ele_pos->x;
		ELETXT[i].Pos.y = ele_pos->y;
		ELETXT[i].f = GUI_BrRight_Txt;
		ELETXT[i].chan = i-2;

	}
#endif
	for( ;i<MAXELE; i++){
		ELEPIC[i].is_valid = FALSE;
		ELETXT[i].is_valid = FALSE;
	}
}


void	IntPic_Refresh(void){

	INT16	i;
//	point_ele	*ele_pos;
	for(i=0; i<MAXELE; i++){
	  	if(ELEPIC[i].is_valid == FALSE) break;
		RUNINTERFACE((&(ELEPIC[i])));
	}

//	ele_pos = Get_DispBreakPos(GetStyle());
//	Rectangle(ele_pos->x, ele_pos->y,ele_pos->x+11,ele_pos->y+109,1);

}

void	IntTxt_Refresh(void){

	INT16	i;
	for(i=0; i<MAXELE; i++){
	  	if(ELETXT[i].is_valid == FALSE) break;
		RUNINTERFACETXT((&(ELETXT[i])),i);
	}

}

void	DisplayTxtForce(void){
	INT16	i;
	for(i=0; i<ELE_SIZE; i++){
		INT16	j;
		for(j=0; j< DISBUFF; j++){
			DispEle[i].Buffer[j] = 0xFF;
		}
	}
	IntTxt_Refresh();
	
}

/*界面布局设置*/
static INT16	style;

INT16	GetStyle(){
	return style;
}

/******************************************************************************************************/
/*界面初始化*/
/******************************************************************************************************/
void 	Interface_Init(){
 	INT16 	i;

	for(i=0;i<MAXELE;i++){
		ELEPIC[i].is_valid = FALSE;
	}
	for(i=0;i<MAXELE;i++){
		ELETXT[i].is_valid = FALSE;
	}

	for(i=0; i< ELE_SIZE; i++){
		DispEle[i].Size = 7;
	}
	/*清屏*/
	ClrScreen(0);

	/*设置字体*/
	FontSet(0,);
	/*图形模式*/
	SetPaintMode(0,1);

	//写入模式
	FontMode(1,BACK_COLOR);

	style = Get_ViewStyle();
	Interface_Load(style);
	DisplayTxtForce();
	IntPic_Refresh();

}



void	Interface_Test(void){

}


