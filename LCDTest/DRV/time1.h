#ifndef _TIME1_H_
#define _TIME1_H_


#define EN_ADSAMPLING	0
#define DIS_ADSAMPLING	1

#ifdef _DRVFUNC_ 
/*timer1 Ïà¹Øº¯Êý*/
extern void timer1_init			(void);                           
extern int	timer1_read			(int addr, int* data);            
extern int	timer1_write		(int addr, int data);             
extern int 	timer1_ioctl		(int io_cmd, 	void* parameter); 
#endif /*_DRVFUNC_*/

#endif /*_TIME1_H_*/

