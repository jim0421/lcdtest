#include <rtl.h>
#include "bsp.h"
#include <stdarg.h>
#include <stdio.h>
#include "sys.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Debug manage
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



typedef struct DbglvlName_tag {
	char* name;
	INT16	id;
}DbglvlName;

class DBG_PLVL {
	
public:
	DBG_PLVL(){}
	~DBG_PLVL(){}
public:
	static const INT16	lvlsize = 32;
	static INT16	lvl[lvlsize];
	static DbglvlName dbgnamep[]; 
	
	static void 	view_lvl(void);					//级别输出
	static INT16	set_lvl(char * name, INT16 lvl);//设置跟踪级别
	static INT16	get_lvl(char * name);			//获得跟踪级别

	static void 	closeall(void);
		
};

INT16	DBG_PLVL::lvl[lvlsize] = {
	1, 		/*DBINFO_LVL = 0,*/
	0,		/*DB_LVL,*/
	0,		/*DB_485,*/
	0,		/*DBIO_LVL,*/
	1,		/*DBR_LVL,*/
	0,		/*DBTIME_LVL,*/
	1,		/*DB_KEY,*/
	0,		/*DB_ERR*/
};

DbglvlName DBG_PLVL::dbgnamep[]={
  	{"INFO", 	DBINFO_LVL},
	{"DB", 	 	DB_LVL},
	{"485",		DB_485},
	{"IO",		DBIO_LVL},
	{"DBR",		DBR_LVL},
	{"TIME",	DBTIME_LVL},
	{"KEY",		DB_KEY},
	{"ERR",		DB_ERR},
};

/*For error printf*/
//static INT16	debuglevel = 5;

void DebugPrint (INT16	level_id, char * format, ...)
{
	char buffer[256];
	
	va_list args;
	if(DBG_PLVL::lvl[level_id] == 0)
	return;
	va_start (args, format);
	vsprintf (buffer,format, args);
	printf (buffer);
	va_end (args);
}


void 	DBG_PLVL::view_lvl(void){					//级别输出
	INT16	i;
	printf("%8s ,%3s %3s\n", "name","id","lvl");
	for(i=0; i<sizeof(dbgnamep)/sizeof(DbglvlName); i++){
		printf("%8s ,%3d %3d\n", dbgnamep[i].name, dbgnamep[i].id, lvl[(dbgnamep[i].id)]);
	}
}


INT16	DBG_PLVL::set_lvl(char * name, INT16 lvlset){//设置跟踪级别
	INT16	i;

	for(i=0; i<sizeof(dbgnamep)/sizeof(DbglvlName); i++){
		if(strcmp(name, dbgnamep[i].name) == 0){
			lvl[(dbgnamep[i].id)] = lvlset;				   //查找成功，并设置
			return TRUE;
		}
	}
	return FALSE;

}

void	DBG_PLVL::closeall(void){					//关闭所有级别
	INT16	i;

	for(i=1; i<lvlsize; i++){
		lvl[i] = 0;
	}
	return;
}

INT16	DBG_PLVL::get_lvl(char * name){			//获得跟踪级别
	INT16	i;

	for(i=0; i<sizeof(dbgnamep)/sizeof(DbglvlName); i++){
		if(strcmp(name, dbgnamep[i].name) == 0)
			return dbgnamep[i].id; 
	}
	return 0;
}

extern "C" void	deblvl_cmd(void){

	INT8	*para;
	INT32	dat;
	if(get_cmdsize()==1){
		DBG_PLVL::view_lvl();
		return;
	}else if(get_cmdsize()==2){
		para = 	cmd_parastrget(1);
		if(para[0] == 'C'){
			printf("Close all.\n");
			DBG_PLVL::closeall();
		}		

	}else if(get_cmdsize()==3){
		
		para = 	cmd_parastrget(1);
	
		dat = cmd_paraget(2);
	
		if(DBG_PLVL::set_lvl(para, dat)==FALSE){
		 	printf("PARA Setting error.\n");
		}
		else
		   	printf("PARA Setting OK.\n");
	}

}

