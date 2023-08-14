#pragma once
#include <string>
#include <vector>
//#pragma unmanaged
#include "XMLWrapper.h"
//#pragma managed

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace std;
using namespace std;
ref class Animations
{
public:
	Animations();
	Animations(String ^id, array <bool> ^channels);
	//std::map<string, string> animationsH;//Hash of animations
	String^ getAnimation(int id);

	void LoadAnimationFromXML( char * XMLFilename );

	bool isChannelUsing(int id);

	String^ animationName;

	array<String ^> ^ animAvailable;
	//array <array<int>^> ^channels;
	array <bool> ^channelsBool;//Which channels are animated
	int numberFrames;//Number of frames in the animation
	vector<vector<unsigned char>> * channels;

	vector<unsigned char> * channel1;
	vector<unsigned char> * channel2;
	vector<unsigned char> * channel3;
	vector<unsigned char> * channel4;
	vector<unsigned char> * channel5;
	vector<unsigned char> * channel6;
	vector<unsigned char> * channel7;
	vector<unsigned char> * channel8;
	vector<unsigned char> * channel9;
	vector<unsigned char> * channel10;
	vector<unsigned char> * channel11;
	vector<unsigned char> * channel12;
	vector<unsigned char> * channel13;
	vector<unsigned char> * channel14;
	vector<unsigned char> * channel15;
	vector<unsigned char> * channel16;
	vector<unsigned char> * channel17;
	vector<unsigned char> * channel18;
	vector<unsigned char> * channel19;
	vector<unsigned char> * channel20;
	vector<unsigned char> * channel21;
	vector<unsigned char> * channel22;
	vector<unsigned char> * channel23;
	vector<unsigned char> * channel24;
	vector<unsigned char> * channel25;
	vector<unsigned char> * channel26;
	vector<unsigned char> * channel27;
	vector<unsigned char> * channel28;

private:
	void initChannels();

	// helper functions
	int readAllXMLDoc(  XMLDocWrapper  * currDoc  );
	int readOneXMLNode( XMLNodeWrapper * currNode );

};


ref class AnimationLibrary
{
public:
	
	AnimationLibrary();
	
	int loadLibraryFromFolder(char * folderPath);

	Animations^ getAnimation(String^ animeName);
	Animations^ getPoseFrame(String^ animeName);

	int getAnimationSize();
	int getPoseFrameSize();

private:
	
	Dictionary <String^, Animations^>^animations;
	Dictionary <String^, Animations^>^representiveFrame;

};
