#include "LPC23xx.h"			/* LPC23XX/24xx Peripheral Registers */
#include "typedef.h"
#include "irq.h"



/* Initialize the interrupt controller */
/******************************************************************************
** Function name:		init_VIC
**
** Descriptions:		Initialize VIC interrupt controller.
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void init_VIC(void) 
{
    UINT32 i = 0;
    UINT32 *vect_addr, *vect_prio;
   	
    /* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		vect_addr = (UINT32 *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		vect_prio = (UINT32 *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + i*4);
		*vect_addr = 0x0;	
		*vect_prio = 0xF;
    }
    return;
}

/******************************************************************************
** Function name:		install_irq
**
** Descriptions:		Install interrupt handler
** parameters:			Interrupt number, interrupt handler address, 
**						interrupt priority
** Returned value:		true or false, return false if IntNum is out of range
** 
******************************************************************************/
UINT32 install_irq( UINT32 IntNumber, void *HandlerAddr, UINT32 Priority )
{
    UINT32 *vect_addr;
    UINT32 *vect_prio;
      
    VICIntEnClr = 1 << IntNumber;	/* Disable Interrupt */
    if ( IntNumber >= VIC_SIZE )
    {
		return ( FALSE );
    }
    else
    {
		/* find first un-assigned VIC address for the handler */
		vect_addr = (UINT32 *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber*4);
		vect_prio = (UINT32 *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + IntNumber*4);
		*vect_addr = (UINT32)HandlerAddr;	/* set interrupt vector */
		*vect_prio = Priority;
		VICIntEnable = 1 << IntNumber;	/* Enable Interrupt */
		return( TRUE );
    }
}
/******************************************************************************
** Function name:		enable_irq
**
** Descriptions:		Enable interrupt
** parameters:			Interrupt number, 
** Returned value:		none
** 
******************************************************************************/
void	enable_irq(UINT32 IntNumber){
	VICIntEnable = 1 << IntNumber;		/* Enable Interrupt */	
}
/******************************************************************************
** Function name:		disable_irq
**
** Descriptions:		Disable interrupt
** parameters:			Interrupt number
** Returned value:		none
** 
******************************************************************************/
void	disable_irq(UINT32 IntNumber){
	VICIntEnClr = (1<<IntNumber); 		/* disable Interrupt */	
}


/******************************************************************************
**                            End Of File
******************************************************************************/


