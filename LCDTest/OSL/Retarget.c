/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <stdio.h>
#include <rt_misc.h>
#include "bsp.h"
#include "sys.h"

#pragma import(__use_no_semihosting_swi)

#define CR     0x0D




struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int ch, FILE *f) {
//  return (sendchar(ch));
	volatile unsigned int i;
	
	int chCR = CR;
	if (ch == '\n'){
		while(device_write(STDIO_DEV, 0, chCR) != 0){
    		for (i=0; i<1000; i++) {
      			/*** DO NOTHING ***/
    		}
		}
	}
  	while (device_write(STDIO_DEV, 0, ch) != 0) {
    	for (i=0; i<1000; i++) {
      	/*** DO NOTHING ***/
    	}
  	}

  	return (ch);
}

/*------------------------------------------------------------------------------
fgetc waits until a character is received from the serial port.  This may not
be the exact desired operation (for example if the buffer is empty, this
function hangs waiting for a character to be received).
------------------------------------------------------------------------------*/
int fgetc(FILE *f) {
  int ch;

  do {
//    ch = sys_call(STDIO_RX, NULL);
	  ch = device_read(STDIO_DEV, 0, NULL);
  }
  while (ch == -1);

  return ((unsigned char) ch);
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
#if 0
//  volatile unsigned int i;

//  while (device_write(STDIO_DEV, 0, ch) != 0) {
//    for (i=0; i<1000; i++) {
      /*** DO NOTHING ***/
//    }
//  }
#endif
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
