#pragma once

#include "StdAfx.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "structDef.h"
#include "LipAnimationGenerator.h"
#include <limits>
#include <string>
#include <map>
#include <stdlib.h>

using namespace std;


LipAnimationGenerator::LipAnimationGenerator(void)
{
	float reduceRange=0.9;//all the visemes are * by this value
	frameCounter=0;
	lip_position temp;
	German_mode=false;

	channel5=new vector<unsigned char>();
	channel6=new vector<unsigned char>();
	channel7=new vector<unsigned char>();

	temp.motor1= 0;
	temp.motor2=0;
	temp.motor3=0;
	phonemeToViseme["sil"]="_";//
	visemeToLips["_"]=temp;
	
	phonemeToViseme[ "ao" ]  = "Ao"; // AO
	phonemeToViseme[ "aa" ]  = "Ao"; // AA
	temp.motor1= 114*reduceRange;//Values given by Robert
	temp.motor2=255*reduceRange;
	temp.motor3=0*reduceRange;
	visemeToLips["Ao"]=temp;

	phonemeToViseme[ "m" ]   = "BMP";//  M
	phonemeToViseme[ "p" ]   = "BMP";//  P
	phonemeToViseme[ "b" ]   = "BMP";//  B Robert 101,2,125
	temp.motor1= 100*reduceRange;//Values given by Robert
	temp.motor2=0;
	temp.motor3=125*reduceRange;
	visemeToLips["BMP"]=temp;

	phonemeToViseme[ "w" ]   = "OO"; //  W   NOT IN SMART BODY BUT ADDED BY ROBERT
	phonemeToViseme[ "y" ]   = "OO"; //  Y 
	temp.motor1= 55*reduceRange;//Values given by Robert
	temp.motor2=255*reduceRange;
	temp.motor3=0;
	visemeToLips["OO"]=temp;

	phonemeToViseme[ "ow" ]  = "Oh"; // OW
	phonemeToViseme[ "uw" ]  = "Oh"; // UW
	phonemeToViseme[ "oy" ]  = "Oh"; // OY
	temp.motor1= 0;
	temp.motor2=255*reduceRange;
	temp.motor3=0;
	visemeToLips["oh"]=temp;

	phonemeToViseme[ "hh" ]  = "Ih"; // HH
	phonemeToViseme[ "ih" ]  = "Ih"; // IH
	phonemeToViseme[ "ae" ]  = "Ih"; // AE
	phonemeToViseme[ "ah" ]  = "Ih"; // AH
	phonemeToViseme[ "ax" ]  = "Ih"; // AX
	phonemeToViseme[ "@" ]  = "Ih"; // Shouldn't happen!
	phonemeToViseme[ "aw" ]  = "Ih"; // AW
	phonemeToViseme[ "ay" ]  = "Ih"; // AY
	phonemeToViseme[ "eh" ]  = "Ih"; // EH
	phonemeToViseme[ "ey" ]  = "Ih"; // ?? probably EY
	temp.motor1=205*reduceRange;
	temp.motor2=0;
	temp.motor3=216*reduceRange;
	visemeToLips["Ih"]=temp;
	
	phonemeToViseme[ "T" ]   = "D";  //  T ?
	phonemeToViseme[ "t" ]   = "D";  //  T ?
	phonemeToViseme[ "l" ]   = "D";  //  L
	phonemeToViseme[ "dx" ]  = "D";  // ??
	phonemeToViseme[ "d" ]   = "D";  //  D
	visemeToLips["D"]=temp;
	temp.motor1=70*reduceRange;
	temp.motor2=0;
	temp.motor3=240*reduceRange;

	phonemeToViseme[ "er" ]  = "Er"; // ER
	temp.motor1=140*reduceRange;
	temp.motor2=0;
	temp.motor3=255*reduceRange;
	visemeToLips["Er"]=temp;

	phonemeToViseme[ "v" ]   = "F";  //  V
	phonemeToViseme[ "f" ]   = "F";  //  F
	temp.motor1=20*reduceRange;
	temp.motor2=110*reduceRange;
	temp.motor3=0;
	visemeToLips["F"]=temp;

	phonemeToViseme[ "zh" ]  = "J";  // ZH
	phonemeToViseme[ "sh" ]  = "j";  // SH	
	phonemeToViseme[ "ch" ]  = "j";  // CH
	phonemeToViseme[ "jh" ]  = "j";  // JH
	temp.motor1=75*reduceRange;
	temp.motor2=0;
	temp.motor3=230*reduceRange;
	visemeToLips["j"]=temp;

	phonemeToViseme[ "g" ]   = "KG"; //  G
	phonemeToViseme[ "k" ]   = "KG"; //  K
	temp.motor1=110*reduceRange;
	temp.motor2=0;
	temp.motor3=90*reduceRange;
	visemeToLips["KG"]=temp;

	phonemeToViseme[ "ng" ]  = "NG"; // NG
	phonemeToViseme[ "n" ]   = "NG"; //  N
	temp.motor1=65*reduceRange;
	temp.motor2=0;
	temp.motor3=140*reduceRange;
	visemeToLips["NG"]=temp;

	phonemeToViseme[ "r" ]   = "R";  //  R
	temp.motor1=70*reduceRange;
	temp.motor2=0;
	temp.motor3=120*reduceRange;
	visemeToLips["R"]=temp;

	phonemeToViseme[ "z" ]   = "Z";  //  Z
	phonemeToViseme[ "s" ]   = "Z";  //  S
	temp.motor1=70*reduceRange;
	temp.motor2=0;
	temp.motor3=255*reduceRange;
	visemeToLips["Z"]=temp;

	phonemeToViseme[ "dh" ]  = "Th"; // DH
	phonemeToViseme[ "th" ]  = "Th"; // TH
	temp.motor1=144*reduceRange;
	temp.motor2=0;
	temp.motor3=0;
	visemeToLips["Th"]=temp;

	phonemeToViseme[ "uh" ]  = "Oh"; // UH
	temp.motor1=0;
	temp.motor2=255*reduceRange;
	temp.motor3=0;
	visemeToLips["Oh"]=temp;

	phonemeToViseme[ "iy" ]  = "EE"; // IY
	temp.motor1=125*reduceRange;
	temp.motor2=0;
	temp.motor3=240*reduceRange;
	visemeToLips["EE"]=temp;
	
	//phonemeToViseme[ "i" ]  = "";  // 
	//phonemeToViseme[ "j" ]  = "";  // 
	//possibly need phonemeToViseme entries for "i" and "j"?*/
}



