/*------------------------------------------------------------------------------
SIO.C:  Serial Communication Routines for Philips LPC2100.

Copyright 2004-2006 KEIL Software, Inc.
------------------------------------------------------------------------------*/
#include <stdio.h>

#include "lpc22xxuartbase.h"
#include "sys.h"
#include "bspdef.h"
#include "irq.h"

//Baudrate table
typedef struct Bauduart_tag{
	
	UINT32	Baud;
	UINT16	UDL;
	UINT16	UDLM;
	UINT16	UDLL;
	UINT16	DivAddVal;
	UINT16	MulVal;
} Bauduart;

Bauduart Baudtable[]={	//Uart clock  36000000Hz
	/*Baud, UDL, 	UDLM, 	UDLL, 	DivAddVal, 	MulVal,*/
	{3200, 	125,	0,		125,	7,			8},
 	{9600, 	125,	0,		125,	7,			8},
	{14400,	125,	0,		125,	1,			4},
	{19200,	74,		0,		74,		7,			12},
	{28800,	71,		0,		71,		1,			10},
	{38400,	37,		0,		37,		7,			12},
	{57600,	23,		0,		23,		7,			10},
	{76800,	23,		0,		23,		3,			10},
	{115200,11,		0,		11,		7,			9},	 
	{230400,8,		0,		8,		2,			9},

   	//Uart clock = 35942400Hz
/*	{9600, 	234,	0,		234,	0,			2},
	{14400,	156,	0,		156,	0,			2},
	{19200,	117,	0,		117,	0,			2},
	{28800,	78,		0,		78,		0,			2},
	{38400,	39,		0,		39,		1,			2},
	{57600,	39,		0,		39,		0,			2},
	{76800,	28,		0,		28,		1,			8},
	{115200,13,		0,		13,		1,			2},	 
	{230400,9,		0,		9,		1,			12},  */
};


/*--------------------------- int_disable_eth -------------------------------*/

//#define DIS_UART0 VICIntEnClr=(1<<VIC_UART0);
//#define EN_UART0 VICIntEnable|=(1<<VIC_UART0);

//static struct 	buf_st rbuf = { 0, 0, };

//static struct 	buf_st tbuf = { 0, 0, };

//static unsigned int tx_restart = 1;   /* NZ if TX restart is required */

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void lpc22xxuartbase::irq (void){
	volatile char 	dummy;
	volatile char 	IIR;
	short			i;
	volatile char 	LSR_DAT;
	
//	struct 			buf_st *p;
//	IENABLE;

	/*------------------------------------------------
	Repeat while there is at least one interrupt source.
	------------------------------------------------*/
  	while (((IIR = uartp->IIR_FCR) & 0x01) == 0) {
		switch (IIR & 0x0E) {
			case 0x06: /* Receive Line Status */
				dummy = uartp->LSR;  /* Just clear the interrupt source */
				break;
		
			case 0x04: /* Receive Data Available */
			case 0x0C: /* Character Time-Out */
//				p = &rbuf;
				
				while(1){
					//if (((rx_buf.in - rx_buf.out) & ~(RBUF_SIZE-1)) == 0) {
					if ((rx_buf.in - rx_buf.out) < (RBUF_SIZE)) {
						rx_buf.buf [rx_buf.in & (RBUF_SIZE-1)] = uartp->RBR_THR_DLL;
						rx_buf.in++;
					}else{
					 	dummy = uartp->RBR_THR_DLL; //To clear the interrupt;
					}
					LSR_DAT = uartp->LSR;
					if((LSR_DAT & 0x1 ) == 0x0)	//Rx dat not avaliable.
						break;						
				}
		 		break;
		
			case 0x02: /* THRE Interrupt */
//		 		p = &tbuf;
		
				for(i=0 ; i<16; i++) {
					if (tx_buf.in != tx_buf.out) {
					   uartp->RBR_THR_DLL = tx_buf.buf [tx_buf.out & (TBUF_SIZE-1)];
					   tx_buf.out++;
					   tx_restart = 0;
					}
					else {
						tx_restart = 1;
						uartp->DLM_IER = LPCUART_RXEN;
					   	break;
					}
				}
				break;
		
		case 0x00: /* Modem Interrupt */
			 dummy = U1MSR;  /* Just clear the interrupt source */
			 break;
		
		default:
		 	break;
		}
	}
//	IDISABLE;	
	VICVectAddr = 0; /* Acknowledge Interrupt */
}
//void com_baudrate (unsigned int baudrate);


