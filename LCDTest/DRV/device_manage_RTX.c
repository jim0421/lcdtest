#include <lpc23xx.h>
#include "bsp.h"

#define _DRVFUNC_ 

#include "ioport.h"

#include "time1.h"


//#include "lpc22xxuart.h"

//#include "at25010.h"

#include <stdio.h>

//#include "ad7888.h"
//#include "lcd240128.h"
#include "LCD050I64BIO.h"

#include "drv.h"



#define DEVICE_NAME_LEN	8

#define MAXDEV_SIZE 32

/*timer0 相关函数, 为系统时钟*/
/*Timer 2 被操作系统时钟占用，RTX中不能调用*/
extern void Timer2_init			(void);                           
extern int	Timer2_read			(int addr, int* data);            
extern int	Timer2_write		(int addr, int data);             
extern int 	Timer2_ioctl		(int io_cmd, 	void* parameter); 

/*			
extern void timer0_init			(void);                           
extern int	timer0_read			(int addr, int* data);            
extern int	timer0_write		(int addr, int data);             
extern int 	timer0_ioctl		(int io_cmd, 	void* parameter); 
*/    

/*lpc22xx uart0 相关函数*/
extern void lpc22xxuart0_initb	(void);                           
extern int	lpc22xxuart0_readb	(int addr, int* data);            
extern int	lpc22xxuart0_writeb	(int addr, int data);             
extern int 	lpc22xxuart0_ioctlb	(int io_cmd, 	void* parameter);   

/*lpc22xx uart1 相关函数*/
extern void lpc22xxuart1_initb	(void);                           
extern int	lpc22xxuart1_readb	(int addr, int* data);            
extern int	lpc22xxuart1_writeb	(int addr, int data);             
extern int 	lpc22xxuart1_ioctlb	(int io_cmd, 	void* parameter);   

/*lpc22xx uart2 相关函数*/
extern void lpc22xxuart2_initb	(void);                           
extern int	lpc22xxuart2_readb	(int addr, int* data);            
extern int	lpc22xxuart2_writeb	(int addr, int data);             
extern int 	lpc22xxuart2_ioctlb	(int io_cmd, 	void* parameter);                                                      

extern void AT24C16a_init			(void);                           
extern int	AT24C16a_read			(int addr, int* data);            
extern int	AT24C16a_write		(int addr, int data);             
extern int 	AT24C16a_ioctl		(int io_cmd, 	void* parameter); 	   


extern void NVRAM_init			(void);                           
extern int	NVRAM_read			(int addr, int* data);            
extern int	NVRAM_write		(int addr, int data);             
extern int 	NVRAM_ioctl		(int io_cmd, 	void* parameter); 	  




/*函数指针定义*/
typedef void	(*device_init_fp)	(void);                           
typedef int		(*device_read_fp)	(int addr, int* data);            
typedef int		(*device_write_fp)	(int addr, int data);             
typedef int		(*device_ioctl_fp)	(int io_cmd, 	void* parameter); 


typedef struct device_tg{
	int					device_id;
	device_init_fp		device_init_p;
	device_read_fp		device_read_p;
	device_write_fp 	device_write_p;
	device_ioctl_fp 	device_ioctl_p;
}	device;

/*设备栈*/
static device device_stack[] ={
	{ID_IOPORT,		ioport_init,		ioport_read,		ioport_write,		ioport_ioctl},

	{ID_UART0,		lpc22xxuart0_initb,	lpc22xxuart0_readb,	lpc22xxuart0_writeb,lpc22xxuart0_ioctlb},
//	{ID_UART1,		lpc22xxuart1_initb,	lpc22xxuart1_readb,	lpc22xxuart1_writeb,	lpc22xxuart1_ioctlb},
//	{ID_UART2,		lpc22xxuart2_initb,	lpc22xxuart2_readb,	lpc22xxuart2_writeb,	lpc22xxuart2_ioctlb},
	{ID_TIMER,		Timer2_init,		Timer2_read,		Timer2_write,		Timer2_ioctl},
	{ID_TIMER1,		timer1_init,		timer1_read,		timer1_write,		timer1_ioctl},
//	{ID_AD7888,		ad7888_init,		ad7888_read,		ad7888_write,		ad7888_ioctl},
//	{ID_LCD,		lcd240128_init,		lcd240128_read,		lcd240128_write,	lcd240128_ioctl},
	{ID_LCD,		lcd050i64bio_init,	lcd050i64bio_read,	lcd050i64bio_write,	lcd050i64bio_ioctl},

//	{ID_RTC,		rtc_init,			rtc_read,			rtc_write,			rtc_ioctl},
	//{ID_AT24C16A,	AT24C16a_init,		AT24C16a_read,		AT24C16a_write,		AT24C16a_ioctl},	 
//	{ID_NVRAM,		NVRAM_init,			NVRAM_read,			NVRAM_write,		NVRAM_ioctl},

};

static INT16 	device_pointer[MAXDEV_SIZE];
void 	init_VIC(void);
 
/*对所有的设备进行初始化*/
void device_initial(){
	int i;

	init_VIC();
	for(i=0; i<MAXDEV_SIZE; i++){
		device_pointer[i] = -1;
	}
	
	for(i=0; i< sizeof(device_stack)/sizeof(device); i++){
		(*device_stack[i].device_init_p)();
		device_pointer[device_stack[i].device_id] = i;
	}


}

static int get_devindex(int dev_id){

	if(dev_id < 0 || dev_id >=MAXDEV_SIZE)
		return -1;
	return device_pointer[dev_id];
}

