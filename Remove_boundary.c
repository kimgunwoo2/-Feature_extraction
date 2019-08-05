#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char

int ismin(int x, int y) {
	if (x > y)
		return y;
	else {
		return x;
	}
}

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

	float *outImg = malloc(hInfo.biSizeImage * sizeof(float));
	float *outImg1 = malloc(hInfo.biSizeImage * sizeof(float));
	BYTE *lpImg1 = malloc(hInfo.biSizeImage);

	int min, max;
	int temp;
	int sum1 = 0;
	for (int i = 0; i < hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			outImg[i*rwsize + j] = 0;
			sum1 += lpImg[i*rwsize + j];
		}
	sum1 = sum1 / (hInfo.biHeight*hInfo.biWidth);

	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			max = 0;
			min = 255;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					min = ismin((int)lpImg[(i + k)*rwsize + (j + l)], min);
					max = ismax((int)lpImg[(i + k)*rwsize + (j + l)], max);
				}
			}
			temp = max + min - 2 * lpImg[i*rwsize + j];

			outImg[i*rwsize + j] = (float)temp;


		}
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {


			if (outImg[i*rwsize + j] < 0 && outImg[i*rwsize + (j + 1)]>0)
				lpImg1[i*rwsize + j] = 0;
			else
				lpImg1[i*rwsize + j] = 255;
		}
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {

			if (outImg[i*rwsize + j] < 0 && outImg[(i + 1)*rwsize + (j)]>0)
				lpImg1[i*rwsize + j] = 0;

		}
	int total;
	int avg;
	float sum = 0;
	temp = 0;
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			total = 0;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					total += lpImg[(i + k)*rwsize + (j + l)];
				}
			}
			avg = (int)(total / 25);
			outImg[i*rwsize + j] = (float)avg;
		}
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			total = 0;
			temp = 0;
			for (int k = -2; k <= 2; k++) {
				for (int l = -2; l <= 2; l++)
				{
					total = lpImg[(i + k)*rwsize + (j + l)] - (int)outImg[i*rwsize + j];
					temp += (int)pow(total, 2);

				}
			}

			outImg1[i*rwsize + j] = (float)temp/24;
			sum += outImg1[i*rwsize + j];
		}

	sum = sum / ((hInfo.biHeight - 4)*(hInfo.biWidth - 4));
	printf("%f\n", sum);
	for (int i = 2; i < hInfo.biHeight - 2; i++)
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			if (outImg1[i*rwsize + j] < sum) {
				lpImg1[i*rwsize + j] = 255;

			}


		}
	FILE *outfile = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(lpImg1, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);
	free(lpImg);
}

void main()
{

	bmpBWrw();
}
