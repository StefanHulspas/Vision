#include "stdafx.h"
#include "Program.h"

#define DIR = "ChangedPics/"

using namespace std;

Program::Program()
{
}

Program::Program(Program * p)
{
}

Program::~Program()
{
}

void Program::run()
{
	string fileName = "";
	string fileExt = "";
	cout << "Please enter file name:";
	cin >> fileName;
	int png = fileName.find(".png");
	int jpg = fileName.find(".jpg");
	FREE_IMAGE_FORMAT format;
	if (png != -1) {
		format = FIF_PNG;
	}
	else if (jpg != -1) {
		format = FIF_JPEG;
	}
	else {
		cout << "Unknown format!\n >>ERROR<<";
		return;
	}
	
	FIBITMAP *dib = FreeImage_Load(format, fileName.c_str());
	int totalPixels = FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib);
	/*
	FIBITMAP * greyDib = FreeImage_Clone(dib);
	if (grayScale(greyDib) == 0) {
		string grey = dir + "grey_" + fileName;
		FreeImage_Save(format, greyDib, grey.c_str());

		
		double * histo = normalizeHistogram(greyDib, grey, 256);
		normalizeHistogram(greyDib, grey, 10);

		seperateColors(dib, fileName, format);

		FIBITMAP * equaDib = FreeImage_Clone(greyDib);
		if (equalize(equaDib, histo, 256, totalPixels) == 0) {
			string equa = dir + "equalized_" + fileName;
			FreeImage_Save(format, equaDib, equa.c_str());
		}
		FreeImage_Unload(equaDib);
	}
	FreeImage_Unload(greyDib);
	*/

	//week 2

	FIBITMAP * noiseDip = FreeImage_Clone(dib);
	saltAndPepper(noiseDip, 2);
	string Salt = dir + "noise_" + fileName;
	FreeImage_Save(format, noiseDip, Salt.c_str());

	FIBITMAP * meanDip = FreeImage_Clone(noiseDip);
	meanFilter(meanDip, 5);
	string Median = dir + "median_" + fileName;
	FreeImage_Save(format, meanDip, Median.c_str());

	FreeImage_Unload(dib);
	cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}

void Program::seperateColors(FIBITMAP * dib, string fileName, FREE_IMAGE_FORMAT format) {
	FIBITMAP * R_Dib = FreeImage_Clone(dib);
	if (manipulateColors(R_Dib, 1.0f, 0.0f, 0.0f) == 0) {
		string rood = dir + "R_" + fileName;
		FreeImage_Save(format, R_Dib, rood.c_str());
		normalizeHistogram(R_Dib, rood.c_str(), 10);
	}
	else cout << "Red Failed\n";
	FIBITMAP * G_Dib = FreeImage_Clone(dib);
	if (manipulateColors(G_Dib, 0.0f, 1.0f, 0.0f) == 0) {
		string groen = dir + "G_" + fileName;
		FreeImage_Save(format, G_Dib, groen.c_str());
		normalizeHistogram(G_Dib, groen.c_str(), 10);
	}
	else cout << "Green Failed\n";
	FIBITMAP * B_Dib = FreeImage_Clone(dib);
	if (manipulateColors(B_Dib, 0.0f, 0.0f, 1.0f) == 0) {
		string blauw = dir + "B_" + fileName;
		FreeImage_Save(format, B_Dib, blauw.c_str());
		normalizeHistogram(B_Dib, blauw.c_str(), 10);
	}
	else cout << "Blue Failed\n";

	FreeImage_Unload(R_Dib);
	FreeImage_Unload(G_Dib);
	FreeImage_Unload(B_Dib);
}

