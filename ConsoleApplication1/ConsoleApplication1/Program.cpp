#include "stdafx.h"
#include "Program.h"
#include <iostream>
#include "FreeImage.h"
#include <string>


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
	if (png != -1) {
		FIBITMAP *dib = FreeImage_Load(FIF_PNG, fileName.c_str(), PNG_DEFAULT);
		string grey = "grey_" + fileName;
		FIBITMAP * greyDip = FreeImage_Clone(dib);
		grayScale(greyDip);
		cout << "saving...";
		FreeImage_Save(FIF_PNG, greyDip, grey.c_str(), PNG_DEFAULT);
		FreeImage_Unload(dib);
	}
	else if (jpg != -1) {
		FIBITMAP *dib = FreeImage_Load(FIF_JPEG, fileName.c_str(), JPEG_DEFAULT);
		string grey = "grey_" + fileName;
		FIBITMAP * greyDip = FreeImage_Clone(dib);
		grayScale(greyDip);
		cout << "saving...";
		FreeImage_Save(FIF_JPEG, greyDip, grey.c_str(), JPEG_DEFAULT);
		FreeImage_Unload(dib);
	}


	cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}

void Program::grayScale(FIBITMAP * dib) {
	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);
	unsigned int x, y;
	if (image_type == FIT_RGBF) {
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			FIRGBF *pixel = (FIRGBF*)bits;
			for (x = 0; x < width; x++) {
				pixel[x].red = (pixel[x].red + pixel[x].green + pixel[x].blue)/3;
				pixel[x].green = (pixel[x].red + pixel[x].green + pixel[x].blue) / 3;
				pixel[x].blue = (pixel[x].red + pixel[x].green + pixel[x].blue) / 3;
			}
			// next line
			bits += pitch;
		}
	}
	else if ((image_type == FIT_BITMAP) && (FreeImage_GetBPP(dib) == 24)) {
		BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
		for (y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for (x = 0; x < width; x++) {
				pixel[FI_RGBA_RED] = (pixel[FI_RGBA_RED] + pixel[FI_RGBA_GREEN] + pixel[FI_RGBA_BLUE])/3;
				pixel[FI_RGBA_GREEN] = (pixel[FI_RGBA_RED] + pixel[FI_RGBA_GREEN] + pixel[FI_RGBA_BLUE]) / 3;
				pixel[FI_RGBA_BLUE] = (pixel[FI_RGBA_RED] + pixel[FI_RGBA_GREEN] + pixel[FI_RGBA_BLUE]) / 3;
				pixel += 3;
			}
			// next line
			bits += pitch;
		}
	}
}