/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void lpc22xxuartbase::write_baud(void)
 {
	
	UINT32 Fdiv;
	UINT32 Fpclk;
	UINT32	pclkindex;

	INT16	i;
	for(i=0;i<sizeof(Baudtable)/sizeof(Bauduart); i++){
	 	if(Baudtable[i].Baud == baud){
			uartp->LCR |= 0x80;           /* Set DLAB */

			uartp->DLM_IER = Baudtable[i].UDLM;							
    		uartp->RBR_THR_DLL = Baudtable[i].UDLL;
			uartp->FDR =  (Baudtable[i].DivAddVal) |(Baudtable[i].MulVal<<4);
	
			uartp->LCR &= ~0x80;          /* Clear DLAB */
			return;
		}
	}

	switch(vicbias){
		case VIC_UART0:
			pclkindex =  PCLK_UART0;
			break;
		case VIC_UART1:
			pclkindex =  PCLK_UART1;
			break;
		case VIC_UART2:
			pclkindex =  PCLK_UART2;
			break;
		case VIC_UART3:
			pclkindex =  PCLK_UART3;
			break;
		default:
			pclkindex =  PCLK_UART0;
			break;
	}
	Fpclk = CPU_PCLKFREQ(pclkindex)*1000;
	Fdiv = ( Fpclk / 16 ) / baud ;	/*baud rate */
	
	uartp->LCR |= 0x80;           /* Set DLAB */
	uartp->DLM_IER = Fdiv / 256;							
    uartp->RBR_THR_DLL = Fdiv % 256;
	uartp->LCR &= ~0x80;          /* Clear DLAB */
 
 }			 
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
INT32 lpc22xxuartbase::uart_putchar (UINT8 c) {
//	struct buf_st *p = &tbuf;
	
	/*------------------------------------------------
	If the buffer is full, return an error value.
	------------------------------------------------*/
	VICIntEnClr=(1<<vicbias);  //Disable uart
	if (UART_BUFLEN(tx_buf) >= TBUF_SIZE){
		VICIntEnable|=(1<<vicbias);	 //Enable uart
		return (-1);
	}
	VICIntEnable|=(1<<vicbias);	//Enable uart
	
	/*------------------------------------------------
	Add the data to the transmit buffer.  If the
	transmit interrupt is disabled, then enable it.
	------------------------------------------------*/
	if (tx_restart)	{
		tx_restart = 0;
		uartp->RBR_THR_DLL = c;
		uartp->DLM_IER = LPCUART_TREN;
	}
	else {
		tx_buf.buf [tx_buf.in & (TBUF_SIZE - 1)] = c;
		tx_buf.in++;
	}
	
	return (0);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
INT32	lpc22xxuartbase::uart_getchar (){
//	struct buf_st *p = &rbuf;
	
	VICIntEnClr=(1<<vicbias);  //Disable uart
	if (UART_BUFLEN(rx_buf) == 0){
		VICIntEnable|=(1<<vicbias);	 //Enable uart
		return (-1);
	}
	VICIntEnable|=(1<<vicbias);	 //Enable uart
	
	return (rx_buf.buf [(rx_buf.out++) & (RBUF_SIZE - 1)]);
}




void lpc22xxuartbase::init (uart_lpc *up, void *irq_address, UINT32 vicpos) {
	volatile char dummy;
	/*------------------------------------------------
	Clear com buffer indexes.
	------------------------------------------------*/
	tx_buf.in = 0;
	tx_buf.out = 0;
	tx_restart = 1;
	
	rx_buf.in = 0;
	rx_buf.out = 0;

	vicbias = vicpos;	//Set the vic bias.
	uartp = up;		//Set base addr of the uart register
	/*------------------------------------------------
	Setup serial port registers.
	------------------------------------------------*/
//	PINSEL0 = 0x00000050;   
//	if(vicpos == VIC_UART2)
//		uartp->LCR = 0x7;
//	else
	install_irq( vicbias, (void *)irq_address, HIGHEST_PRIORITY );
		uartp->LCR = 0x03;				/* 8 bits, no Parity, 1 Stop bit */

//	set_baudrate(19200);	 //for 10MHz system 19200
	write_baud();

	uartp->IIR_FCR	= 0xc7;	
	uartp->DLM_IER = LPCUART_TRDIS;               	/* Disable UART1 Interrupts */
	 
	dummy = uartp->IIR_FCR;   			/* Read IrqID - Required to Get Interrupts Started */
//	if(uartp != lpc_uart1base)
		uartp->DLM_IER = LPCUART_RXEN;		/* Enable UART1 RX and THRE Interrupts */
}

INT32	lpc22xxuartbase::resetbaud(UINT32 baudrate){
	set_baudrate(baudrate);	 //for 10MHz system 19200
	write_baud();
	
	return 1;	
}

INT32	lpc22xxuartbase::kbhit(void){

	if(UART_BUFLEN(rx_buf)!=0) 	
		return 	TRUE;
	else
		return FALSE;
}

/*监测数据是否发送完毕*/

INT32	lpc22xxuartbase::txnearlyend(void){
   	return (tx_restart == 1);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

lpc22xxuartbase uart0;

void uart0_irq(void) __irq{
  	uart0.irq();
}

extern "C" void lpc22xxuart0_initb (void) {
	PINSEL0 = 0x00000050;
	uart0.set_baudrate(115200);   
 	uart0.init(lpc_uart0base, (void *)uart0_irq,VIC_UART0);
}

extern "C" int	lpc22xxuart0_writeb		(int addr, int data){
	int retval;

	UINT8	data8 = data & 0xff;

	if(addr == 0){
		retval = uart0.uart_putchar(data8);
		return retval;
	}
				
	return DEVICE_WRERR;
}

extern "C" int	lpc22xxuart0_readb		(int addr, int* data){
	int retval;

	if(addr == 0){
	
		retval = uart0.uart_getchar();

		if(data!=NULL)
			*data = retval;
		return retval;
	}
				
	return DEVICE_RDERR;
}


extern "C" int lpc22xxuart0_ioctlb		(int io_cmd,	void* parameter){
	if(io_cmd == KEY_PRESS)
		return (uart0.kbhit());
	else if(io_cmd == TX_END)
		return (uart0.txnearlyend());

	else return	DEVICE_CMDNOEXIST;
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

lpc22xxuartbase uart1;

void uart1_irq(void) __irq{
  	uart1.irq();
}

extern "C" void lpc22xxuart1_initb (void) {
	PINSEL7 |= 0x0000000F;	/* P3.16 TXD1, P3.17 RXD1 */
	uart1.set_baudrate(19200); 
//	uart1.set_baudrate(57600); 
//	uart1.set_baudrate(115200); 
//	PINSEL0 |= 0x1<<20|0x1<<22|0x1<<24|0x1<<26;	/*Enable RTS,CTS,DSR,DTR*/ 
 	uart1.init(lpc_uart1base, (void *)uart1_irq,VIC_UART1);
}

extern "C" int	lpc22xxuart1_writeb		(int addr, int data){
	int retval;

	UINT8	data8 = data & 0xff;

	if(addr == 0){
		retval = uart1.uart_putchar(data8);
		return retval;
	}
				
	return DEVICE_WRERR;
}

extern "C" int	lpc22xxuart1_readb		(int addr, int* data){
	int retval;

	if(addr == 0){
	
		retval = uart1.uart_getchar();

		if(data!=NULL)
			*data = retval;
		return retval;
	}
				
	return DEVICE_RDERR;
}


extern "C" int lpc22xxuart1_ioctlb		(int io_cmd,	void* parameter){
	if(io_cmd == KEY_PRESS)
		return (uart1.kbhit());
	else if(io_cmd == TX_END)
		return (uart1.txnearlyend());
	else return	DEVICE_CMDNOEXIST;
}


/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

lpc22xxuartbase uart2;

void uart2_irq(void) __irq{
  	uart2.irq();
}

extern "C" void lpc22xxuart2_initb (void) {
	PCONP |= 1<<24;	//Turn on power.
	PINSEL4 |= 0x000A0000;	/* P2.8 TXD2, P2.9 RXD2 */
//	uart2.set_baudrate(19200); 
	uart2.set_baudrate(57600); 
//	uart2.set_baudrate(38400); 
 	uart2.init(lpc_uart2base, (void *)uart2_irq,VIC_UART2);
}

extern "C" int	lpc22xxuart2_writeb		(int addr, int data){
	int retval;

	UINT8	data8 = data & 0xff;

	if(addr == 0){
		retval = uart2.uart_putchar(data8);
		return retval;
	}
				
	return DEVICE_WRERR;
}

extern "C" int	lpc22xxuart2_readb		(int addr, int* data){
	int retval;

	if(addr == 0){
	
		retval = uart2.uart_getchar();

		if(data!=NULL)
			*data = retval;
		return retval;
	}
				
	return DEVICE_RDERR;
}


extern "C" int lpc22xxuart2_ioctlb		(int io_cmd,	void* parameter){
	UINT32 dat;
	
	if(io_cmd == KEY_PRESS)
		return (uart2.kbhit());
	else if(io_cmd == TX_END)
		return (uart2.txnearlyend());
	else if(io_cmd == BAUDSET){
		dat = *((UINT32 *)parameter);
		return (uart2.resetbaud(dat));
	}
	else return	DEVICE_CMDNOEXIST;


}


