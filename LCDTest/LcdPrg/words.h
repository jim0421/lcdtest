#ifndef _WORDS_H
#define _WORDS_H
#define WORDLEN 24

// ------------------  ������ģ�����ݽṹ���� ------------------------ //
typedef struct typFNT_GB12                // ������ģ���ݽṹ
{
       char idx[4];             // ������������
       char Msk[24];                      // ����������
}FNT_GB12;

// ��ʾ����
extern void dtb_r(int d_num, int b_num[], int idx);
extern void putWord(int x, int y, int num);
extern void putNumber(int x, int y, char *val);
#endif
