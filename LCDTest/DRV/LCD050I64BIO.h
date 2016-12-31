#include <lpc23xx.h>


#ifndef _LCD050I64BIO_H_
#define _LCD050I64BIO_H_



/*Command for ioctl */
#define 	LCD_RST		0
#define		LCD_LGTON	1
#define		LCD_LGTOFF	2

/*LCD addr*/
#define		LCDREG_X	0
#define		LCDREG_Y	1
#define		LCDREG_PIX	2
#define		LCDREG_8S	3
#define		LCDREG_F	4
#define		LCDREG_B	5
#define		LCDREG_SCMD	6
#define 	LCDREG_FILL	7

#define 	INC_DIR		15
#define		Y_INC		14
#define		X_INC		13
#define		BACK_F		12
#define		WR_L		7
#define		DISP_M		3
#define		LUM			0		


#define		LCD_ADDR	(*((volatile unsigned char *)(STATIC_MEM2_BASE+0x0)))		 	//Write ADD register and read busy flag
#define 	LCD_DATA	(*((volatile unsigned char *)(STATIC_MEM2_BASE+0xff)))			//Write register and read pixel.


#define		lcd_data(DAT) 	LCD_DATA=DAT
#define		lcd_addr(DAT)	LCD_ADDR=DAT
#define		lcdbusyreg		LCD_ADDR	

UINT8	lcdbusystate(void);	 			//Return lcd busy state.

void	set_xy_add(UINT16 x, UINT16 y);	//Set address of pixel

void	set_sys_cmd(UINT16 cmd);   		//Set command with 2 cmd
//void	set_sys_cmd1(UINT16 cmd);   		//Set command with 1 cmd

void	set_reg1(UINT8 regaddr, UINT16 dat);
void	set_reg2(UINT8 regaddr, UINT16 dat);

void	lcd050i64bio_init	(void);

/*addr = 1 cmd, addr=0 dat*/
int		lcd050i64bio_read	(int addr, int* data);

/*addr = 1 cmd, addr=0 dat*/
int		lcd050i64bio_write	(int addr, int data);

int		lcd050i64bio_ioctl	(int io_cmd, 	void* parameter);

int		lcd050i64bio_test(void);

INT16	get_lcderr(void);


#endif /*_LCD050I64BIO_H_*/
