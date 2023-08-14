#include "checkerCpp.h"


checkerCpp::checkerCpp(void)
{
	wrapper=new Checker("../checker.ini");
}


checkerCpp::~checkerCpp(void)
{
}

void checkerCpp::MoveOneFrame(unsigned char* CH)
{
	wrapper->MoveOneFrame(CH);
}