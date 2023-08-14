//This class converts files from Maya to Wten adn vis versa. 
#pragma once
#include <string>
#include <vector>
#include "StdAfx.h"
#include <sstream>
#include <iostream>
#pragma once
#include <fstream>
#include <map>
#include "LipAnimationGenerator.h" //Time conversion function
using namespace std;

#pragma once
class MayaWtenConverter
{
public:
	MayaWtenConverter(void);
	~MayaWtenConverter(void);
	int convert(string path);
private:
	int wtenToMaya(string path);
	int mayaToWten(string path);
	map<string,int> reverse();

	map<int, string> channelToLabel;
};

