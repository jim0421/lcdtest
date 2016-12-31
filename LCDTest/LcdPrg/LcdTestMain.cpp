#include <stdio.h>
#include "words.h"
#include "utility.h"
#include "components.h"
#include <graphics.h>
#include <lpc23xx.h>

#include "bsp.h"
#include <stdio.h>
#include "lcd050i64bio.h"
#include "ioport.h"
#include "sys.h"

void partialRefresh();
void refresh();
void drawLeft(int y, struct D_VALUEBRANCH value, bool open);
void drawRight(int y, struct D_VALUEBRANCH value, bool open);

/* Globle Variables */
int D_HEIGHT;
int D_OFFSET;
int D_NUM;
int D_SIDE;
int D_TYPE; // 1-6 according to the ??021?????? 
//stands for ?? and 1 stands for ??
bool D_OPEN; // true stands for ?? and false stands for ??
int D_NAME;
//+++++++++++++++
char *D_ADDR; // 458 address for ???
struct D_VALUEBRANCH *D_VALUE; // D_VALUE records all the values (color values 0-3 and numerical values)
struct D_SWITCHBRANCH *D_SWITCH; // D_SWITCH records all the switch states ( true stands for 松闸 and false stands for 合闸 )

/* the screen should be (320, 240) 
 * num:int -> number of display line (1-4) no error check
 * side:int -> style of layout (both:0, left:-1, right:1) no error check
 * open:bool -> whether the switch is open or closed
 * type:int -> 1-6 according to the 关于021给樊书华的信
 * old(0 stands for 压力 and 1 stands for 位移)
 * name:int -> 1-4 represents A-D
 * value:struct D_VALUEBRANCH * -> records all the colors and values
 * SWITCH:struct D_SWITCHBRANCH * -> records all the switch states
 * Chinese character gap -> 26px
 * w1 -> (20+56,30) w2 -> (230,30)
 */
void init_frame(int num, int side, int type, bool open, int name, char *addr, struct D_VALUEBRANCH *value, struct D_SWITCHBRANCH *SWITCH)
{
	D_HEIGHT = 20 * num + 10 * (num + 1);
	D_OFFSET = 65 + 10 * (4 - num);
	D_NUM = num;
	D_SIDE = side;
	D_OPEN = open;
	D_TYPE = type;
	D_NAME = name;
	D_ADDR = addr;
	D_VALUE = value;
	D_SWITCH = SWITCH;
	refresh();
}

/* type:int -> 1-6 according to the 关于021给樊书华的信
 * old(0 stands for 压力 and 1 stands for 位移)
 * open:bool -> whether the switch is open or closed
 * value:struct D_VALUEBRANCH * -> records all the colors and values
 * SWITCH:struct D_SWITCHBRANCH * -> records all the switch states
 */
void change_value(int type, bool open, struct D_VALUEBRANCH *value, struct D_SWITCHBRANCH *SWITCH)
{
	D_OPEN = open;
	D_VALUE = value;
	D_SWITCH = SWITCH;
	if (D_TYPE == type){
		partialRefresh();
	} else {
		D_TYPE = type;
		refresh();		
	}
}

void partialRefresh()
{
	// add partialClear
	partialClear(115, 15, 14, 12); //?? may cause position bug : logical bug
	if (D_OPEN)
		putWord(115, 15, 20);	//松
	else
		putWord(115, 15, 21);	//合
	for (int i = 0; i < D_NUM; i++){	// the data frame
		if (D_SIDE <= 0) {
			partialClear(141, D_OFFSET + 10 + 30 * i, 10, 21);
			drawLeft(D_OFFSET + 10 + 30 * i, D_VALUE[i], D_SWITCH[i].switch1);
		}
		if (D_SIDE >= 0) {
			partialClear(170, D_OFFSET + 10 + 30 * i, 10, 21);
			drawRight(D_OFFSET + 10 + 30 * i, D_VALUE[i], D_SWITCH[i].switch2);
		}
	}
}

/* The word display procedure can be modified into a simple module 
 * Take a list of word and the position
 * Draw the word
 */
