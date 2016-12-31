#ifndef _SYS_H_
#define _SYS_H_

#ifdef __cplusplus 
extern "C" {
#endif

//#define STDIO_TX	LPCUART0_TX
//#define STDIO_RX	LPCUART0_RX

#define STDIO_DEV 	ID_UART0

//系统调用
#define 		LPCUART0_TX			0
#define 		LPCUART0_RX			1
#define 		LPCUART1_TX			2
#define 		LPCUART1_RX			3
#define 		SYS_TICK 			4



//定义系统调用
typedef int	(* sys_call_routine)	(void* parameter);
//系统调用入口
INT32 	sys_call		(int command, void *p);
//增加系统调用
//INT32 add_syscall(sys_call_routine sys_routine);

void 	sys_delay		(INT32 ticks);
void 	bsp_delayms		(INT32 delay);
void 	sys_tick_call	(void);	//该函数应该被主程序重复调用

INT32 	add_tickroutine	(INT32 tick_interval, void(*routine)(void));

void 	delay			(UINT32 ticks);

//int 	getline 		(char *line, int n);



//发送错误码
void 	send_error		(INT32 ErrCode);

/*
vt100 functions
*/
void 	vt100Init		(void);
void 	vt100ClearScreen(void);
void 	vt100SetAttr(UINT8 attr);
void 	vt100SetCursorMode(UINT8 visible);
void 	vt100SaveCursorAttr(void);
void 	vt100RestoreCursorAttr(void);
void 	vt100SetCursorPos(UINT8 line, UINT8 col);
void 	vt100StrollDown	(void);
void 	vt100StrollUp	(void);
void 	vt100StrollScreen(UINT8 start, UINT8 end);



//命令行控制函数和申明



typedef struct usrcmd_tag{
		char 	*cmdchar;
		void	(*f)(void);	
		char	*description;
}		usrcmd;

extern 	void	set_usrcmd		(const usrcmd * cmd_p, INT32 cmdsize);	//设置用户命令函数
extern 	void 	help			(void);		//指示命令帮助
extern 	INT32 	command			(void);		//周期调用，处理用户命令
extern 	UINT32 	cmd_paraget		(int index);//取得命令行参数
extern 	UINT32 	get_cmdsize		(void);		//取得命令数量
extern	INT8* 	cmd_parastrget	(int index);
extern 	UINT32 cmd_paragethex	(int index);



//保存数据
INT32	store_dat(INT32 device_id, INT32 len, void* data_in);
//恢复数据
INT32	restore_dat(INT32 device_id, INT32 maxlen, void* data_out);
#ifdef __cplusplus 
}
#endif
#endif /*_SYS_H_*/
