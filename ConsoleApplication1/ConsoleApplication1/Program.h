#pragma once

#include "FreeImage.h" 
#include <string>
#include <fstream>
#include <iostream>


class Program
{
private:
	const std::string dir = "ChangedPics/";
public:
	Program();
	Program(Program *p);
	~Program();
	void run();
	void seperateColors(FIBITMAP * dib, std::string fileName, FREE_IMAGE_FORMAT format);
	int grayScale(FIBITMAP * dib);
	int manipulateColors(FIBITMAP * dib, float rood, float groen, float blauw);
	double * normalizeHistogram(FIBITMAP * dib, std::string s, int size);
	int equalize(FIBITMAP * dib, double * histo, int size, int total);
	void saltAndPepper(FIBITMAP * dib, int percentage);
};

