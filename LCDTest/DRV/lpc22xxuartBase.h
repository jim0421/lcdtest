
#ifndef _LPC22XXUARTBASE_H_
#define _LPC22XXUARTBASE_H_

#include <lpc23xx.h>
#include "bsp.h"

typedef struct uart_tag{
	LPC_REG RBR_THR_DLL;	//(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
	//LPC_REG U0THR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
	//LPC_REG U0DLL          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
	
	LPC_REG DLM_IER;         //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
	//LPC_REG U0IER          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
	
	LPC_REG IIR_FCR;         //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
	//LPC_REG U0FCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))

	LPC_REG LCR;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x0C))
	LPC_REG	Reserved1[1];
	LPC_REG LSR;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x14))
	LPC_REG	Reserved2[1];
	LPC_REG SCR;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x1C))
	LPC_REG ACR;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x20))
	LPC_REG ICR;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x24))
	LPC_REG FDR;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x28))
	LPC_REG	Reserved3[1];
	LPC_REG TER;          //(*(volatile unsigned long *)(UART0_BASE_ADDR + 0x30))
} uart_lpc, *uart_lpcp;


#define lpc_uart0base      	((uart_lpcp) UART0_BASE_ADDR) // (UART0) Base Address
#define lpc_uart1base      	((uart_lpcp) UART1_BASE_ADDR) // (UART0) Base Address
#define lpc_uart2base      	((uart_lpcp) UART2_BASE_ADDR) // (UART0) Base Address
#define lpc_uart3base     	((uart_lpcp) UART3_BASE_ADDR) // (UART0) Base Address


#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80


/*------------------------------------------------------------------------------
Notes:

The length of the receive and transmit buffers must be a power of 2.

Each buffer has a next_in and a next_out index.

If next_in = next_out, the buffer is empty.

(next_in - next_out) % buffer_size = the number of characters in the buffer.
------------------------------------------------------------------------------*/
#define TBUF_SIZE   1024	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   1024      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

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


typedef struct buf_st_tag{
	unsigned int in;          /* Next In Index */
	unsigned int out;         /* Next Out Index */
	char buf [RBUF_SIZE];     /* Buffer */
} buff_uart;

#define UART_BUFLEN(buff) ((unsigned int)(buff.in - buff.out))




class lpc22xxuartbase{
public:
	lpc22xxuartbase(){baud = 9600;}
	~lpc22xxuartbase(){}

public:
	uart_lpc*	uartp;
	UINT32		vicbias;	//Bias of the uart intterrupt bit.	
	buff_uart	tx_buf;
	buff_uart	rx_buf;
	UINT32		tx_restart;
	UINT32		baud;
private:
	void		write_baud(void);

public:
	void		init(uart_lpc *up, void *irq_address, UINT32 vicpos);
	void		irq(void);
	void 		set_baudrate(UINT32 baudrate){
		baud = baudrate;
	}

	INT32		resetbaud(UINT32 baudrate);

	INT32 		uart_putchar (UINT8 c);
	INT32		uart_getchar ();
	
	INT32		kbhit(void);
	INT32		txnearlyend(void);

};



#endif /*_LPC22XXUARTBASE_H_*/

