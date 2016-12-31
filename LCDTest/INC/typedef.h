
#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#ifdef __cplusplus 
extern "C" {
#endif

/*基础类型声明*/
typedef unsigned int	UINT32;
typedef unsigned short	UINT16;
typedef int				INT32;
typedef short			INT16;

typedef unsigned char 	UINT8;
typedef char			INT8;

typedef unsigned char 	BYTE;


//typedef unsigned long	U32; 
typedef unsigned short	WORD; 

typedef unsigned long 	DWORD;

typedef const unsigned char FLASH;

//逻辑标准
#define TRUE 	1
#define FALSE	0

#define	OK		1
#define ERROR	0


#define my_abs(val)	((val) > 0 ? (val) : -(val))
#define my_min(val1,val2) (((val1)>(val2)) ? (val2):(val1))
#define my_max(val1,val2) (((val1)>(val2)) ? (val1):(val2))

#define true  1
#define false 0
#ifdef __cplusplus 
}
#endif

#endif /*_TYPEDEF_H_*/
