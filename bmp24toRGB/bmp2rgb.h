#ifndef _BMP2RGB_H_
#define _BMP2RGB_H_

typedef unsigned char		BYTE;
typedef unsigned short	HWORD;
typedef unsigned int		WORD;
typedef unsigned long		DWORD;

typedef	long						LONG;
typedef BYTE						imgpel;

//#pragma			pack(2)
#include <pshpack2.h>
typedef struct  
{
	HWORD			bfType;
	DWORD			bfSize;
	HWORD			bfReserved1;
	HWORD			bfReserved2;
	DWORD			bfOffBits;
}BITMAPFILEHEADER;
#include <PopPack.h>

typedef struct  
{
	DWORD			biSize;
	LONG			biWidth;
	LONG			biHeight;
	HWORD			biPlanes;
	HWORD			biBitcount;
	DWORD			biCompression;
	DWORD			biSizeImage;
	LONG			biXPelsPerMeter;
	LONG			biYPelsPerMeter;
	DWORD			biClrUsed;
	DWORD			biClrImportant;
}BITMAPINFOHEADER;


#endif