void refresh()
{
	clear();
	putWord(10, 15, 22);	//?
	putWord(24, 15, 27);	//?
	putWord(39, 15, 10 + D_NAME);	//ABCD
	putNumber(54, 15, D_ADDR);
	if (D_SIDE < 0)
		putWord(53, 15, 44);	//L
	if (D_SIDE > 0)
		putWord(53, 15, 45);	//R
	if (D_OPEN)
		putWord(115, 15, 20);	//?
	else
		putWord(115, 15, 21);	//?
	for (int i = 0; i < 5; i++)
		putWord(129 + 14 * i, 15, 22 + i);	//?????
	drawRec(0, 35, 320, 35, 1);	// the line
	drawRec(151, D_OFFSET, 169, D_OFFSET + D_HEIGHT, 0);	// the tags
	for (int i = 0; i < D_NUM; i++){	// the data frame
		if (D_SIDE <= 0)
			drawLeft(D_OFFSET + 10 + 30 * i, D_VALUE[i], D_SWITCH[i].switch1);
		if (D_SIDE >= 0)
			drawRight(D_OFFSET + 10 + 30 * i, D_VALUE[i], D_SWITCH[i].switch2);
	}
	switch (D_TYPE){
		case 1:
			if (D_SIDE <= 0){
				//unit one
				putWord(26, D_OFFSET - 10, 28);	//??
				putWord(42, D_OFFSET - 10, 29);	
				putWord(26, D_OFFSET + D_HEIGHT, 41);	//??
				putWord(42, D_OFFSET + D_HEIGHT, 42);
				//unit two
				putWord(88, D_OFFSET - 10, 30);	//??
				putWord(104, D_OFFSET - 10, 31);
				putWord(88, D_OFFSET + D_HEIGHT, 41);	//??
				putWord(104, D_OFFSET + D_HEIGHT, 42);
			}
			if (D_SIDE >= 0){
				putWord(265, D_OFFSET - 10, 28);	
				putWord(280, D_OFFSET - 10, 29);
				putWord(265, D_OFFSET + D_HEIGHT, 41);
				putWord(280, D_OFFSET + D_HEIGHT, 42);

				putWord(204, D_OFFSET - 10, 30);
				putWord(219, D_OFFSET - 10, 31);	
				putWord(204, D_OFFSET + D_HEIGHT, 41);
				putWord(219, D_OFFSET + D_HEIGHT, 42);
			}
			break;
		case 2:
			if (D_SIDE <= 0){
				//unit one
				putWord(26, D_OFFSET - 10, 28);	//??
				putWord(42, D_OFFSET - 10, 29);	
				putWord(26, D_OFFSET + D_HEIGHT, 41);	//??
				putWord(42, D_OFFSET + D_HEIGHT, 42);
				//unit two
				if (D_OPEN){
					putWord(88, D_OFFSET - 10, 39);	//??
					putWord(104, D_OFFSET - 10, 40);
				}
				else{
					putWord(88, D_OFFSET - 10, 37);	//??
					putWord(104, D_OFFSET - 10, 38);
					putWord(88, D_OFFSET + D_HEIGHT, 41);	//??
					putWord(104, D_OFFSET + D_HEIGHT, 42);
				}
			}
			if (D_SIDE >= 0){
				putWord(265, D_OFFSET - 10, 28);	
				putWord(280, D_OFFSET - 10, 29);
				putWord(265, D_OFFSET + D_HEIGHT, 41);
				putWord(280, D_OFFSET + D_HEIGHT, 42);

				if (D_OPEN){
					putWord(204, D_OFFSET - 10, 39);	//??
					putWord(219, D_OFFSET - 10, 40);
				}
				else{
					putWord(204, D_OFFSET - 10, 37);	//??
					putWord(210, D_OFFSET - 10, 38);
					putWord(204, D_OFFSET + D_HEIGHT, 41);	//??
					putWord(219, D_OFFSET + D_HEIGHT, 42);
				}
			}
			break;
		case 3:
			if (D_SIDE <= 0){
				//unit one			
				putWord(26, D_OFFSET - 10, 28);	//??
				putWord(42, D_OFFSET - 10, 29);	
				putWord(26, D_OFFSET + D_HEIGHT, 41);	//??
				putWord(42, D_OFFSET + D_HEIGHT, 42);
				//unit two
				putWord(72, D_OFFSET - 10, 46);	//??
				putWord(88, D_OFFSET - 10, 47);
				putWord(104, D_OFFSET - 10, 28);	//??
				putWord(120, D_OFFSET - 10, 29);
				putWord(88, D_OFFSET + D_HEIGHT, 41);	//??
				putWord(104, D_OFFSET + D_HEIGHT, 42);
			}
			if (D_SIDE >= 0){
				putWord(265, D_OFFSET - 10, 28);	
				putWord(280, D_OFFSET - 10, 29);
				putWord(265, D_OFFSET + D_HEIGHT, 41);
				putWord(280, D_OFFSET + D_HEIGHT, 42);

				putWord(188, D_OFFSET - 10, 46);
				putWord(204, D_OFFSET - 10, 47);
				putWord(219, D_OFFSET - 10, 28);	
				putWord(235, D_OFFSET - 10, 29);	
				putWord(204, D_OFFSET + D_HEIGHT, 41);
				putWord(219, D_OFFSET + D_HEIGHT, 42);
			}
			break;
		case 4:
			if (D_SIDE <= 0){
				//unit one
				putWord(26, D_OFFSET - 10, 34);	//??
				putWord(42, D_OFFSET - 10, 35);	
				putWord(26, D_OFFSET + D_HEIGHT, 43);	//?
				//unit two
				putWord(88, D_OFFSET - 10, 30);	//??
				putWord(104, D_OFFSET - 10, 31);
				putWord(88, D_OFFSET + D_HEIGHT, 41);	//??
				putWord(104, D_OFFSET + D_HEIGHT, 42);
			}
			if (D_SIDE >= 0){
				putWord(265, D_OFFSET - 10, 34);	
				putWord(280, D_OFFSET - 10, 35);
				putWord(265, D_OFFSET + D_HEIGHT, 43);

				putWord(204, D_OFFSET - 10, 30);
				putWord(219, D_OFFSET - 10, 31);	
				putWord(204, D_OFFSET + D_HEIGHT, 41);
				putWord(219, D_OFFSET + D_HEIGHT, 42);
			}
			break;
		case 5:
			if (D_SIDE <= 0){
				//unit one
				putWord(26, D_OFFSET - 10, 34);	//??
				putWord(42, D_OFFSET - 10, 35);	
				putWord(26, D_OFFSET + D_HEIGHT, 43);	//?
				//unit two
				putWord(204, D_OFFSET - 10, 39);	//??
				putWord(219, D_OFFSET - 10, 40);
			}
			if (D_SIDE >= 0){
				putWord(265, D_OFFSET - 10, 34);	
				putWord(280, D_OFFSET - 10, 35);
				putWord(265, D_OFFSET + D_HEIGHT, 43);

				putWord(204, D_OFFSET - 10, 39);	//??
				putWord(219, D_OFFSET - 10, 40);
			}
			break;
		case 6:
			if (D_SIDE <= 0){
				//unit one
				putWord(26, D_OFFSET - 10, 34);	//??
				putWord(42, D_OFFSET - 10, 35);	
				putWord(26, D_OFFSET + D_HEIGHT, 43);	//?
				//unit two			
				putWord(72, D_OFFSET - 10, 46);	//??
				putWord(88, D_OFFSET - 10, 47);
				putWord(104, D_OFFSET - 10, 34);	//??
				putWord(120, D_OFFSET - 10, 35);
				putWord(88, D_OFFSET + D_HEIGHT, 43);	//?
			}
			if (D_SIDE >= 0){
				putWord(265, D_OFFSET - 10, 34);	
				putWord(280, D_OFFSET - 10, 35);
				putWord(265, D_OFFSET + D_HEIGHT, 43);

				putWord(188, D_OFFSET - 10, 46);
				putWord(204, D_OFFSET - 10, 47);
				putWord(219, D_OFFSET - 10, 34);	
				putWord(235, D_OFFSET - 10, 35);
				putWord(204, D_OFFSET + D_HEIGHT, 43);
			}
			break;
		default:
			;
			// can add error code
	}
}