LipAnimationGenerator::~LipAnimationGenerator(void)
{

}


void LipAnimationGenerator::addPhoneme(anim_data data)
{
	phoneList.push_back(data);
}

void LipAnimationGenerator::groupShortPhonemes(double minDuration)
{
	for (int i=0;i<phoneList.size()-1;i++)
	{
		if(phoneList.at(i).end-phoneList.at(i).start<minDuration)
		{
			phoneList.at(i+1).start=phoneList.at(i).start;
			phoneList.erase(phoneList.begin()+i);//TO DO: Eventually do something more eficient
		}
	}
	if(phoneList.at(phoneList.size()-1).end-phoneList.at(phoneList.size()-1).start<minDuration)
	{
		phoneList.at(phoneList.size()-2).end=phoneList.at(phoneList.size()-1).end;
		phoneList.erase(phoneList.begin()+phoneList.size()-1);
		groupShortPhonemes(minDuration);
	}
}

//This function generate the trajectory with motor command once every 1/30 sec.
void LipAnimationGenerator::generateAnimation()
{
	//cout.precision(4);

	int i=0;
	int j=0;
	frameCounter=0;
	int frameInserted=0; //Number of frame added to close the mouth for invalid transitions
	int totalFrame=0;//Number of frames to be animated for the current phoneme
	
	channel5->clear();
	channel6->clear();
	channel7->clear();

	double currentLength=0;
	groupShortPhonemes(0.1);//This function defines the shortest a phoneme can be. If shorter, the phoneme is grouped with the next one...
	float totalDuration=phoneList.at(phoneList.size()-1).end;
	int additional_phonemes = 12;

	lip_position previous={0,0,0};
	lip_position next={0,0,0};
	lip_position current={0,0,0};
	cout << "German mode" << German_mode << endl;
		if (German_mode)
		{
			if (phoneList.size() > 100)
			{
				cout << "Include more phonemes here" << endl;
				while(i<=  (int(phoneList.size()/50) - 1 ) * additional_phonemes)
				{
					phoneList.push_back(phoneList.at(i));
					i++;
				}
			}

		}
		cout << "Phoneme Size" << phoneList.size() << endl;
		i = 0;
		while(i<phoneList.size()) //i is the number of phonemes to be animated
		{
			currentLength=phoneList.at(i).end-phoneList.at(i).start;
			next=phoneToLip(phoneList.at(i).phone);
			
			//Check if the transition is a fobiddent mouth movement (i.e. Chanel 6 and 7 both different from 0)
			if(previous.motor2*next.motor3>0 || previous.motor3*next.motor2>0)
			{
				//insert a mouth closing movement at the beginning of the next phoneme
				//cout << "\n Inserting 3 Frames to close the mouth";
				frameInserted=3;
				next.motor1=0;
				next.motor2=0;
				next.motor3=0;
				for(j=1;j<=frameInserted;j++)
				{
					current.motor1=interpolate(previous.motor1,next.motor1, (double)j/frameInserted);
					current.motor2=interpolate(previous.motor2,next.motor2,  (double)j/frameInserted);
					current.motor3=interpolate(previous.motor3,next.motor3,  (double)j/frameInserted);
					
					channel5->push_back((unsigned char) current.motor1);
					channel6->push_back((unsigned char) current.motor2);
					channel7->push_back((unsigned char) current.motor3);
				}
				previous=next;
			}
			else
			{
				//interpolate between the current position and the next one. Reach the final position at the last frame.
				totalFrame=(int)(currentLength*30-frameInserted+0.5);
				for(j=1;j<=totalFrame;j++)//create the trajectory by interpolating between the previous position and the next one....
				{
					current.motor1=interpolate(previous.motor1,next.motor1, (double)j/totalFrame);
					current.motor2=interpolate(previous.motor2,next.motor2,  (double)j/totalFrame);
					current.motor3=interpolate(previous.motor3,next.motor3,  (double)j/totalFrame);
					
					channel5->push_back((unsigned char) current.motor1);
					channel6->push_back((unsigned char) current.motor2);
					channel7->push_back((unsigned char) current.motor3);
				}
				previous=next;
				frameInserted=0;
				i++;
			}
		}
		//cout << "\n Inserting 3 Frames to close the mouth";
		frameInserted=3;
		next.motor1=0;
		next.motor2=0;
		next.motor3=0;
		for(j=1;j<=frameInserted;j++)
		{
			current.motor1=interpolate(previous.motor1,next.motor1, (double)j/frameInserted);
			current.motor2=interpolate(previous.motor2,next.motor2,  (double)j/frameInserted);
			current.motor3=interpolate(previous.motor3,next.motor3,  (double)j/frameInserted);

			channel5->push_back((unsigned char) current.motor1);
			channel6->push_back((unsigned char) current.motor2);
			channel7->push_back((unsigned char) current.motor3);
			
		}
		//file.close();
		phoneList.clear();
		//cout << "\n NUMBER OF FRAMES CREATED: " << frameCounter;
		//cout <<"\n TOTAL DURATION SOUNDTRACK IS: "<< totalDuration;
}

