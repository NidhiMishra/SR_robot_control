#pragma once
#include <vector>
#include <cmath>
#include "structDef.h"
#include <map>
#include <string>


class LipAnimationGenerator
{
public:
	LipAnimationGenerator(void);
	~LipAnimationGenerator(void);

	void addPhoneme(anim_data data);//Add a phoneme for animation
	void generateAnimation();//Produce an animation file based on the current list of phonemes
	void generateAnimationBackUp();
	std::string timeConversion(int i); //convert the frame counter into a String representing time for the animation

	std::vector<unsigned char> *channel5;
	std::vector<unsigned char> *channel6;
	std::vector<unsigned char> *channel7;
	lip_position lipAnimation[3000];
	int frameCounter;
	bool German_mode;
	

private:
	double interpolate(double a, double b, double x);
	void groupShortPhonemes(double minDuration);
	lip_position phoneToLip(const char * aPhone);
	

	std::vector <anim_data> phoneList;
	std::map<std::string, std::string> phonemeToViseme;
	std::map<std::string, lip_position> visemeToLips;

	

};

