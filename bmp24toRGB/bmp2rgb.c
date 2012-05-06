#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bmp2rgb.h"

void HeadStand(void* buffer, int width, int height)
{
	int				i, j, dist, framesize=width*height;
	imgpel		*rgbBuf = (imgpel*)buffer;
	imgpel		*buftmp;

	buftmp	= (imgpel*)calloc(width*3, sizeof(imgpel));
	if (!buftmp)
	{
		printf("temporary buffer allocation failed.\n");
		exit(-1);
	}

	dist	= width*3*sizeof(imgpel);
	for (i=0; i<height/2; i++)
	{
		memcpy(buftmp, rgbBuf+i*dist, dist);
		memcpy(rgbBuf+i*dist, rgbBuf+(height-1)*dist-i*dist, dist);
		memcpy(rgbBuf+(height-1)*dist-i*dist, buftmp, dist);
	}

	free(buftmp);
}

int bmp2rgb(char *infile)
{
	int					i, len;
	int					width, height, framesize;
	FILE				*ip, *op;
	imgpel			*irgbBuf, *orgbBuf;
	imgpel			*buf_r, *buf_g, *buf_b;
	char				*outfile, *suffix;

	BITMAPFILEHEADER			*bitmap;
	BITMAPINFOHEADER			*bitmapInfo;

	const char	*SFX = ".rgb";
	const char	*MT	 = "x";
	const	int		MAXLEN  = 256;


	if (!(ip=fopen(infile, "rb")))
	{
		printf("open infile failed.\n");
		exit(-1);
	}


	bitmap			= (BITMAPFILEHEADER*)calloc(sizeof(BITMAPFILEHEADER), 1);
	bitmapInfo	= (BITMAPINFOHEADER*)calloc(sizeof(BITMAPINFOHEADER), 1);
	if ((!bitmap)||(!bitmapInfo))
	{
		printf("memory allocation failed.\n");
		exit(-1);
	}

	if (fread(bitmap, sizeof(char), sizeof(BITMAPFILEHEADER), ip)!=sizeof(BITMAPFILEHEADER))
	{
		printf("read bitmapfileheader failed.\n");
		exit(-1);
	}
	if (fread(bitmapInfo, sizeof(char), sizeof(BITMAPINFOHEADER), ip)!=sizeof(BITMAPINFOHEADER))
	{
		printf("read bitmapinfoheader failed.\n");
		exit(-1);
	}

	width			= bitmapInfo->biWidth;
	height		= bitmapInfo->biHeight;
	framesize	= width*height;

	outfile		= (char*)calloc(MAXLEN, sizeof(char));
	suffix		= (char*)calloc(25, sizeof(char));
	assert((outfile!=NULL)&&(suffix!=NULL));
	len				= strlen(infile);
	memcpy(outfile, infile, len-4);
	suffix		= itoa(width, suffix, 10); 
	strcat(outfile, suffix);
	strcat(outfile, MT);
	memset(suffix, 0, sizeof(suffix));
	suffix		= itoa(height, suffix, 10);
	strcat(outfile, suffix);
	strcat(outfile, SFX);


	if (!(op=fopen(outfile, "wb")))
	{
		printf("open outfile failed.\n");
		exit(-1);
	}

	irgbBuf		= (imgpel*)calloc(framesize*3, sizeof(imgpel));
	orgbBuf		= (imgpel*)calloc(framesize*3, sizeof(imgpel));
	if ((!irgbBuf)||(!orgbBuf))
	{
		printf("rgb buffer allocation failed.\n");
		exit(-1);
	}
	fseek(ip, bitmap->bfOffBits, SEEK_SET);
	if (len = fread(irgbBuf, sizeof(char), framesize*3, ip)!=framesize*3)
	{
		
		printf("write to rgbBuf faled.\n");
		exit(-1);
	}
	buf_r = orgbBuf;
	buf_g = buf_r + framesize;
	buf_b = buf_g + framesize;

	HeadStand(irgbBuf, width, height);

	for (i=0; i<framesize; i++)
	{
		buf_b[i]	= irgbBuf[i*3];
		buf_g[i]	= irgbBuf[i*3+1];
		buf_r[i]	= irgbBuf[i*3+2];
	}

	if (fwrite(orgbBuf, sizeof(imgpel), framesize*3, op)!=framesize*3)
	{
		printf("write to rgb file failed.\n");
		exit(-1);
	}


	fclose(ip);
	fclose(op);
	free(bitmap);
	free(bitmapInfo);
	free(irgbBuf);
	free(orgbBuf);

	return 1;
}

int main(int argc, char** argv)
{
	if (argc<2)
	{
		printf("[inputfile name]\n");
		return -1;
	}

	if (bmp2rgb(argv[1])==1)
		printf("convertion done.\n");

	return 0;
}