int Program::grayScale(FIBITMAP * dib) {
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	BYTE roodTemp;
	BYTE groenTemp;
	unsigned int x, y;
	if (image_type == FIT_BITMAP) {
		int aantalWaardes = 4;
		if (FreeImage_GetBPP(dib) == 24) aantalWaardes--;
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (x = 0; x < width; x++) {
				BYTE roodTemp = (BYTE)pixel[FI_RGBA_RED];
				BYTE groenTemp = (BYTE)pixel[FI_RGBA_GREEN];
				pixel[FI_RGBA_RED] = (BYTE)(roodTemp * 0.30 + groenTemp * 0.59 + pixel[FI_RGBA_BLUE] * 0.11);
				pixel[FI_RGBA_GREEN] = (BYTE)(roodTemp * 0.30 + groenTemp * 0.59 + pixel[FI_RGBA_BLUE] * 0.11);
				pixel[FI_RGBA_BLUE] = (BYTE)(roodTemp * 0.30 + groenTemp * 0.59 + pixel[FI_RGBA_BLUE] * 0.11);
				pixel += aantalWaardes;
			}
			// next line
			bits += pitch;
		}
	}
	else return -1;
	return 0;
}

int Program::manipulateColors(FIBITMAP * dib, float rood, float groen, float blauw) {
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	unsigned int x, y;
	if (image_type == FIT_BITMAP) {
		int aantalWaardes = 4;
		if (FreeImage_GetBPP(dib) == 24) aantalWaardes--;
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (x = 0; x < width; x++) {
				pixel[FI_RGBA_RED] = pixel[FI_RGBA_RED] * rood;
				pixel[FI_RGBA_GREEN] = pixel[FI_RGBA_GREEN] * groen;
				pixel[FI_RGBA_BLUE] = pixel[FI_RGBA_BLUE] * blauw;
				pixel += aantalWaardes;
			}
			// next line
			bits += pitch;
		}
	}
	else return -1;
	return 0;

}

double * Program::normalizeHistogram(FIBITMAP * dib, string s, int size) {
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	int totalPixels = width * height;

	string name = s + "_H" + to_string(size) + ".csv"; 
	ofstream outputFile;
	outputFile.open(name.c_str());
	int * histo = new int[size];
	int i;
	for (i = 0; i < size; i++)
		histo[i] = 0;

	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	unsigned int x, y;
	if ((image_type == FIT_BITMAP)) {
		int aantalWaardes = 4;
		if (FreeImage_GetBPP(dib) == 24) aantalWaardes--;
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (x = 0; x < width; x++) {
				if (size == 256)
					histo[pixel[FI_RGBA_RED]]++;
				else if (size == 10)
					histo[(int)(pixel[FI_RGBA_RED] * 10 /256)]++;
				pixel += aantalWaardes;
			}
			// next line
			bits += pitch;
		}
	}
	double * outputHisto = new double[size];
	for (i = 0; i < size; i++) {
		outputHisto[i] = ((float)histo[i] / (float)totalPixels);
		outputFile << '[' << (i + 1) << "], " << ((float)histo[i] / (float)totalPixels) << '\n';
	}
	outputFile.close();
	return outputHisto;
}

int Program::equalize(FIBITMAP * dib, double * histo, int size, int total) {
	if (size != 256) return -1;
	unsigned int i;
	double * histoModified = new double[size];
	histoModified[0] = histo[0];
	double intensiteit = size / total;
	for (i = 1; i < size; i++) {
		histoModified[i] = histoModified[i - 1] + histo[i];
	}
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	unsigned int x, y;
	if ((image_type == FIT_BITMAP)) {
		int aantalWaardes = 4;
		if (FreeImage_GetBPP(dib) == 24) aantalWaardes--;
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (x = 0; x < width; x++) {
				pixel[FI_RGBA_RED] = histoModified[(BYTE)pixel[FI_RGBA_RED]] * size;
				pixel[FI_RGBA_GREEN] = histoModified[(BYTE)pixel[FI_RGBA_GREEN]] * size;
				pixel[FI_RGBA_BLUE] = histoModified[(BYTE)pixel[FI_RGBA_BLUE]] * size;
				pixel += aantalWaardes;
			}
			// next line
			bits += pitch;
		}
	}
	return 0;
}

