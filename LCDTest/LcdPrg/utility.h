#ifndef _UTILITY_H
#define _UTILITY_H
/* get value from left to right */
typedef struct D_VALUEBRANCH{
	int col1;
	char *val1;
	int col2;
	char *val2;
	char *val3;
	int col3;
	char *val4;
	int col4;
}Value;

typedef struct D_SWITCHBRANCH{
	bool switch1;
	bool switch2;
};

int getStringLength(char *value);
int abs(int value);
#endif
