#include <stdio.h>
#include <ctype.h>                       /* character functions               */
#include <LPC23xx.h>


#include "bsp.h"
#include "sys.h"
#include <string.h>
#include "drv.h"

/*Line input status.*/
#define 	LINE_END 		1	/*Line input is over*/
#define 	LINE_ESC 		2	/*Line input is cancel*/
#define 	LINE_PENDING 	0	/*Line input is going on*/


/*Special ASCII*/
#define 	CNTLQ      0x11
#define 	CNTLS      0x13
#define 	DEL        0x7F
#define 	BACKSPACE  0x08
#define 	CR         0x0D
#define 	LF         0x0A
#define 	ESC		   0x1B

#define 	CMDLEN		200
#define 	CMDSIZE_MAX 30

static char cmdbuf [CMDLEN];              	/* command input buffer        */

char 		ERRORMSG [] = "\n*** ERROR: %s\n";        /* ERROR message string in code   */

static struct {
	int		size;
	char*	cmd_p[CMDSIZE_MAX];
}	cmdline_data;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//command function start here .
UINT32 cmd_paraget(int index);
UINT32 get_cmdsize(void);

static 	const usrcmd 	*usrcommand = NULL;
static	int		usrcmdsize = 0;

void	set_usrcmd(const usrcmd * cmd_p, INT32 cmdsize){
	usrcommand = cmd_p;			//设置命令指针
	usrcmdsize = cmdsize; 			//设置命令数量
														   
	return; 
}

//command function end here .

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Command define .
/*
void help(char *);
	Help function. It will print the command legally.
*/

void help(void){
	int i;

	
	printf("System debug command list.\n");
	for(i=0; i<usrcmdsize; i++){
		if(usrcommand == NULL) break;
		printf("%s--%s\n",usrcommand[i].cmdchar,usrcommand[i].description);	
	}
}

/***************/
/* Line Editor */
/***************/
int getline (char *line, int n)  {

 	static int  cnt = 0;
  	static char c;
	if((device_ioctl(STDIO_DEV, KEY_PRESS, NULL) == FALSE))
		return LINE_PENDING;
  	else {
    	if ((c = getchar ()) == CR)  c = LF;		/* read character                 */
    	if (c == BACKSPACE  ||  c == DEL)  {    /* process backspace              */
	      	if (cnt != 0)  {
		        cnt--;         					/* decrement count                */
		   //     line--;              			/* and line pointer               */
		        putchar (BACKSPACE);  			/* echo backspace                 */
		        putchar (' ');
		        putchar (BACKSPACE);
	      	}
	    }
	    else if(c == ESC){
	    	putchar ('\r');

	    	*line = 0;
			cnt = 0;
	    	return LINE_ESC;
	    }
    	else if (c != CNTLQ && c != CNTLS)  {	/* ignore Control S/Q             */
			putchar (*(line+cnt) = c);				/* echo and store character       */
		//	line++;   							/* increment line pointer         */
			cnt++;        						/* and count                      */
    	}
    	
    	if (cnt < n - 1  &&  c != LF)
    		return LINE_PENDING;					/* check limit and line feed      */
    	else {
    		*(line+cnt - 1) = 0;         			/* mark end of string             */
    		cnt = 0;
    		return LINE_END;
    	}
    		
	}  
  
}

/************************************************************************/

void cmd_test(void){
	int i;
	
	for(i=0; i<cmdline_data.size; i++)	{
		printf("%d - %s\n", i, cmdline_data.cmd_p[i]);
		printf("    %x\n", cmd_paraget(i));
	}
	
	printf("\n");
}

void cmdline_analysis( void );
void runcommand(void);

INT32 command(void)
{
	

	
	//Get a command and run it.
	if(getline (&cmdbuf[0], sizeof (cmdbuf))==LINE_END){  /* input command line          */
		
		cmdline_analysis();
		if(strcmp(cmdline_data.cmd_p[0], "X")==0){
			printf("Exit \n");
			return FALSE;
		}
		runcommand();
		printf ("\nCommand: ");
		
		return TRUE;
	}
	return TRUE;
	
}


//define a state machine here.
#define SCH_BLANK 1
#define SCH_CHAR 2


void cmdline_analysis( void ){
	
	int i;
	int	state;
	
	state = SCH_BLANK;
	cmdline_data.size = 0;
	
	
	for(i=0; i< CMDLEN; i++){
		if(cmdbuf[i] == 0) break;
		cmdbuf[i] = toupper(cmdbuf[i]);
		
				
		if(state == SCH_BLANK){
			if(cmdbuf[i] != ' '){
				cmdline_data.cmd_p[cmdline_data.size] = &(cmdbuf[i]);
				cmdline_data.size++;
				if(cmdline_data.size>= CMDSIZE_MAX) break;
				state = SCH_CHAR;
			}
		}
		else if(state == SCH_CHAR){
			if(cmdbuf[i] == ' '){
				cmdbuf[i] = NULL;
				state = SCH_BLANK;
			}
		}
		
	}
	
	
}

UINT32 cmd_paragethex(int index){
	UINT32 dat;
	
	if(index >= cmdline_data.size || (index <1))
		return 0;
	sscanf(cmdline_data.cmd_p[index],"%x", &dat);
	
	return dat;
}

UINT32 cmd_paraget(int index){
	UINT32 dat;
	
	if(index >= cmdline_data.size || (index <1))
		return 0;
	sscanf(cmdline_data.cmd_p[index],"%d", &dat);
	
	return dat;
}

INT8* cmd_parastrget(int index){
	if(index >= cmdline_data.size || (index <1))
		return NULL;
		
	return (cmdline_data.cmd_p[index]);
}


UINT32 get_cmdsize(void){
	return cmdline_data.size;
}


void runcommand()
{
	
	int i;
	
	for(i=0; i<usrcmdsize; i++){
		if(usrcommand == NULL) break;
		if(strcmp(usrcommand[i].cmdchar, cmdline_data.cmd_p[0])==0){
//			printf("%s",usercommand[i].description);	
			(usrcommand[i].f)();
			break;
		}
	}
	if(i==usrcmdsize)
		printf (ERRORMSG, "UNKNOWN COMMAND");
	

}