extern "C" void LcdTest()
{

	initgraph(320,240);
	int num = 4;
	int side = 0;
	int type = 3;
	bool open = false;
	char *addr = "13";
	struct D_VALUEBRANCH haha[] = {
		1, "10.1", 2, "2", "3.4", 2, "4.2", 1,
		1, "10.1", 2, "2.3", "3.4", 2, "4.2", 1,
		1, "10.1", 2, "2.3", "3.4", 2, "4.2", 1,
		1, "10.12", 2, "2.3", "3.4", 2, "4.2", 1 };
	struct D_SWITCHBRANCH ha[] = {
		false,	true,
		true,	false,
		true,	true,
		false,	false};
	struct D_VALUEBRANCH hehe[] = {
		1, "2.3", 2, "2", "8.4", 2, "4.2", 1,
		1, "3.4", 2, "3.4", "2.33", 2, "4.2", 1,
		1, "9.7", 2, "2.22", "3.4", 2, "4.2", 1,
		1, "2.3", 2, "2.3", "3.4", 2, "4.2", 1 };
	struct D_SWITCHBRANCH he[] = {
		false,	false,
		false,	true,
		false,	false,
		false,	true};
	clear();
	init_frame(num,side,type,open,1,addr,haha,ha);
	closegraph();
}
