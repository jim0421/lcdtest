#ifndef _SYS_H_
#define _SYS_H_

#ifdef __cplusplus 
extern "C" {
#endif

//#define STDIO_TX	LPCUART0_TX
//#define STDIO_RX	LPCUART0_RX

#define STDIO_DEV 	ID_UART0

//ϵͳ����
#define 		LPCUART0_TX			0
#define 		LPCUART0_RX			1
#define 		LPCUART1_TX			2
#define 		LPCUART1_RX			3
#define 		SYS_TICK 			4



//����ϵͳ����
typedef int	(* sys_call_routine)	(void* parameter);
//ϵͳ�������
INT32 	sys_call		(int command, void *p);
//����ϵͳ����
//INT32 add_syscall(sys_call_routine sys_routine);

void 	sys_delay		(INT32 ticks);
void 	bsp_delayms		(INT32 delay);
void 	sys_tick_call	(void);	//�ú���Ӧ�ñ��������ظ�����

INT32 	add_tickroutine	(INT32 tick_interval, void(*routine)(void));

void 	delay			(UINT32 ticks);

//int 	getline 		(char *line, int n);



//���ʹ�����
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



//�����п��ƺ���������



typedef struct usrcmd_tag{
		char 	*cmdchar;
		void	(*f)(void);	
		char	*description;
}		usrcmd;

extern 	void	set_usrcmd		(const usrcmd * cmd_p, INT32 cmdsize);	//�����û������
extern 	void 	help			(void);		//ָʾ�������
extern 	INT32 	command			(void);		//���ڵ��ã������û�����
extern 	UINT32 	cmd_paraget		(int index);//ȡ�������в���
extern 	UINT32 	get_cmdsize		(void);		//ȡ����������
extern	INT8* 	cmd_parastrget	(int index);
extern 	UINT32 cmd_paragethex	(int index);



//��������
INT32	store_dat(INT32 device_id, INT32 len, void* data_in);
//�ָ�����
INT32	restore_dat(INT32 device_id, INT32 maxlen, void* data_out);
#ifdef __cplusplus 
}
#endif
#endif /*_SYS_H_*/
