#include "components.h"
#include <graphics.h>
#include "utility.h"
/* clear the screen */
void clear()
{
	for (int x = 0; x < 320; x++)
		for (int y = 0; y < 240; y++)
			putpixel(x, y, WHITE);
}

/* partially clear the screen */
void partialClear(int x_pos, int y_pos, int x_len, int y_len)
{
	for (int x = x_pos; x < x_pos + x_len; x++)
		for (int y = y_pos; y < y_pos + y_len; y++)
			putpixel(x, y, WHITE);
}

/* change the color of the alarming block */
void changeBlockColor(int x, int y, int COLOR)
{
	for (int i = x + 1; i < 10 + x; i++)
		for (int j = y + 1; j < 20 + y; j++)
//			putpixel(x, y, COLOR);
			switch (COLOR){
				case 0:	putpixel(i, j, WHITE);
					break;
				case 1:	putpixel(i, j, RED);
					break;
				case 2: putpixel(i, j, YELLOW);
					break;
				case 3: putpixel(i, j, BLACK);
					break;
			}
}

/* dir(direction):0,1,2,3 -> down,up,right,left */
void drawLine(int x, int y, int dir, int length)
{
	for (int i = 0; i < length; i++){
		switch (dir){
		case 0: putpixel(x, y + i, BLACK);
			break;
		case 1: putpixel(x, y - i, BLACK);
			break;
		case 2: putpixel(x + i, y, BLACK);
			break;
		case 3: putpixel(x - i, y, BLACK);
			break;
		}
	}
}

/* abs max min need redefining
 * get the diagnol coordiantes (x1,y1) (x2,y2)
 * isFull:true -> the rectangle is fully printed 
 * can be modified a bit */
void drawRec(int x1, int y1, int x2, int y2, bool isFull)
{

	int x_length = abs(x1 - x2);
	int y_length = abs(y1 - y2);
	
	int x_min = min(x1, x2);
	int y_min = min(y1, y2);
//	int x_max = max(x1, x2);
//	int y_max = max(y1, y2);
	
	for (int i = 0; i < x_length; i++){
		for (int j = 0; j < y_length; j++){
			if (isFull)
				putpixel(x_min + i, y_min + j, BLACK);
			else
				putpixel(x_min + i, y_min + j, WHITE);
		}
	}
	
	if (x1 < x2){
		drawLine(x1, y1, 2, x_length + 1);
		drawLine(x2, y2, 3, x_length + 1);
	}
	else{
		drawLine(x1, y1, 3, x_length + 1);
		drawLine(x2, y2, 2, x_length + 1);
	}
	if (y1 < y2){
		drawLine(x1, y1, 0, y_length + 1);
		drawLine(x2, y2, 1, y_length + 1);
	}
	else{
		drawLine(x1, y1, 1, y_length + 1);
		drawLine(x2, y2, 0, y_length + 1);
	}

}
