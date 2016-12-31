#ifndef _BSP_H_
#define _BSP_H_

#include "typedef.h"

#ifdef __cplusplus 
extern "C" {
#endif

#define SIMUMODE
//#define REFBOARD	//采用开发板进行调试
#define WATCHDOGEN 1



#ifdef REFBOARD
	#define CPU_OSC_FREQ	14756	/*kHz*/
#else
	#define CPU_OSC_FREQ	12000	/*kHz*/
#endif
/*外部调用的函数入口*/

/*外部调用的函数通用返回值*/
#define		DEVICE_OK			1
#define		DEVICE_FAULT		-1
#define		DEVICE_DEFAULT		0
#define		DEVICE_NOEXIST		-2
#define		DEVICE_CMDNOEXIST	-3
#define 	DEVICE_WRERR		-4
#define		DEVICE_RDERR		-5
#define 	DEVICE_BUSERR		-6

/*设备名称定义*/
#define		ID_FLASH	1
#define		ID_LCD		2	
#define		ID_IOPORT	3
#define		ID_AD		4		
#define		ID_AT25010	5	
#define		ID_UART0	6	
#define		ID_TIMER	7
#define 	ID_TIMER1	8
#define 	ID_AD7888	9
#define 	ID_UART1	10
#define		ID_NVRAM	11
#define 	ID_DAC8531B	12
#define 	ID_AD7714	13
#define 	ID_AT24C16A	14
#define		ID_RTC		15
#define 	ID_UART2	16


void		device_initial(void);
int			device_read(int device_id, int addr, int* data);
int			device_write(int device_id, int addr, int data);
int 		device_ioctl(int device_id, int io_cmd, void* parameter);

   
#define	PCLK_WDT  		0
#define	PCLK_TIMER0		2
#define	PCLK_TIMER1		4
#define	PCLK_UART0 		6
#define	PCLK_UART1 		8
#define	PCLK_PWM0  		10
#define	PCLK_PWM1  		12
#define	PCLK_I2C0  		14
#define	PCLK_SPI   		16
#define	PCLK_RTC   		18
#define	PCLK_SSP1  		20
#define	PCLK_DAC   		22
#define	PCLK_ADC   		24
#define	PCLK_CAN1  		26
#define	PCLK_CAN2 		28
#define	PCLK_ACF   		30


#define	PCLK_BAT_RAM	0	| 0x100
#define	PCLK_GPIO  		2	| 0x100
#define	PCLK_PCB   		4	| 0x100
#define	PCLK_I2C1  		6	| 0x100
#define	PCLK_SSP0  		10	| 0x100
#define	PCLK_TIMER2		12	| 0x100
#define	PCLK_TIMER3		14	| 0x100
#define	PCLK_UART2 		16	| 0x100
#define	PCLK_UART3 		18	| 0x100
#define	PCLK_I2C2  		20	| 0x100
#define	PCLK_I2S   		22	| 0x100
#define	PCLK_MCI   		24	| 0x100
#define	PCLK_SYSCON		28	| 0x100

UINT32		CPU_PCLKFREQ(UINT32 PCLK);
UINT32 		CPU_CCLKFREQ(void);
UINT32 		CPU_HCLKFREQ (void);
void 		PCLK_print(void);





//////////////////////////////////////////////////////////////////////
/*Flash的宏定义*/
#define OFFSET_SET	0x1


/////////////////////////////////////////////////////////////////////
/*AD7714宏定义*/
#define AD7714_RDY	0



//Timer宏定义

//UART0宏定义
#define KEY_PRESS 1
#define TX_END	2
#define BAUDSET 3

//看门狗函数

void init_watchdog(void);	//看门狗初始化
void feed_dog(void);		//喂狗函数

//////////////////////////////////////////////////////////////////////////////////////
typedef volatile unsigned long LPC_REG;

UINT32	get_sysclk(void);
void 	sleep(INT16 duration);
void	viewdat(UINT8 *dat ,INT16 size);

void	rs232send(UINT8 *pdat, INT16 size);
void	rs232init(void);

enum DGB_ID {
	DBINFO_LVL = 0,
	DB_LVL,
	DB_485,
	DBIO_LVL,
	DBR_LVL,
	DBTIME_LVL,
	DB_KEY,
	DB_ERR,
};


void 	DebugPrint (INT16	level, char * format, ...);

#define MY_PI 3.14159265358979323846


#ifdef __cplusplus 
}
#endif

#endif /*_BSP_H_*/
