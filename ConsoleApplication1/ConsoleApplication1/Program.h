#pragma once

#include "FreeImage.h" 
#include <string>
#include <fstream>
#include <iostream>


class Program
{
private:
	//Directory where the changed pictures will be saved
	const std::string dir = "ChangedPics/"; 
	//Where the file will be saved
	std::string fileName = ""; 
	//The format of the image
	FREE_IMAGE_FORMAT format;
	//Total pixels in the image
	int totalPixels;
public:
	Program();
	Program(Program *p);
	~Program();
	void run();
	//Method to make a red, a green and a blue picture
	void seperateColors(FIBITMAP * dib);
	//Method to make the image gray
	void grayScale(FIBITMAP * dib);
	//Method to manipulate the colors in a certain way
	void manipulateColors(FIBITMAP * dib, float rood, float groen, float blauw);
	//Method to create a normalized Histogram with a certain size
	double * normalizeHistogram(FIBITMAP * dib, std::string s, int size);
	//Method to equalize a picture according to a normalized histogram
	void equalize(FIBITMAP * dib, double * histo, int size, int total);
	//Method to add a certain percentage of salt and pepper noise
	void saltAndPepper(FIBITMAP * dib, int percentage);
	//Method to apply a mean filter to clear up noise
	void meanFilter(FIBITMAP * dib, int medianSize);
	//Method to apply a median filter on an image
	//the filterType determines which spot in the sortedarray of pixels will be chosen as the color
	void medianFilter(FIBITMAP * dib, int medianSize, int filterType, bool gray);
	//Method to sort an array of ints
	int * sortArray(int * colorArray, int medianSize);

	//The following two methods are plucked from the internet to see if quick sort would increase the speed of the median filter
	int partition(int* input, int p, int r);
	void quicksort(int* input, int p, int r);

};

