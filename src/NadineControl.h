#pragma once
#include "StdAfx.h"
//#include "NadineControl.h"
#include "AgentFeedbackService.h"
#include "ProtectedClient.h"
#include <iostream>
#include <string>
#include <vector>
#include "Joint.h"
#include "checkerCpp.h"
#include "Cervevoice.h"
#include "checkerWrapper.h"
#include "NadineAnimations.h"
#include "MayaWtenConverter.h"
#include <ctime>
#include "checkerCpp.h"
#include "Windows.h"
#include "WorldClient.h"

using namespace std;
using namespace System::Threading;

ref class NadineControl
{
public:
	NadineControl();
	NadineControl(char* voicePath, char* voicePathGerman, char* voicePathFrench);
	NadineControl(char* voicePath, char* voicePathGerman, char* voicePathFrench ,const char * feedbackIP);
	//NadineControl(char* voicePath, char* animeLibraryFolderPath);

	void initMe(bool initAnime);

	//~NadineControl(void);
	void askForText();//Ask text in the console
	void makeNadineSpeak(string text,const double volume);
	//void playAnimation(int id);

	//William added function overloading
	void playAnimation(string animeName);

	void endLessMovements();
	static int animToBePlayed;
	void groupAnimations();
	//void initAnimations();

	void gazeTarget(bool aTargetSet,string aTarget);
	//Add world Query for lookAtTarget

	void shakeHand(float x, float y, float z);




	void hToVoice(float x,float y,float z);

	int convert(double x) {return (int)x;};
	//void blink();

	bool shakeTimeInterval(int aTime);

	void lookAtPosition(float x, float y, float z);
	void NadineControl::moveEyes(int aLeftRightValue, int aUpDownValue);
	void pointToward(float x, float y, float z);

	void changePosture(string aPostureName);
	
	void loadAnimationLibrary( char* animeLibraryFolderPath );
	void lookAtTarget(std::string target);
	void setWorldClientIP(std::string ip);
	void setFeedbackServerIP(std::string ip);
	void loopLookAtTarget();
	void endLookAt();
	String ^ worldClientIP;
	String ^ feedbackServerIP;
	bool isUsingFeedback;

	String ^ currentBodyPosture;

	void generateRandTraj(int aChannelId, int startPoint, int aAnimLength);
	void initBody();

	string timeConversion(int i);
	vector<Joint> *body;
	Cervevoice *cerve;
	checkerCpp *checkerWilliam;
	//checkerWrapper^ myWrapper;

	String ^ currentPos;

	ProtectedClient<imi::AgentFeedbackServiceClient> *feedback;
	System::Media::SoundPlayer ^speechPlayer;

	//array <Animations ^> ^animations;

	AnimationLibrary ^ animeLibrary;
	char * animeLibraryFolder;

	// to be deleted
	//array <array<int>^> ^channels;

	int animLength;//number of frame to be animated
	array <bool> ^channelsToAnim;//channels that are not set to their default values

	//Flags used by the group animation function
	bool speak;
	bool blinking;
	bool speechFeedbackConnected;//Records connection problems

	bool alreadyChanged;

	bool shutDown;
	int shutCounter;

	Mutex ^mut; 
	Thread ^blink;
	Thread ^newThread; //This is the checker thread... Sorry for the name!
	Thread ^lookAtTargetThread; 
	static int processCount=0;

	//MLApp::MLApp ^matlab; //Used to call matlab
	time_t lastAction;

	String ^target; //o be used to store the name of the user to gaze at lookAtTarget
	bool targetSet; //If this is true, gaze at target lookAtTarget

	bool lookingUp;

	float positionX;
	float positionY;
	float positionZ;

	int lastChannel9;

	Joint * channel1;
	Joint * channel2;
	Joint * channel3;
	Joint * channel4;
	Joint * channel5;
	Joint * channel6;
	Joint * channel7;
	Joint * channel8;
	Joint * channel9;
	Joint * channel10;
	Joint * channel11;
	Joint * channel12;
	Joint * channel13;
	Joint * channel14;
	Joint * channel15;
	Joint * channel16;
	Joint * channel17;
	Joint * channel18;
	Joint * channel19;
	Joint * channel20;
	Joint * channel21;
	Joint * channel22;
	Joint * channel23;
	Joint * channel24;
	Joint * channel25;
	Joint * channel26;
	Joint * channel27;
	Joint * channel28;

	char * charIP ;
	char * lookAtTargetName;

	bool headAnimation;
};

