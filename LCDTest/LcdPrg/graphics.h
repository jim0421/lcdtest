

#define 	BLACK	0x00
#define		RED		0xe0
#define		GREEN	0x1c
#define		BLUE	0x03
#define		WHITE	0xff
#define 	YELLOW 0xfc

//#define abs(x) (x>0 ? x : (-x)) bug here
#define min(x1,x2) (x1>x2 ? x2 : x1)
#define max(x1,x2) (x1<x2 ? x2 : x1)

extern "C" {
	void putpixel(int x, int y, int color);
	void initgraph(int x, int y);
	void closegraph();
}

