#ifndef _IOPORT_H_
#define _IOPORT_H_

/* bit 0 in SCS register, port 0/1 are regular ports when bit 0 
is 0,  fast ports when bit 0 is 1. */
#define GPIOM			0x00000001

/* see master definition file lpc230x.h for more details */
#define REGULAR_PORT_DIR_BASE		GPIO_BASE_ADDR + 0x08
#define REGULAR_PORT_DIR_INDEX		0x10	

//#define HS_PORT_DIR_BASE			FIO_BASE_ADDR + 0x00

#define HS_PORT_DIR_BASE			FIO0DIR
#define HS_PORT_MASK_BASE			FIO0MASK
#define HS_PORT_PIN_BASE			FIO0PIN
#define HS_PORT_SET_BASE			FIO0SET
#define HS_PORT_CLR_BASE			FIO0CLR


#define HS_PORT_DIR_INDEX			0x8

#define FAST_PORT		0x01
#define REGULAR_PORT	0x02

#define DIR_OUT			0x01
#define DIR_IN			0x02


//////////////////////////////////////////////////////////////////////
/*IOPORT的宏定义*/

#define 	PORT_GET		0x2
#define 	PORT_SET		0x1
#define 	PORT_CLR		0x0
#define 	PORT_SEND_SET	0x3
#define 	PORT_KEY_SET	0x4
#define 	PORT_KEY_GET	0x5

#define 	PORT_ADCS		0		//P0.16
#define		PORT_LCDRST		1		//P0.10
#define		PORT_LCDBK		2		//P2.3
#define		PORT_LIGHT		3		//P2.10
#define 	PORT_MCIPWR		4		//P1.5
#define 	PORT_DO1		5
#define		PORT_DO2		6
#define		PORT_EIA485TR	7
#define		PORT_DI1		8
#define		PORT_DI2		9
#define		PORT_BP121		10
#define		PORT_BP122		11
#define		PORT_EINT2		12
#define		PORT_EINT3		13

#define		PORT_EINT1		14

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct ioport_info_tag{
	int		port_name;
	int		status;
}	ioport_info;


#define	EIA485_TXEN		0x1<<0		//P2.0
#define assert_txen485()	FIO2SET = EIA485_TXEN	//High to enable tx.
#define	dessert_txen485() 	FIO2CLR = EIA485_TXEN

/*外部接口函数*/
#ifdef _DRVFUNC_

/*ioport相关函数*/

extern void	ioport_init			(void);                           
extern int	ioport_read			(int addr, int* data);            
extern int	ioport_write		(int addr, int data);             
extern int	ioport_ioctl		(int io_cmd, 	void* parameter); 
#endif /*_DRVFUNC_*/

void	port_test(void);

void	eia485tx_set(INT16 status);	//Control eia485 port.

#ifdef __cplusplus 
}
#endif

#endif /*_IOPORT_H_*/

