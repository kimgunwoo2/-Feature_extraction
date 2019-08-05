#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char

int ismax(int x, int y) {
	if (x > y)
		return x;
	else {
		return y;
	}
}

void bmpBWrw()
{
	int histogram[256] = { 0, };
	FILE *infile;
	int sum_histogram[256] = { 0, };
	if ((infile = fopen("Lena.bmp", "rb")) == NULL) {
		printf("No Image File g");
		return;
	}

	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);


	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	if (hInfo.biBitCount != 8) { printf("Bad File Format!!"); return; }
	RGBQUAD hRGB[256];


	fread(hRGB, sizeof(RGBQUAD), 256, infile);
	BYTE *lpImg = malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	BYTE *outImg = malloc(hInfo.biSizeImage);

	int max;
	float tmp;
	for (int i = 0; i < hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			outImg[i*rwsize + j] = 0;
		}
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			tmp = 0.;
			max = 0;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					max = ismax((int)lpImg[(i + k)*rwsize + (j + l)], max);
				}
			}
			outImg[i*rwsize + j] = max - lpImg[i*rwsize + j];
			

		}

	

	for (int i = 0; i < hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			histogram[outImg[i*rwsize + j]]++;
		}
	int sum = 0;
	int x;
	for (int i = 0; i < 256; i++) {
		sum += histogram[255 - i];
		if (sum >(hInfo.biHeight*hInfo.biWidth*0.2)) {
			x = 255 - i;
			break;
		}
	}
	for (int i = 1; i < hInfo.biHeight - 1; i++)
		for (int j = 1; j < hInfo.biWidth - 1; j++) {
			if (x <= outImg[i*rwsize + j])
				outImg[i*rwsize + j] = 0;
			else
				outImg[i*rwsize + j] = 255;
		}
	

	FILE *outfile = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(outImg, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);
	free(lpImg);
}

void main()
{

	bmpBWrw();
}