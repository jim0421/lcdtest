#include <lpc23xx.h>

#include "bsp.h"
#include "sys.h"
#include <stdio.h>
#include "ioport.h"


typedef struct ioport_tag{
	UINT8	port;
	UINT32	mask;
	UINT32	pos;
	UINT8	ioro;
} ioport;

ioport	ioport_sys[] = {
//	{2, 0xff, 0, DIR_OUT},		//P2.0, P2.7 
	{0, 0x1, 16, DIR_OUT},		//SPI0 CS
	{0, 0x1, 10, DIR_OUT},		//LCD RST
	{2, 0x1, 3,  DIR_OUT},	    //LCD BKLIGHT
	{1, 0x1, 20, DIR_OUT},	    //LCD Light
	{1, 0x1, 5, DIR_OUT},	    //PWR MCI

	{2, 0x1, 1, DIR_OUT},		//DO1	;5
	{2, 0x1, 2, DIR_OUT},		//DO2	;6

	{2, 0x1, 0, DIR_OUT},		//EIA485_TR;7

	{0, 0x1, 4, DIR_IN},		//DI1;8
	{0, 0x1, 5, DIR_IN},		//DI2;9
	{1, 0x1, 21, DIR_IN},		//BP121;10
	{1, 0x1, 22, DIR_IN},		//BP122;11
	{2, 0x1, 12, DIR_IN},		//EINT2;12
	{2, 0x1, 13, DIR_IN},		//EINT3;13
	{2, 0x1, 11, DIR_IN},		//EINT1;13



//	{3, 0x1, 30, DIR_OUT},       //P3.30
//	{2, 0x1, 13, DIR_IN},       //P2.13 
//	{2, 0x1, 10, DIR_IN},       //P2.10 


};                              
                                
/*****************************************************************************
** Function name:		GPIOInit
**
** Descriptions:		initialize GPIO port
**
** parameters:			port number and port type, Fast I/O or 
** 						regular GPIO, direction
** Returned value:		None
** 
*****************************************************************************/
void GPIOInit( UINT32 PortNum, UINT32 PortPos, UINT32 PortDir )
{
  //Fast port only
	
	if ( PortDir == DIR_OUT )
	{
	  *(&HS_PORT_DIR_BASE 
			+ PortNum * HS_PORT_DIR_INDEX) |= PortPos;
	}
	else
	{
	  (*(&HS_PORT_DIR_BASE 
			+ PortNum * HS_PORT_DIR_INDEX)) &= ~PortPos;
    }
  	return;
}



void	ioport_init		(void){
	int i;
	SCS |= GPIOM;	/* set GPIOx to use Fast I/O */
	for(i=0; i<sizeof(ioport_sys)/sizeof(ioport); i++){
		GPIOInit(ioport_sys[i].port, ioport_sys[i].mask<<ioport_sys[i].pos, ioport_sys[i].ioro); 
	}
}

int		ioport_read		(int addr, int* data){

	UINT32 	port_val;

	port_val = 	 (*(&HS_PORT_PIN_BASE 
			+ (ioport_sys[addr].port) * HS_PORT_DIR_INDEX));
	port_val >>= ioport_sys[addr].pos;
	port_val &= ioport_sys[addr].mask;

	if(data!= NULL)
		*data = port_val;

	return port_val;
	
}


int		ioport_write	(int addr, int data){

	UINT32 set_dat = (data & ioport_sys[addr].mask) << ioport_sys[addr].pos;
	UINT32 clr_dat = ((~data) & ioport_sys[addr].mask) << ioport_sys[addr].pos;

	if(addr == PORT_ADCS)
		printf("dsfjsa;lkdf;sdf\n");

	(*(&HS_PORT_SET_BASE + (ioport_sys[addr].port) * HS_PORT_DIR_INDEX)) = set_dat;
	(*(&HS_PORT_CLR_BASE + (ioport_sys[addr].port) * HS_PORT_DIR_INDEX)) = clr_dat;

	return DEVICE_OK;
}


int		ioport_ioctl	(int io_cmd, 	void* parameter){
	

	return DEVICE_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EIA485 port trx switch
#define	EIA485_TXEN		0x1<<0		//P2.0


void	eia485tx_set(INT16 status){
 	if(status == PORT_SET)
		FIO2SET = EIA485_TXEN;	//High to enable tx.
	else
		FIO2CLR = EIA485_TXEN;
}


void	port_test(void){

	INT8	*para;

	INT32	portstatus;

	INT32	port;
	
	if(get_cmdsize()<2){
		
		device_read(ID_IOPORT, PORT_DI1, &portstatus);
		printf("DI1=%d\n", portstatus);

		device_read(ID_IOPORT, PORT_DI2, &portstatus);
		printf("DI1=%d\n", portstatus);
		
		device_read(ID_IOPORT, PORT_BP121, &portstatus);
		printf("BP121=%d\n", portstatus);

		device_read(ID_IOPORT, PORT_BP122, &portstatus);
		printf("BP122=%d\n", portstatus);

		device_read(ID_IOPORT, PORT_EINT1, &portstatus);
		printf("INT1=%d\n", portstatus);

		device_read(ID_IOPORT, PORT_EINT2, &portstatus);
		printf("INT2=%d\n", portstatus);
		
		device_read(ID_IOPORT, PORT_EINT3, &portstatus);
		printf("INT3=%d\n", portstatus);
		return;
	}
	para = 	cmd_parastrget(1);



//	printf(para);

	sscanf(para, "%d,%d", &(port), &(portstatus));

	if(portstatus ==1){
		printf("Set port %d=1",port);
		device_write(ID_IOPORT, port, 1);	

	}
	else{
		printf("Set port %d=0",port);
		device_write(ID_IOPORT, port, 0);	
	}

	printf("\nPINSEL4 = %x\n",PINSEL4);
	printf("\nPINSEL10 = %x\n",PINSEL10);
	
	return;
}

 