/*设备读*/
int	device_read(int device_id, int addr, int* data){

	int dev_index = get_devindex(device_id);
	if(dev_index == -1) return -1;
	
	return (*device_stack[dev_index].device_read_p)(addr, data);

}	

/*设备写*/
int	device_write(int device_id, int addr, int data){
	int dev_index = get_devindex(device_id);
	if(dev_index == -1) return -1;
	
	return (*device_stack[dev_index].device_write_p)(addr, data);

}


/*设备读*/
int	device_ioctl(int device_id, int io_cmd, void* parameter){
	int dev_index = get_devindex(device_id);
	if(dev_index == -1) return -1;
	return (*device_stack[dev_index].device_ioctl_p)(io_cmd, parameter);
}


/*Frequency related*/
UINT32 CPU_HCLKFREQ (void)
{
    UINT32  msel,nsel;
    UINT32  hclk_freq;
    

    msel         = (UINT32)(PLLCFG   & 0x7FFF);
	nsel 		 = (UINT32)(PLLCFG >> 16 ) & 0xff;

    hclk_freq     = 2*CPU_OSC_FREQ * (msel + 1) / (nsel +1);

	return hclk_freq;
}

UINT32	CPU_CCLKFREQ(void){
 	
	UINT32  cclk_freq;

	UINT32 	cclk_sel = CCLKCFG & 0xff;
    
    cclk_freq     = CPU_HCLKFREQ() / (cclk_sel +1);


    return (cclk_freq);
}

static INT32 CLK_DIV(UINT32 val){

	switch (val){
	 	case 0:
			return 4;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 8;
		default:
			return 1;
	}
}



UINT32		CPU_PCLKFREQ(UINT32 PCLK){

	UINT32 sel;
	UINT32 cclk_freq = CPU_CCLKFREQ();

	
	if(PCLK & 0x100)
		sel =  PCLKSEL1;
	else
		sel =  PCLKSEL0;
	
	if(PCLK == PCLK_RTC){
	 	if(((sel >> PCLK_RTC)&0x3) == 0x3)
			return cclk_freq/6;
	}

	return cclk_freq/CLK_DIV((sel>>(PCLK&0xff))&0x3);

}

void PCLK_print(void){

	UINT32 cclk_freq = CPU_CCLKFREQ();
	UINT32 sel0 = PCLKSEL0;
	UINT32 sel1 = PCLKSEL1;

	printf("PCLK_WDT   =%d kHz\n", CPU_PCLKFREQ(PCLK_WDT));
	printf("PCLK_TIMER0=%d kHz\n", CPU_PCLKFREQ(PCLK_TIMER0));
	printf("PCLK_TIMER1=%d kHz\n", CPU_PCLKFREQ(PCLK_TIMER1));
	printf("PCLK_UART0 =%d kHz\n", CPU_PCLKFREQ(PCLK_UART0));
	printf("PCLK_UART1 =%d kHz\n", CPU_PCLKFREQ(PCLK_UART1));
	printf("PCLK_PWM0  =%d kHz\n", CPU_PCLKFREQ(PCLK_PWM0));
	printf("PCLK_PWM1  =%d kHz\n", CPU_PCLKFREQ(PCLK_PWM1));
	printf("PCLK_I2C0  =%d kHz\n", CPU_PCLKFREQ(PCLK_I2C0));
	printf("PCLK_SPI   =%d kHz\n", CPU_PCLKFREQ(PCLK_SPI));
	printf("PCLK_RTC   =%d kHz\n", CPU_PCLKFREQ(PCLK_RTC ));
	printf("PCLK_SSP1  =%d kHz\n", CPU_PCLKFREQ(PCLK_SSP1 ));
	printf("PCLK_DAC   =%d kHz\n", CPU_PCLKFREQ(PCLK_DAC));
	printf("PCLK_ADC   =%d kHz\n", CPU_PCLKFREQ(PCLK_ADC));
	printf("PCLK_CAN1  =%d kHz\n", CPU_PCLKFREQ(PCLK_CAN1));
	printf("PCLK_CAN2  =%d kHz\n", CPU_PCLKFREQ(PCLK_CAN2));
	printf("PCLK_ACF   =%d kHz\n", CPU_PCLKFREQ(PCLK_ACF));


	printf("PCLK_BAT_RAM=%d kHz\n", CPU_PCLKFREQ(PCLK_BAT_RAM));
	printf("PCLK_GPIO  =%d kHz\n", CPU_PCLKFREQ(PCLK_GPIO));
	printf("PCLK_PCB   =%d kHz\n", CPU_PCLKFREQ(PCLK_PCB));
	printf("PCLK_I2C1  =%d kHz\n", CPU_PCLKFREQ(PCLK_I2C1 ));
	printf("PCLK_SSP0  =%d kHz\n", CPU_PCLKFREQ(PCLK_SSP0));
	printf("PCLK_TIMER2=%d kHz\n", CPU_PCLKFREQ(PCLK_TIMER2));
	printf("PCLK_TIMER3=%d kHz\n", CPU_PCLKFREQ(PCLK_TIMER3));
	printf("PCLK_UART2 =%d kHz\n", CPU_PCLKFREQ(PCLK_UART2));
	printf("PCLK_UART3 =%d kHz\n", CPU_PCLKFREQ(PCLK_UART3));
	printf("PCLK_I2C2  =%d kHz\n", CPU_PCLKFREQ(PCLK_I2C2));
	printf("PCLK_I2S   =%d kHz\n", CPU_PCLKFREQ(PCLK_I2S));
	printf("PCLK_MCI   =%d kHz\n", CPU_PCLKFREQ(PCLK_MCI));
	printf("PCLK_SYSCON=%d kHz\n", CPU_PCLKFREQ(PCLK_SYSCON));

}
