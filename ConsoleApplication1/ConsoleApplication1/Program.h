#pragma once

#include "FreeImage.h" 

class Program
{
public:
	Program();
	Program(Program *p);
	~Program();
	void run();
	void grayScale(FIBITMAP * dib);
};

