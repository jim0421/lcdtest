#ifndef _BSPDEF_H_

#define _BSPDEF_H_

#ifdef __cplusplus
extern "C" {
#endif
/***************************************************************************
 **
 **  VIC Interrupt channels
 **
 ***************************************************************************/
#define VIC_WDT          0  /* Watchdog                           */
#define VIC_SW           1  /* Software interrupts                */
#define VIC_DEBUGRX      2  /* Embedded ICE, DbgCommRx            */
#define VIC_DEBUGTX      3  /* Embedded ICE, DbgCommTx            */
#define VIC_TIMER0       4  /* Timer 0 (Match 0-3 Capture 0-3)    */
#define VIC_TIMER1       5  /* Timer 1 (Match 0-3 Capture 0-3)    */
#define VIC_UART0        6  /* UART 0  (RLS, THRE, RDA, CTI)      */
#define VIC_UART1        7  /* UART 1  (RLS, THRE, RDA, CTI, MSI) */
#define VIC_PWM0         8  /* PWM 0   (Match 0-6 Capture 0-3)    */
#define VIC_I2C          9  /* I2C 0   (SI)                       */
#define VIC_SPI         10  /* SPI 0   (SPIF, MODF)               */
#define VIC_SSP         11  /* SSP                                */
#define VIC_PLL         12  /* PLL lock (PLOCK)                   */
#define VIC_RTC         13  /* RTC      (RTCCIF, RTCALF)          */
#define VIC_EINT0       14  /* External interrupt 0 (EINT0)       */
#define VIC_EINT1       15  /* External interrupt 1 (EINT1)       */
#define VIC_EINT2       16  /* External interrupt 2 (EINT2)       */
#define VIC_EINT3       17  /* External interrupt 3 (EINT3)       */
#define VIC_AD0         18  /* A/D converter 0                    */
#define VIC_I2C1        19  /* I2C 1                              */
#define VIC_BOD         20  /* Brown out detect                   */
#define VIC_AD1         21  /* A/D converter 1                    */
#define VIC_USB         22  /* USB Low and High priority          */
#define	VIC_CAN			23
#define VIC_SD			24
#define VIC_GPDMA		25
#define VIC_TIMER2		26
#define VIC_TIMER3		27
#define VIC_UART2		28
#define VIC_UART3		29
#define VIC_I2C2		30
#define VIC_I2S			31


#define LPCUART_TXEN 0x2
#define LPCUART_RXEN 0x1
#define LPCUART_TREN 0x3
#define LPCUART_TRDIS 0x0


//#define	EIA485_TXEN	1<<11			//P0.11
//#define en485_tx()	IO0SET = EIA485_TXEN
//#define	dis485_tx()	IO0CLR = EIA485_TXEN


/*------------------------------------------------------------------------------
Notes:

The length of the receive and transmit buffers must be a power of 2.

Each buffer has a next_in and a next_out index.

If next_in = next_out, the buffer is empty.

(next_in - next_out) % buffer_size = the number of characters in the buffer.
------------------------------------------------------------------------------*/
//#define TBUF_SIZE   256	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
//#define RBUF_SIZE   256      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
/*
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

*/


//struct buf_st {
//	unsigned int in;          /* Next In Index */
//	unsigned int out;         /* Next Out Index */
//	char buf [RBUF_SIZE];     /* Buffer */
//};



/*
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))
*/
#ifdef __cplusplus
}
#endif

#endif /*_BSPDEF_H_*/

