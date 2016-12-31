#include <stdio.h>
#include "bsp.h"


void vt100Init(void)
{
#ifndef SIMUMODE
	// initializes terminal to "power-on" settings
	// ESC c
	printf("\x1B\x63");
#else
	return;
	
#endif

}

void vt100ClearScreen(void)
{
#ifndef SIMUMODE
	// ESC [ 2 J
	printf("\x1B[2J");
#else
	return;
	
#endif
}

void vt100SetAttr(UINT8 attr)
{
#ifndef SIMUMODE
	// ESC [ Ps m
	printf("\x1B[%dm",attr);
#else
	return;
	
#endif
}

void vt100SetCursorMode(UINT8 visible)
{
#ifndef SIMUMODE
	if(visible)
	    // ESC [ ? 25 h
	    printf("\x1B[?25h");
	else
	    // ESC [ ? 25 l
	    printf("\x1B[?25l");
#else
	return;
	
#endif
}

void vt100SaveCursorAttr(void)
{
#ifndef SIMUMODE
	// <ESC>[s
	printf("\x1B%d",7);
#else
	return;
	
#endif
}

void vt100RestoreCursorAttr(void)
{
#ifndef SIMUMODE
	// <ESC>[u
	printf("\x1B%d",8);
#else
	return;
	
#endif
}


void vt100SetCursorPos(UINT8 line, UINT8 col)
{
#ifndef SIMUMODE
	// ESC [ Pl ; Pc H
	printf("\x1B[%d;%dH",line,col);
#else
	return;
	
#endif
}

void vt100StrollDown(void)
{
#ifndef SIMUMODE
	// <ESC>D
	printf("\x1B\x44");
#else
	return;
	
#endif
}

void vt100StrollUp(void)
{
#ifndef SIMUMODE
	// <ESC>M
	printf("\x1B\x4D");
#else
	return;
	
#endif
}

void vt100StrollScreen(UINT8 start, UINT8 end)
{
#ifndef SIMUMODE
	// <ESC>[{start};{end}r
	printf("\x1B[%d;%dr",1, 25);
#else
	return;
	
#endif
}
