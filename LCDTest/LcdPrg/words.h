#ifndef _WORDS_H
#define _WORDS_H
#define WORDLEN 24

// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB12                // 汉字字模数据结构
{
       char idx[4];             // 汉字内码索引
       char Msk[24];                      // 点阵码数据
}FNT_GB12;

// 显示函数
extern void dtb_r(int d_num, int b_num[], int idx);
extern void putWord(int x, int y, int num);
extern void putNumber(int x, int y, char *val);
#endif