void Program::saltAndPepper(FIBITMAP * dib, int percentage){
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	BYTE *bits = (BYTE*)FreeImage_GetBits(dib);

	int random = 0;
	unsigned int x, y;
	if ((image_type == FIT_BITMAP)) {
		int aantalWaardes = 4;
		if (FreeImage_GetBPP(dib) == 24) aantalWaardes--;
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (x = 0; x < width; x++) {
				random = rand() % 100;
				if (random <= percentage){
					if (random % 2 == 0){
						pixel[FI_RGBA_RED] = 0;
						pixel[FI_RGBA_GREEN] = 0;
						pixel[FI_RGBA_BLUE] = 0;
					}
					else {
						pixel[FI_RGBA_RED] = 255;
						pixel[FI_RGBA_GREEN] = 255;
						pixel[FI_RGBA_BLUE] = 255;
					}
				}
				pixel += aantalWaardes;
			}
			// next line
			bits += pitch;
		}
	}
}

void Program::meanFilter(FIBITMAP * dib, int medianSize){
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	int* convolutieRed = new int[medianSize * medianSize];
	int* convolutieBlue = new int[medianSize * medianSize];
	int* convolutieGreen = new int[medianSize * medianSize];

	FIBITMAP * original = FreeImage_Clone(dib);

	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	unsigned int x, y, i;
	if ((image_type == FIT_BITMAP)) {
		int aantalWaardes = 4;
		if (FreeImage_GetBPP(dib) == 24) aantalWaardes--;
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		BYTE *originalBits = (BYTE*)FreeImage_GetBits(original);
		
		for (y = 0; y < height - (medianSize - 1); y++) {
			BYTE *pixel = (BYTE*)bits;
			BYTE *originalPixel = (BYTE*)originalBits;
			int collum = 0;
			int plaats = medianSize * (medianSize - 1);
			for (x = 0; x < (medianSize * (medianSize - 1)); x++) {
				if (x % medianSize == 0 && x != 0) collum++;
				int pixelPlaats = collum * aantalWaardes + (x % medianSize) * width * aantalWaardes;
				convolutieRed[x] = originalPixel[pixelPlaats + FI_RGBA_RED];
				convolutieGreen[x] = originalPixel[pixelPlaats + FI_RGBA_GREEN];
				convolutieBlue[x] = originalPixel[pixelPlaats + FI_RGBA_BLUE];
				convolutieRed[x] = convolutieRed[x];
				convolutieGreen[x] = convolutieGreen[x];
				convolutieBlue[x] = convolutieBlue[x];
			}
			for (x = 0; x < width - (medianSize - 1); x++){
				for (i = 0; i < medianSize; i++) {
					int pixelPlaats = (x + medianSize - 1) * aantalWaardes + i * width * aantalWaardes;
					convolutieRed[plaats + i] = originalPixel[pixelPlaats + FI_RGBA_RED];
					convolutieGreen[plaats + i] = originalPixel[pixelPlaats + FI_RGBA_GREEN];
					convolutieBlue[plaats + i] = originalPixel[pixelPlaats + FI_RGBA_BLUE];
				}
				plaats += medianSize;
				plaats %= medianSize * medianSize;
				int totalred = 0, totalgreen = 0, totalblue = 0;
				for (i = 0; i < medianSize * medianSize; i++){
					totalred += convolutieRed[i];
					totalgreen += convolutieGreen[i];
					totalblue += convolutieBlue[i];
				}
				int pixelPlaats = ((x + (medianSize - 1) / 2) * aantalWaardes + (((medianSize - 1) / 2) * width * aantalWaardes));
				pixel[pixelPlaats + FI_RGBA_RED] = totalred / (medianSize * medianSize);
				pixel[pixelPlaats + FI_RGBA_GREEN] = totalgreen / (medianSize * medianSize);
				pixel[pixelPlaats + FI_RGBA_BLUE] = totalblue / (medianSize * medianSize);
			}
			bits += pitch;
			originalBits += pitch;
		}
	}
}