lip_position LipAnimationGenerator::phoneToLip(const char * aPhone)
{
	lip_position result;
	int random_lip_position = 0;
	float reduceRange=0.9;//all the visemes are * by this value
	try
	{
		if (phonemeToViseme.count(aPhone) > 0)
		{
			if (visemeToLips.count(phonemeToViseme.at(aPhone)) > 0)
			{
			}
			else
			{
				//cout << "Not Found in Viseme To Lip Conversion " << phonemeToViseme.at(aPhone) << endl;
			}
		}
		else
		{
			//cout << "Not Found in Phoneme To Viseme Conversion " << aPhone << endl;
		}
		result=visemeToLips.at(phonemeToViseme.at(aPhone));//Complicated for no reason 
	}
	catch(exception &e)
	{
		cerr << "Caught: " << e.what( ) << endl;
		/*result.motor1=0;
		result.motor2=0;
		result.motor3=0;*/
		random_lip_position = rand() % 16;
		switch (random_lip_position) {
		case 0:
			result.motor1= 0;
			result.motor2=0;
			result.motor3=0;
			break;
			
		case 1:
			result.motor1= 114*reduceRange;//Values given by Robert
			result.motor2=255*reduceRange;
			result.motor3=0*reduceRange;
			break;
			
		case 2:
			result.motor1= 100*reduceRange;//Values given by Robert
			result.motor2=0;
			result.motor3=125*reduceRange;
			break;

		case 3:
			result.motor1= 55*reduceRange;//Values given by Robert
			result.motor2=255*reduceRange;
			result.motor3=0;
			break;

		case 4:
			result.motor1= 0;
			result.motor2=255*reduceRange;
			result.motor3=0;
			break;

		case 5:
			result.motor1=205*reduceRange;
			result.motor2=0;
			result.motor3=216*reduceRange;
			break;

		case 6:
			result.motor1=70*reduceRange;
			result.motor2=0;
			result.motor3=240*reduceRange;
			break;

		case 7:
			result.motor1=140*reduceRange;
			result.motor2=0;
			result.motor3=255*reduceRange;
			break;

		case 8:
			result.motor1=20*reduceRange;
			result.motor2=110*reduceRange;
			result.motor3=0;
			break;
		
		case 9:
			result.motor1=75*reduceRange;
			result.motor2=0;
			result.motor3=230*reduceRange;
			break;
		
		case 10:
			result.motor1=110*reduceRange;
			result.motor2=0;
			result.motor3=90*reduceRange;
			break;
		
		case 11:
			result.motor1=65*reduceRange;
			result.motor2=0;
			result.motor3=140*reduceRange;
			break;
		
		case 12:
			result.motor1=70*reduceRange;
			result.motor2=0;
			result.motor3=120*reduceRange;
			break;
		
		case 13:
			result.motor1=70*reduceRange;
			result.motor2=0;
			result.motor3=255*reduceRange;
			break;
		
		case 14:
			result.motor1=144*reduceRange;
			result.motor2=0;
			result.motor3=0;
			break;
		
		case 15:
			result.motor1=0;
			result.motor2=255*reduceRange;
			result.motor3=0;
			break;
		
		case 16:
			result.motor1=125*reduceRange;
			result.motor2=0;
			result.motor3=240*reduceRange;
			break;

		default:
			result.motor1= 0;
			result.motor2=0;
			result.motor3=0;
			break;
		}
	}
	return result;
}

