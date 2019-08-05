#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char


void bmpBWrw()
{
	int histogram[256] = { 0, };
	FILE *infile;
	int sum_histogram[256] = { 0, };
	if ((infile = fopen("Ctest.bmp", "rb")) == NULL) {
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
	float *outImgx = malloc(hInfo.biSizeImage * sizeof(float));
	float *outImgy = malloc(hInfo.biSizeImage * sizeof(float));
	float *outImgxy = malloc(hInfo.biSizeImage * sizeof(float));
	float *outImgx1 = malloc(hInfo.biSizeImage * sizeof(float));
	float *outImgy1 = malloc(hInfo.biSizeImage * sizeof(float));
	float *outImgxy1 = malloc(hInfo.biSizeImage * sizeof(float));
	float *result = malloc(hInfo.biSizeImage * sizeof(float));
	float *result2 = malloc(hInfo.biSizeImage * sizeof(float));
	float windowsx[3][3] = { -1,0,1,-2,0,2,-1,0,1 };
	float windowsy[3][3] = { -1,-2,-1,0,0,0,1,2,1 };

	float tmp;
	memset(outImgx, 0,sizeof(outImgx));
	memset(outImgy, 0, sizeof(outImgx));
	memset(outImgxy, 0, sizeof(outImgx));
	for (int i = 1; i < hInfo.biHeight - 1; i++)
		for (int j = 1; j < hInfo.biWidth - 1; j++) {
			tmp = 0.;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++)
				{
					tmp += windowsx[k + 1][l + 1] * lpImg[(i + k)*rwsize + (j + l)];
				}
			}
			outImgx[i*rwsize + j] = tmp;
			
		}

	for (int i = 1; i < hInfo.biHeight - 1; i++)
		for (int j = 1; j < hInfo.biWidth - 1; j++) {
			tmp = 0.;
			for (int k = -1; k <= 1; k++) {
				for (int l = -1; l <= 1; l++)
				{
					tmp += windowsy[k + 1][l + 1] * lpImg[(i + k)*rwsize + (j + l)];
				}
			}
			outImgy[i*rwsize + j] = tmp;

		}
	for (int i = 1; i < hInfo.biHeight - 1; i++)
		for (int j = 1; j < hInfo.biWidth - 1; j++) {
			
			outImgxy[i*rwsize + j] = (float)(abs((int)outImgx[i*rwsize + j])*abs((int)outImgy[i*rwsize + j]));
			result2[i*rwsize + j] = abs(outImgx[i*rwsize + j]) + abs(outImgy[i*rwsize + j]);
			outImgx[i*rwsize + j] = (float)pow(outImgx[i*rwsize + j], 2);
			outImgy[i*rwsize + j] = (float)pow(outImgy[i*rwsize + j], 2);
		}
	
	float tmp1 = 0;
	float tmp2 = 0;
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			tmp = 0.;
			tmp1 = 0.;
			tmp2= 0.;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					
				
					tmp +=  outImgxy[(i + k)*rwsize + (j + l)]/25;
					tmp1 += outImgx[(i + k)*rwsize + (j + l)]/25;
					tmp2 +=  outImgy[(i + k)*rwsize + (j + l)]/25;
					
					
				}
			}
			outImgxy1[i*rwsize + j] = tmp;
			outImgx1[i*rwsize + j] = tmp1;
			outImgy1[i*rwsize + j] = tmp2;
			
		}
	
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {

			result[i*rwsize + j] = outImgx1[i*rwsize + j] * outImgy1[i*rwsize + j] - (float)pow(outImgxy1[i*rwsize + j], 2) - (float)(0.05*pow(outImgx1[i*rwsize + j] + outImgy1[i*rwsize + j],2));
			
		}
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			if (result[i*rwsize+j]>0.01) {
				outImg[i*rwsize + j] = 0;
				//printf("d");
			}
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