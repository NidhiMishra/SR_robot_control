#pragma once

#include "Comm.h"
#include "checker.h"
using namespace std;
using namespace CheckerCPP;

class checkerCpp
{
public:
	checkerCpp(void);
	~checkerCpp(void);

	Checker *wrapper;

	void MoveOneFrame(unsigned char* CH);
	
};

