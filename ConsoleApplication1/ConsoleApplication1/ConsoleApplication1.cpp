// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <limits>
#include "Program.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Program p = new Program();
	p.run();

	std::cout << "Press ENTER to continue... ";
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
	
	return 0;
}