void LipAnimationGenerator::generateAnimationBackUp()
{
	//cout.precision(4);
	float totalDuration=phoneList.at(phoneList.size()-1).end;
	int i=0;
	int j=0;
	int current=0;
	fstream file;
	groupShortPhonemes(0.1);
	file.open("../lipAnim.txt",fstream::out);

	if(!file.fail())
	{
		while(i<totalDuration*30) //i is the number of frame to be generated
		{
			if(i<phoneList.at(current).end*30)
			{
				//add viseme to animation
				file << "\n" << timeConversion(i) << ",0,0,0,0," << visemeToLips.at(phonemeToViseme.at(phoneList.at(current).phone)).motor1 <<"," << visemeToLips.at(phonemeToViseme.at(phoneList.at(current).phone)).motor2<<","<<visemeToLips.at(phonemeToViseme.at(phoneList.at(current).phone)).motor3 <<",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0," ;
				//cout << "\n phoneme: " << phoneList.at(current).phone;
				i++;
			}
			else
			{
				//cout << "\nPhoneme lasted: " << (phoneList.at(current).end-phoneList.at(current).start);
				current++;
			}
		}
		file.close();
		phoneList.clear();
	}
	else
	{
		cout << "\n Error: Failed to open the file ../lipAnim.txt ";
	}
}


string LipAnimationGenerator::timeConversion(int i)//00:00:00.00
{
	int temp1=i/30;
	int temp2=i%30;
	stringstream ss;
	string   s;

	if(temp1/60>0)
		NULL;//TO DO: have tts more that 1 minute.
	else if(temp1<10 && temp2<10)
		ss << "00:00:0"<<temp1<<".0"<<temp2;
	else if(temp1<10)
		ss << "00:00:0"<<temp1<<"."<<temp2;
	else if(temp2<10)
		ss << "00:00:"<<temp1<<".0"<<temp2;
	else
		ss << "00:00:"<<temp1<<"."<<temp2;

	s = ss.str();
	return s;
}

double LipAnimationGenerator::interpolate(double a, double b, double x)
{
	//return (b-a)*x;
	double ft = x * 3.1415927;
	double f = (1 - cos(ft)) * .5;
	//cout <<"\n interpolation: "<<a<<" " <<b<<" "<<x << "  "<<a*(1-f) + b*f;
	return  a*(1-f) + b*f;
}


