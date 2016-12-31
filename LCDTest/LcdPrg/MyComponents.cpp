#include "components.h"
#include "words.h"
#include "utility.h"


/* draw right piston */
void drawC0(int x, int y)
{
	drawRec(x, y, x + 5, y + 20, 1);
	drawRec(x + 5, y + 9, x + 25, y + 11, 1);
}

void drawC00(int x, int y)
{
	drawRec(x, y, x + 5, y + 20, 1);
	drawRec(x + 5, y + 9, x + 30, y + 11, 1);
}

/* draw left piston */
void drawC1(int x, int y)
{
	drawRec(x, y, x + 5, y + 20, 1);
	drawRec(x - 20, y + 9, x, y + 11, 1);
}

void drawC11(int x, int y)
{
	drawRec(x, y, x + 5, y + 20, 1);
	drawRec(x - 25, y + 9, x, y + 11, 1);
}

/* draw button for light */
void drawC2(int x, int y, int color)
{
	drawRec(x, y, x + 10, y + 20, 0);
	changeBlockColor(x, y, color);
}

/* draw button for value */
void drawC3(int x, int y, char *value)
{
	drawRec(x, y, x + 37, y + 20, 0);
	if (value)
		putNumber(x + 3, y + 8, value);
}

/* draw left part */
void drawLeft(int y, struct D_VALUEBRANCH value, bool open)
{
	int x = 8;
	drawC2(x, y, value.col1);
	drawC3(x + 15, y, value.val1);
	drawC2(x + 61, y, value.col2);
	drawC3(x + 76, y, value.val2);
	if (open)
		drawC1(x + 133, y);
	else{
		drawC11(x + 138, y);
	}
}


/* draw right part */
void drawRight(int y, struct D_VALUEBRANCH value, bool open)
{
	int x = 302;
	if (open)
		drawC0(x - 128, y);
	else{
		drawC00(x - 133, y);
	}
	drawC3(x - 103, y, value.val3);
	drawC2(x - 61, y, value.col3);
	drawC3(x - 42, y, value.val4);
	drawC2(x, y, value.col4);
}
