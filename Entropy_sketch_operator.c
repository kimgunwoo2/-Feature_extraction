#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char


float ismax(float x, float y) {
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
	int *entropy2 = (int*)malloc(hInfo.biSizeImage * sizeof(int));
	float *entropy = (float*)malloc(hInfo.biSizeImage * sizeof(float));
	float *entropy1 = (float*)malloc(hInfo.biSizeImage * sizeof(float));
	for (int i = 0; i < hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			outImg[i*rwsize + j] = 0;
			entropy[i*rwsize + j] = 0;
		}
	int sum;

	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			sum = 0;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					sum += lpImg[(i + k)*rwsize + (j + l)];

				}
			}

			entropy[i*rwsize + j] = (lpImg[i*rwsize + j] / (float)sum); //확률 구하고
			entropy[i*rwsize + j] = (float)(-log2(entropy[i*rwsize + j])); //불확실성 구하기
			entropy[i*rwsize + j] = entropy[i*rwsize + j] * (lpImg[i*rwsize + j] / (float)sum);
			
		}

	float temp;
	float temp1 = 0;
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {


		}
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			temp = 0;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					temp += entropy[(i + k)*rwsize + (j + l)];

				}
			}
			entropy1[i*rwsize + j] = temp;
			temp1 += temp;
		}
	float max = 0;
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			max = ismax(entropy1[(i)*rwsize + (j)], max);
		}
	printf("%f", max);
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			entropy1[i*rwsize + j] = entropy1[i*rwsize + j] / max * 255;
			entropy2[i*rwsize + j] = (int)(entropy1[i*rwsize + j] + 0.5);
		}


	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			histogram[entropy2[i*rwsize + j]]++;
		}
	sum = 0;
	int x;
	for (int i = 0; i < 256; i++) {
		sum += histogram[i];
		if (sum >(hInfo.biHeight*hInfo.biWidth*0.2)) {
			x = i;
			break;
		}
	}
	for (int i = 1; i < hInfo.biHeight - 1; i++)
		for (int j = 1; j < hInfo.biWidth - 1; j++) {
			if (x >= entropy2[i*rwsize + j])
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
