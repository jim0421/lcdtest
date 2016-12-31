#include "utility.h"

/* get the length of char* */
int getStringLength(char *value)
{
	int len = 0;
	char c = 32;
	while (c >= 32 && c <= 126)
		c = value[len++];
	return len - 1;
}

int abs(int value)
{
	if (value >= 0)
		return value;
	else
		return -value;
}


