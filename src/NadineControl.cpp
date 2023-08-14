#pragma once
#include "StdAfx.h"
#include "AgentFeedbackService.h"
#include "NadineControl.h"
#include <iostream>
#include <sstream>
#include <string>
#include "NadineAnimations.h"
#include "checkerCpp.h"
#include "Cervevoice.h"
#include "Joint.h"
#include "checkerWrapper.h"

#include "structDef.h"
#include "checkerCpp.h"
#include <Windows.h>

#define FEEDBACK true

#define __debug
#define SMOOTHER 30
using namespace std;
using namespace System;
using namespace System::Threading;

NadineControl::NadineControl()
{

}

NadineControl::NadineControl(char * voicePath,  char* voicePathGerman, char* voicePathFrench)
{
	
	this->cerve=new Cervevoice(voicePath,voicePathGerman,voicePathFrench);
	//this->initMe(true);
	this->isUsingFeedback = false;
	/*this->speechFeedbackConnected=true;*/
	
}


void NadineControl::loadAnimationLibrary( char* animeLibraryFolderPath )
{
	fprintf(stderr,"loading animation library [%s]...\n",animeLibraryFolderPath);
	this->animeLibraryFolder = new char[2046];
	strcpy(this->animeLibraryFolder, animeLibraryFolderPath);
	this->animeLibrary = gcnew AnimationLibrary();
	this->animeLibrary->loadLibraryFromFolder(this->animeLibraryFolder);
	fprintf(stdout,"animation library loading completed, [%d] animations loaded\n", this->animeLibrary->getAnimationSize());
	fprintf(stdout, "animation library contain %d poses\n",this->animeLibrary->getPoseFrameSize());
	//fprintf(stdout,"%s",this->animeLibrary->);
}


NadineControl::NadineControl(char* voicePath,char* voicePathGerman, char* voicePathFrench,const char * feedbackIP)
{
	this->feedbackServerIP = gcnew String(feedbackIP);
	/*speechFeedbackConnected=true;*/
	this->isUsingFeedback = true;//CHANGED ON THE 17/12/2015
	this->cerve=new Cervevoice(voicePath,voicePathGerman,voicePathFrench);
	//this->initMe(true);
}

void NadineControl::initMe(bool initAnime)
{
	lookingUp=false;
	this->charIP = new char[20];
	
	animToBePlayed=0;
	animLength=0;
	speak=false;
	target="";
	targetSet=false;
	lastChannel9=80;
	channelsToAnim=gcnew array <bool> (28);
//	channels=gcnew array <array <int> ^> (28);

	body=new vector<Joint>();
	initBody();
	float x=0;
	float y=0;
	float z=0;

	speechPlayer=gcnew System::Media::SoundPlayer("../lipAnim.wav");

	//matlab = gcnew MLApp::MLApp();
	//matlab->Execute("cd D:/IMI-Project/i2p_Nadine_Control/matlab");
	String ^command="ShakeHandC=ShakeHand("+x+","+y+","+z+")";
//	matlab->Execute(command);
	mut=gcnew Mutex();

	//for(int i=0; i<28;i++)
	//	channelsToAnim[i]=false;
	//if (initAnime)
	//{
	//	animations=gcnew array <Animations ^>(14);
	//	initAnimations();
	//}
	
	//myWrapper=gcnew checkerWrapper();


	checkerWilliam=new checkerCpp();


	blink= gcnew Thread(gcnew ThreadStart(this, &NadineControl::endLessMovements));
	blink->Priority = ThreadPriority::Highest;
	blink->Start();


	lastAction=time(NULL);

	newThread=gcnew Thread(gcnew ThreadStart(this, &NadineControl::askForText));
	newThread->Start();

	this->lookAtTargetThread = gcnew Thread(gcnew ThreadStart(this, &NadineControl::loopLookAtTarget));

	alreadyChanged = false;
	//changePosture("poseGerman");
// 	this->lookAtTargetThread->Start();


	currentPos = "";
	
	shutDown = false;
	shutCounter = 0;


}

void NadineControl::lookAtTarget(std::string target)
{	
	int i ;
	for ( i = 0 ; i < this->worldClientIP->Length ; i ++ )
		charIP[i] = this->worldClientIP[i];
	charIP[i] = '\0';
	
	this->lookAtTargetName = new char[100];
	strcpy(this->lookAtTargetName, target.c_str());

	if(!lookAtTargetThread->IsAlive)
	{
		this->lookAtTargetThread->Start();
	}
}

void NadineControl::loopLookAtTarget()
{
	while(this->lookAtTargetName)
	{
		if(body->at(9).state==0 && body->at(8).state==0 && body->at(7).state==0)
		{
			imi::Location targetLocation;
			queryFromWorldClient(charIP, targetLocation, std::string(this->lookAtTargetName));
			this->lookAtPosition(targetLocation.location.x, targetLocation.location.y, targetLocation.location.z);
			sleep(90);
		}
	}
}

void NadineControl::endLookAt()
{
	if (this->lookAtTargetName)
	{
		//delete [] this->lookAtTargetName;
		this->lookAtTargetName = NULL;
		lookAtTargetThread->Join();

		body->at(7).updateDefaultPos(240);
		body->at(8).updateDefaultPos(240);//240
		body->at(9).updateDefaultPos(128);//128

		this->lookAtTargetThread = gcnew Thread(gcnew ThreadStart(this, &NadineControl::loopLookAtTarget));
	}
}

void NadineControl::setWorldClientIP(std::string ip)
{
	this->worldClientIP = gcnew String(ip.c_str());
}

void NadineControl::setFeedbackServerIP(std::string ip)
{
	if(ip!="")
	{
		//std::string i2p="localhost";
		std::string i2p=ip;
		this->feedbackServerIP = gcnew String(i2p.c_str());
		this->isUsingFeedback = true;

		char * charIP = new char[20];
		int i ;
		for ( i = 0 ; i < this->feedbackServerIP->Length ; i ++ )
			charIP[i] = this->feedbackServerIP[i];
		charIP[i] = '\0';
		fprintf(stdout,"before  feedback service at ip:[%s].....\n", charIP );
		//system("PAUSE");
		//feedback=new ProtectedClient<imi::AgentFeedbackServiceClient>("localhost",12007);
		feedback = new ProtectedClient<imi::AgentFeedbackServiceClient>(charIP, 12007);

		//this->isUsingFeedback = false;
	}
}


//This function is to control the robot with the Console.
void NadineControl::askForText()
{
	string text;
	string processed="";

	if (this->isUsingFeedback)
	{
		char * charIP = new char[20];
		int i ;
		for ( i = 0 ; i < this->feedbackServerIP->Length ; i ++ )
			charIP[i] = this->feedbackServerIP[i];
		charIP[i] = '\0';
		fprintf(stdout,"before initializing feedback service at ip:[%s].....\n", charIP );
		//system("PAUSE");
		feedback=new ProtectedClient<imi::AgentFeedbackServiceClient>(charIP,12007);
	}

	while(text!="q")
	{
		//cout << "\npress 's' to enter text to be spoken \n press 'c' to convert \n press 'g' to perform some animations \n press 'q' to exit \n";

		std::cout << "**********************************************************************************************\n";
		std::cout << "This software belongs to the Institute for Media Innovation, Nanyang Technological University. \n";
		std::cout << "The main developers are Aryel Beck and Zhang Zhijun \n";
		std::cout << "Animations were made by Chax Rivera \n";
		std::cout << "**********************************************************************************************\n \n \n";
		sleep(500);

		std::cout << "**********************************************************************************************\n";
		std::cout << "Please ensure that you have a valid license for CEREVOICE SDK \n";
		std::cout << "**********************************************************************************************\n \n \n";
		sleep(500);

		getline(cin,text);

		if (text == "shut")
		{
			playAnimation("SHUTDOWN_NADINE");
			shutDown = true;
			//sleep(9000);
			//blink->Suspend();
			
		}

		if(text=="s")
		{
			cout << " \n Please enter text for speech. Enter \"q\" to quit \n";
			getline(cin,text);

			if(text!="q")
			{
				makeNadineSpeak(text,10);
			}
			if(!blink->IsAlive)
			{
				blink= gcnew Thread(gcnew ThreadStart(this, &NadineControl::endLessMovements));
				blink->Start();
			}
		}

	if(text=="ger")
		{
			cout << " \n Please enter text for speech. Enter \"q\" to quit \n";
			getline(cin,text);

			if(text!="q")
			{
				makeNadineSpeak(text,20);
			}
			if(!blink->IsAlive)
			{
				blink= gcnew Thread(gcnew ThreadStart(this, &NadineControl::endLessMovements));
				blink->Start();
			}
		}
		if(text=="f")
		{
			cout << " \n Please enter text for speech. Enter \"q\" to quit \n";
			getline(cin,text);

			if(text!="q")
			{
				makeNadineSpeak(text,30);
			}
			if(!blink->IsAlive)
			{
				blink= gcnew Thread(gcnew ThreadStart(this, &NadineControl::endLessMovements));
				blink->Start();
			}
		}

		if(text=="set world ip")
		{
			if ( this->worldClientIP->Length == 0 )
			{
				cout << " \n Please enter the world client IP address:";
				getline(cin,text);
				this->worldClientIP = gcnew String(text.c_str());
			}
		}
		if(text=="c")
		{
			cout << " \n Please enter the path of the file to be converter (you can also drag and drop the file). Enter \"q\" to quit \n";
			getline(cin,text);
			if(text!="q")
			{
				MayaWtenConverter *converter=new MayaWtenConverter();
				converter->convert(text);
			}
		}
		if(text=="g")
		{
			cout << "\n Enter the name of the gestures you want to perform: ";
			getline(cin,text);
			if(text!="q")
			{
				if (text == "shut")
				{
					playAnimation("SHUTDOWN_NADINE");
					shutDown = true;
					sleep(4000);
					blink->Suspend();
				}
				else
				{
					playAnimation(text);
				}
			}
		}
		if (text == "p")
		{
			cout << "\n Enter the name of the pose you want to perform: ";
			getline(cin, text);
			changePosture(text);
		}
	}
}

void NadineControl::moveEyes(int aLeftRightValue, int aUpDownValue)
{
	body->at(2).updateDefaultPos((unsigned char)((int)body->at(2).defaultPos+aUpDownValue));
	body->at(3).updateDefaultPos((unsigned char)((int)body->at(3).defaultPos+aLeftRightValue));
//#ifdef __debug
	std::cout << "inputs for eyes position: " << aUpDownValue << " " << aUpDownValue << "\n";
	std::cout<<"new eyes position: "<< (int)body->at(2).defaultPos <<" " <<(int)body->at(3).defaultPos << "\n";
//#endif
}


void NadineControl::shakeHand(float x, float y, float z)
{
	/*int animLength=121;
	animToBePlayed++;

	newThread->Priority = ThreadPriority::Lowest;

	System::Array ^resa14=gcnew array<double>(animLength);
	System::Array ^resb14=gcnew array<double>(animLength);
	System::Array ^resa16=gcnew array<double>(animLength);
	System::Array ^resb16=gcnew array<double>(animLength);
	System::Array ^resa18=gcnew array<double>(animLength);
	System::Array ^resb18=gcnew array<double>(animLength);
	System::Array ^resa20=gcnew array<double>(animLength);
	System::Array ^resb20=gcnew array<double>(animLength);
	System::Array ^resa22=gcnew array<double>(animLength);
	System::Array ^resb22=gcnew array<double>(animLength);
	System::Array ^resa24=gcnew array<double>(animLength);
	System::Array ^resb24=gcnew array<double>(animLength);
	System::Array ^resa26=gcnew array<double>(animLength);
	System::Array ^resb26=gcnew array<double>(animLength);
	System::Array ^resa15=gcnew array<double>(animLength);
	System::Array ^resb15=gcnew array<double>(animLength);
	System::Array ^resa17=gcnew array<double>(animLength);
	System::Array ^resb17=gcnew array<double>(animLength);
	System::Array ^resa19=gcnew array<double>(animLength);
	System::Array ^resb19=gcnew array<double>(animLength);
	System::Array ^resa21=gcnew array<double>(animLength);
	System::Array ^resb21=gcnew array<double>(animLength);
	System::Array ^resa23=gcnew array<double>(animLength);
	System::Array ^resb23=gcnew array<double>(animLength);
	System::Array ^resa25=gcnew array<double>(animLength);
	System::Array ^resb25=gcnew array<double>(animLength);
	System::Array ^resa27=gcnew array<double>(animLength);
	System::Array ^resb27=gcnew array<double>(animLength);
	matlab->Execute("cd D:/Aryel/i2p_Nadine_Control/matlab");

	matlab->Execute("Channel15=ShakeHandC(:,2)");
	matlab->GetFullMatrix("Channel15","base",resa15,resb15);

	matlab->Execute("Channel17=ShakeHandC(:,4)");
	matlab->GetFullMatrix("Channel17","base",resa17,resb17);

	matlab->Execute("Channel19=ShakeHandC(:,6)");
	matlab->GetFullMatrix("Channel19","base",resa19,resb19);

	matlab->Execute("Channel21=ShakeHandC(:,8)");
	matlab->GetFullMatrix("Channel21","base",resa21,resb21);

	matlab->Execute("Channel23=ShakeHandC(:,10)");
	matlab->GetFullMatrix("Channel23","base",resa23,resb23);

	matlab->Execute("Channel25=ShakeHandC(:,12)");
	matlab->GetFullMatrix("Channel25","base",resa25,resb25);

	matlab->Execute("Channel27=ShakeHandC(:,14)");
	matlab->GetFullMatrix("Channel27","base",resa27,resb27);

	//myWrapper->stopAnim();

	double temp15=0;
	std::vector<unsigned char> *tmpChannel=new vector<unsigned char>();
	for(int i=0;i<animLength;i++)
	{
		temp15 =(double)resa15->GetValue(i);
		tmpChannel->push_back((unsigned char)temp15);
	}
	body->at(14).addTrajectory(*tmpChannel);
	tmpChannel->clear();

	double temp17=0;
	for(int i=0;i<animLength;i++)
	{
		temp17 =(double)resa17->GetValue(i);
		tmpChannel->push_back((unsigned char)temp17);
	}
	body->at(16).addTrajectory(*tmpChannel);
	tmpChannel->clear();

	double temp19=0;
	for(int i=0;i<animLength;i++)
	{
		temp19 =(double)resa19->GetValue(i);
		tmpChannel->push_back((unsigned char)temp19);
	}
	body->at(18).addTrajectory(*tmpChannel);
	tmpChannel->clear();

	double temp21=0;
	for(int i=0;i<animLength;i++)
	{
		temp21 =(double)resa21->GetValue(i);
		tmpChannel->push_back((unsigned char)temp21);
	}
	body->at(20).addTrajectory(*tmpChannel);
	tmpChannel->clear();

	double temp23=0;
	for(int i=0;i<animLength;i++)
	{
		temp23 =(double)resa23->GetValue(i);
		tmpChannel->push_back((unsigned char)temp23);
	}
	body->at(22).addTrajectory(*tmpChannel);
	tmpChannel->clear();

	double temp25=0;
	//channels[24]=gcnew array<int>(animLength);
	for(int i=0;i<animLength;i++)
	{
		temp25 =(double)resa25->GetValue(i);
		tmpChannel->push_back((unsigned char)temp25);
	}
	body->at(24).addTrajectory(*tmpChannel);
	tmpChannel->clear();


	double temp27=0;
	for(int i=0;i<animLength;i++)
	{
		temp27 =(double)resa27->GetValue(i);
		tmpChannel->push_back((unsigned char)temp27);
	}
	body->at(26).addTrajectory(*tmpChannel);
	tmpChannel->clear();
}
*/
}
//Make the robot blink. Should be called in a separate thread.
void NadineControl::endLessMovements()
{
	blinking=true;
	while(blinking)
	{
		groupAnimations();
		System::Threading::Thread::Sleep(30);
		//System::Threading::Thread::Sleep(120);
	}
}

//Takes a string as input and convert it to an animation and sound file. Update the appropriate channels and flags.
void NadineControl::makeNadineSpeak(string text,const double volume)
{

	if (shutDown)
		return;

	if (currentPos!="LOOKUPPostureDefault")
		changePosture("LOOKUPPostureDefault");

	animToBePlayed++;

	cout << "Speech request: " << text << "\n";

	// James comes to check the lip of cerevoice
	if(volume == 10)
	{
		lip_position * processed=cerve->processString(text, false); //false refers to english language
	}
	if(volume == 20)
	{
		lip_position * processed1=cerve->processStringGerman(text);
	}
	if(volume == 30)
	{
		lip_position * processed2=cerve->processStringFrench(text);
	}
	if(volume == 40)
	{
		lip_position * processed=cerve->processString(text, true); //true refers to hindi, chinese, japanese language, where phonemes have to be included.
	}
	int animSize=cerve->getAnimSize();

	//mut->WaitOne();
	if (animSize>animLength)
		animLength=animSize;

	body->at(4).clearTrajectory();
	body->at(5).clearTrajectory();
	body->at(6).clearTrajectory();

	body->at(4).addTrajectory(*cerve->generator->channel5);
	body->at(5).addTrajectory(*cerve->generator->channel6);
	body->at(6).addTrajectory(*cerve->generator->channel7);

	animToBePlayed--;
	//mut->ReleaseMutex();
	
	
	if (volume < 40){
		speak=true;
		speechPlayer->Load();
	
		fprintf(stdout,"%d\n", this->isUsingFeedback);
	
		printf("Inside make Nadinespeak function before feedback \n");
		if(this->isUsingFeedback)
		{
			if(feedback->ensureConnection())
			{
				try
				{
					feedback->getClient()->send_speakBegin("Nadine", 0);
					/*speechFeedbackConnected=true;*/
				}
				catch( apache::thrift::TException &tx ) 
				{
					std::cerr << "EXCEPTION on [SendSoundStartToServer()]: opening the network conn: " << tx.what() << "\n";
					feedback->receiveNetworkException();
					/*speechFeedbackConnected=false;*/
					//return 1;
				}
			}
		}
			printf("Inside makeNadinespeak function after feedback\n");	
		speechPlayer->Play();
	}

}

void NadineControl::changePosture(string aPostureName)
{
	System::String^ mgPostureName = gcnew System::String(aPostureName.c_str());
	Animations^ currAnimation = this->animeLibrary->getPoseFrame(mgPostureName);
	currentPos = mgPostureName;
	if (currAnimation == nullptr)
	{
		cerr << "No animation named [" << aPostureName << "] in the posture library!\n";
		return;
	}
	currentBodyPosture = mgPostureName;
	for (int i = 0; i<28; i++)
	{
		if (currAnimation->isChannelUsing(i))
		{
			//if (abs(currAnimation->channels->at(i).at(0) - body->at(i).defaultPos)>20)
			{
				vector<unsigned char> temp;
				int traj = abs(currAnimation->channels->at(i).at(0) - body->at(i).defaultPos) / SMOOTHER;
				if (currAnimation->channels->at(i).at(0) > body->at(i).defaultPos)
				{
					for (int j = 0; j <= SMOOTHER; j++)
						temp.push_back((traj * j) + body->at(i).defaultPos);
				}
				else
				{	
					for (int j = 0; j <= SMOOTHER; j++)
						temp.push_back(-(traj * j) + body->at(i).defaultPos);
				}
				body->at(i).addTrajectory(temp);
				temp.clear();
			}
			body->at(i).updateDefaultPos(currAnimation->channels->at(i).at(0));
		}
	}
	lookingUp = aPostureName == "LOOKUPPostureDefault";
}



/*void NadineControl::changePosture(string aPostureName)
{
	System::String^ mgPostureName = gcnew System::String(aPostureName.c_str());
	Animations^ currAnimation = this->animeLibrary->getPoseFrame(mgPostureName);
	if (currAnimation==nullptr)
	{
		cerr << "No animation named [" << aPostureName << "] in the posture library!\n" ;
		return;
	}
	currentBodyPosture=mgPostureName;
	for(int i=0;i<28;i++)
	{
		if(currAnimation->isChannelUsing(i))
		{
			body->at(i).updateDefaultPos(currAnimation->channels->at(i).at(0));
		}
	}
	lookingUp=aPostureName=="LOOKUPPostureDefault";
}*/
void NadineControl::playAnimation(string animeName)
{
	try{
		if (shutDown)
			return;

		//added to control the gaze along with head nods and shake
		if (animeName == "nod" || animeName == "shake")
			headAnimation = true;

		if(animeName!="blink")
		{
		
			if(animeName!="Laptop_Default_idle")
			{
				changePosture("LOOKUPPostureDefault");
			}
			else
			{
				changePosture("poseGerman");
			}
		}
		if (animeName == "LOOKUP_Waving" || animeName == "SHUTDOWN_NADINE")
		{
			for(int i=0; i<27;i++)
			{
				body->at(i).clearTrajectory();
			}
		}

		if (animeName == "Laptop_Default_idle")
			alreadyChanged = false;

		System::String^ mgAnimeName = gcnew System::String(animeName.c_str());
		Animations^ currAnimation = this->animeLibrary->getAnimation(mgAnimeName);
		if (currAnimation->animationName != mgAnimeName)
		{
			cerr << "No animation named [" << animeName << "] in the animation library!\n" ;
			return;
		}

		if (animeName == "blink" || animeName == "breathingSlow") //idle motion
		{
			for(int i = 0 ; i < 28 ; i ++ )
			{
				if(currAnimation->isChannelUsing(i)) //&& channelsToAnim[i]!=true)
				{
					body->at(i).addIdleMovement(currAnimation->channels->at(i));//,animations[id]->numberFrames);
				}
			}
		}
		else
		{
			for(int i = 0 ; i < 28 ; i ++ )
			{
				if(currAnimation->isChannelUsing(i)) //&& channelsToAnim[i]!=true)
				{
					if((i==4||i==5||i==6)&&(this->speak))
					{
						this->speak=false;
						body->at(4).clearTrajectory();
						body->at(5).clearTrajectory();
						body->at(6).clearTrajectory();
						speechPlayer->Stop();
						if(this->isUsingFeedback /*&& this->speechFeedbackConnected*/)
						{	
							printf("Inside playAnimation function before speech feedback \n");
							if(feedback->ensureConnection())
							{
								try
								{
									feedback->getClient()->send_speakEnd("Nadine", 0);
								}
								catch( apache::thrift::TException &tx ) 
								{
									std::cerr << "EXCEPTION on [FeedbackClientConnection]: opening the network conn: " << tx.what() << "\n";
									feedback->receiveNetworkException();
									//return 1;
								}
							}
						}
						printf("Inside playAnimation function after speech feedback \n");



					}
				
					body->at(i).addTrajectory(currAnimation->channels->at(i));//,animations[id]->numberFrames);
					if (animeName == "Idle_Transition"){
						body->at(i).updateDefaultPos(currAnimation->channels->at(i).back());
						printf("Update Default Position!\n");
					}
				}
			}
		}

		if (currAnimation->numberFrames > animLength)
			animLength=currAnimation->numberFrames;

		animToBePlayed--;
		lastAction=time(NULL);
	}
	catch (...){
		std::cout << "An exception occurred in Play Animation '\n'";
	}
}


//Send an animation to the checker and re start the blink if needed.
//void NadineControl::playAnimation(int id)
//{
//	if(id!=100)//&&(shakeTimeInterval(5)))
//	{
//		if(id==0||id==10)//This is an idle motion
//		{
//			for(int i=0;i<28;i++)
//			{
//				if(animations[id]->channelsBool[i]==true) //&& channelsToAnim[i]!=true)
//				{
//					body->at(i).addIdleMovement(animations[id]->channels->at(i));//,animations[id]->numberFrames);
//				}
//			}
//		}
//		else
//		{
//			for(int i=0;i<28;i++)
//			{
//				if(animations[id]->channelsBool[i]==true) //&& channelsToAnim[i]!=true)
//				{
//					if((i==4||i==5||i==6)&&(speak))
//					{
//						speak=false;
//						body->at(4).clearTrajectory();
//						body->at(5).clearTrajectory();
//						body->at(6).clearTrajectory();
//						speechPlayer->Stop();
//						if(this->isUsingFeedback)
//							feedback->getClient()->send_speakEnd("Nadine", 0);
//					}
//
//					body->at(i).addTrajectory(animations[id]->channels->at(i));//,animations[id]->numberFrames);
//				}
//			}
//		}
//
//		if (animations[id]->numberFrames>animLength)
//			animLength=animations[id]->numberFrames;
//
//		animToBePlayed--;
//		lastAction=time(NULL);
//	}
//	//if(!blink->IsAlive)
//	//{
//	//	blink= gcnew Thread(gcnew ThreadStart(this, &NadineControl::endLessMovements));
//	//	blink->Start();
//	//}
//
//}


void NadineControl::groupAnimations()
{
	unsigned char currentFrame[28];

	if (shutDown && shutCounter >= 180)
		return;

	//check if nod is finished 
	if (headAnimation && body->at(2).state == 0 && body->at(3).state == 0 && body->at(7).state == 0 && body->at(8).state == 0 && body->at(9).state == 0)
		headAnimation = false;
	

	if(body->at(0).state==0 && currentBodyPosture!="poseNeutralEyesClosed")
		playAnimation("blink");

	//Uncomment the following lines to put back the breathing
	/*if(body->at(13).state==0)
		playAnimation("breathingSlow");*/

	//mut->WaitOne(1000);
	for(int i=0;i<28;i++)
	{
		currentFrame[i]=body->at(i).getFrame();
	}
	if(currentFrame[5]>0&&currentFrame[6]>0)
	{
		currentFrame[5]=0;
		currentFrame[6]=0;
		std::cout<<"Warning: Forbidden lips movement. Closing the mouth \n";
	}
		
	checkerWilliam->MoveOneFrame(currentFrame);

	if (shutDown && shutCounter<1000)
		shutCounter++;

	if(speak&&body->at(4).state==0&&body->at(5).state==0&&body->at(6).state==0)
	{
// 		if(this->isUsingFeedback) BACKUP 1/07/2014
// 			feedback->getClient()->send_speakEnd("Nadine", 0);
// 		speak=false;

		speak=false;
		if(this->isUsingFeedback /*&& this->speechFeedbackConnected*/)
		{	
			printf("Inside groupAnimation function before speech feedback \n");
			if(feedback->ensureConnection())
			{
				try
				{
					feedback->getClient()->send_speakEnd("Nadine", 0);
				}
				catch( apache::thrift::TException &tx ) 
				{
					std::cerr << "EXCEPTION on [FeedbackClientConnection]: opening the network conn: " << tx.what() << "\n";
					feedback->receiveNetworkException();
					//return 1;
				}
			}
		}
		printf("Inside GroupAnimation function after speech feedback \n");

	}
	
}


int myround(double x)
{
	if( x > 0.0 )
		return (int)( x + 0.5 );
	else
		return (int)( x - 0.5 );
} 

//void NadineControl::lookAtPosition(float xs, float ys, float zs)

void NadineControl::lookAtPosition(float xw, float yw, float zw)
{
	if (shutDown || headAnimation)
		return;

	float xrd,yrd,zrd,xr,yr,zr;
	//xrd = 0.75;
	//yrd = 2.45;
	//zrd = 0.75;
	xrd = 0;//0.54
	yrd = -0.3;//2.6
	zrd =0.76;//0.75
	//cout << "xw: " << xw <<" yw:"<<yw<<" zw:"<<zw<<"\n";

	//The next three lines are from world system to robot system
	/*
	xr = 1.4142/2*(xw + yw - xrd - yrd); //sqrt(2)=1.414;
	yr = 1.4142/2*(-xw + yw + xrd - yrd);
	zr = zw - zrd;
	
	cout << "xr: " << xr <<" yr:"<<yr<<" zr:"<<zr<<"\n";
	*/

	xr =-xw;		//sqrt(2)=1.414;
	yr =zw+yrd;
	zr =yw+zrd;

	//float neckLRmax = 16.5*3.1415926/180;
	// float neckLRmin = -16.5*3.1415926/180;

	float neckUNDmax = 14.5*3.14159265/180;
	float neckUNDmin = -14.5*3.14159265/180;

	//float neckUNDmax = 8.5*3.14159265/180;
	//float neckUNDmin = -8.5*3.14159265/180;

	//float neckLRmax=35*3.14159265/180;  //left and right limits of the neck
	//float neckLRmin=-35*3.14159265/180; 
	float neckLRmax=0.3;  //left and right limits of the neck
	float neckLRmin=-0.3; 

	float eyeNUDmax=1*3.14159265/180; //down is positive;limits of the eyes
	float eyeNUDmin=-1*3.14159265/180; //up is minus
	float eyeLRmax= 35*3.14159265/180; //down is positive; //limits of the eyes
	float eyeLRmin= -35*3.14159265/180; //up is positive; //limits of the eyes

	float cha10 = 128;
	float cha9 = 128;
	float cha8 = 128;

	float cha4 = 128;
	float cha3 = 128;

	//cout << "neckmax is" << neckmax <<"\n" << "neckmin is" << neckmin <<"\n";
	float thetaNeckLR = 0;// Turn left and right
	float thetaNeckUND = 0; // Turn up and down
	float thetaEyeLR = 0;// Turn left and right
	float thetaEyeUND = 0; // Turn up and down

	float ZTneckUPlimit=0;
	float ZTneckDOWNlimit=0;

	float lN = 0.405;//the height of neck
	//float lE = 0.135+0.405;//the distance between neck and eyes
	float lEN = 0.135;//the distance between neck and eyes
	float thetaLR = 0;

	// neck coded by Jason

	thetaNeckLR = -atan(xr/yr);
	thetaLR = -atan(xr/yr);

	//cout << "robot neck theta turns left or right" << thetaNeckLR <<"\n";


	//ZTneckUPlimit=(abs(yr)+lEN*sin(neckUNDmin))*sin(neckUNDmin)+lN+lN*cos(neckUNDmin);
	ZTneckUPlimit=(abs(yr)+lEN*sin(abs(neckUNDmin)))*sin(abs(neckUNDmin))+lN+lN*cos(abs(neckUNDmin));
	//ZTneckDOWNlimit=(lN+lEN*cos(neckUNDmax))-(abs(yr)-lEN*sin(neckUNDmax))*tan(neckUNDmax);
	ZTneckDOWNlimit=(lN+lEN/cos(neckUNDmax))-(abs(yr)-lEN*sin(neckUNDmax))*tan(neckUNDmax);

	if (zr>ZTneckUPlimit)
	{
		thetaNeckUND=neckUNDmin;
		//thetaEyeUND=-atan((zr-(lN+lEN*cos(ZTneckUPlimit)))/(abs(yr)+lEN*sin(thetaNeckUND)))-thetaNeckUND;  
	    thetaEyeUND=-atan((zr-(lN+lEN*cos(thetaNeckUND)))/(abs(yr)+lEN*sin(abs(thetaNeckUND))))-abs(thetaNeckUND);  
	}

	else if (zr<ZTneckDOWNlimit)
	{
		thetaNeckUND=neckUNDmax;
	    thetaEyeUND=atan((abs(yr)-lEN*sin(thetaNeckUND))/(lN+lEN*cos(thetaNeckUND)-zr));
	}
	else
	{
		thetaNeckUND=atan((lN+lEN-zr)/abs(yr));
	    thetaEyeUND=0;
	}		
	/*
	cout<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n In Robot space, user position is: "<<xr<<" "<<yr<<" "<<zr<<" \n";

	cout << "ZTneckDOWNlimit: " << ZTneckDOWNlimit <<"\n";
	cout << "ZTneckUPlimit: " << ZTneckUPlimit <<"\n";
	cout << "zr: " << zr <<"\n";*/

		// neck limit handling 
	if (thetaNeckLR > neckLRmax)
	{
		thetaNeckLR = neckLRmax;
	}

	if (thetaNeckLR < neckLRmin)
	{
		thetaNeckLR = neckLRmin;
	}
		
	//cout << "theta between max and min: " << thetaNeckLR <<"\n";

	//thetaUND = 3.1415926/2-acos((lE-lN)/sqrt(xr*xr+yr*yr+(zr-lN)*(zr-lN)))-asin((zr-lN)/sqrt(xr*xr+yr*yr+(zr-lN)*(zr-lN)));

	if (thetaNeckUND > neckUNDmax)
	{
		thetaNeckUND = neckUNDmax;
	}

	if (thetaNeckUND < neckUNDmin)
	{
		thetaNeckUND = neckUNDmin;
	}

	if (thetaNeckLR==neckLRmax)
	{
		//thetaEyeLR=thetaEyeLR-eyeLRmax;

		thetaEyeLR=thetaLR-thetaNeckLR;

	}
	else if(thetaNeckLR==neckLRmin)
	{
		//thetaEyeLR=thetaEyeLR-eyeLRmin;
		thetaEyeLR=thetaLR-thetaNeckLR;
	}
	else
	{
		thetaEyeLR=0;
	}



	
	// we set the eye updown movement to be zero to avoid undesirable results. By James
	thetaEyeUND=0;

	//// eyes limit handling 
	//if (thetaEyeUND>eyeNUDmax){
		//thetaEyeUND=eyeNUDmax;
		//printf("Eye movement reaches Max limit!\n");
	//}
	//if (thetaEyeUND<eyeNUDmin){
		//thetaEyeUND=eyeNUDmin;
		//printf("Eye movement reaches Min limit!\n");
	//}


	if (thetaEyeLR>eyeLRmax)
		thetaEyeLR=eyeLRmax;
	if (thetaEyeLR<eyeLRmin)
		thetaEyeLR=eyeLRmin;

/*	
	cout << "neckUNDmax: " << neckUNDmax <<"\n";
	cout << "neckUNDmin: " << neckUNDmin <<"\n";
	cout << "thetaNeckUND: " << thetaNeckUND <<"\n";
	cout << "\n";


	cout << "neckLRmax: " << neckLRmax <<"\n";
	cout << "neckLRmin: " << neckLRmin <<"\n";
	cout << "thetaNeckLR: " << thetaNeckLR <<"\n";
	cout << "\n";

	cout << "eyeUNDmax: " << eyeNUDmax <<"\n";
	cout << "eyeUNDmin: " << eyeNUDmin <<"\n";
	cout << "thetaEyeUND: " << thetaEyeUND <<"\n";
	cout << "\n";
	
	cout << "eyeLRmax: " << eyeLRmax <<"\n";
	cout << "eyeLRmin: " << eyeLRmin <<"\n";
	cout << "thetaEyeLR: " << thetaEyeLR <<"\n";
	cout << "\n";


	*/

	//====================
	cha10 = 255*(thetaNeckLR-neckLRmin)/(neckLRmax-neckLRmin);

	cha8 = cha9 = 255-255*(thetaNeckUND-neckUNDmin)/(neckUNDmax-neckUNDmin);

	//cout << "cha8:" << cha8 << "\n";

	cha4 = 255*(thetaEyeLR-eyeLRmin)/(eyeLRmax-eyeLRmin);

	//cout << "cha4: " << cha4 <<"\n";

	cha3 =255-255*(thetaEyeUND-eyeNUDmin)/(eyeNUDmax-eyeNUDmin);

	//cout << "cha3:" << cha3 << "\n";
	if (!alreadyChanged)
	{
		changePosture("LOOKUPPostureDefault");
		alreadyChanged = true;
	}


	//body->at(9).addFrame((unsigned char) myround(cha10));
	body->at(9).updateDefaultPos((unsigned char) myround(cha10));

	//body->at(7).addFrame((unsigned char) myround(cha8));
	body->at(7).updateDefaultPos((unsigned char) myround(cha8));

	//body->at(8).addFrame((unsigned char) myround(cha9));
	body->at(8).updateDefaultPos((unsigned char) myround(cha9));

	//cout<<cha10 <<" "<<myround(cha9)<<" "<<myround(cha8)<<" \n";

	body->at(3).updateDefaultPos((unsigned char) myround(cha4));
	body->at(2).updateDefaultPos((unsigned char) myround(cha3));

	//Need to define a new pos with body ok for gazing
	//body->at(10).updateDefaultPos(128);
	//body->at(11).updateDefaultPos(128);

	//Added by Aryel to interrupt ongoing animations
	body->at(2).clearTrajectory();
	body->at(3).clearTrajectory();
	body->at(7).clearTrajectory();
	body->at(8).clearTrajectory();
	body->at(9).clearTrajectory();
	//body->at(10).clearTrajectory();
	//body->at(11).clearTrajectory();

	//body->at(3).updateDefaultPos(128);
	//body->at(0).addTrajectory(std::vector<unsigned char> aTrajectory);

	//if(body->at(0).state==0||body->at(0).state==2)
	//if(body->at(0).state==0)
		//playAnimation("blinkFast");
	//bool flag=0;
	//if (0==rand()% 2)
	//{
	//	playAnimation("blinkFast");
	//}

	//float xrd,yrd,zrd,xr,yr,zr;
	////xrd = 0.75;
	////yrd = 2.45;
	////zrd = 0.75;
	//xrd = 0.54;
	//yrd = 2.6;
	//zrd = 0.75;
	//cout << "xw: " << xw <<" yw:"<<yw<<" zw:"<<zw<<"\n";

	////The next three lines are from world system to robot system
	//xr = 1.4142/2*(xw + yw - xrd - yrd); //sqrt(2)=1.414;
	//yr = 1.4142/2*(-xw + yw + xrd - yrd);
	//zr = zw - zrd;

	//cout << "xr: " << xr <<" yr:"<<yr<<" zr:"<<zr<<"\n";

	//float neckmax = 16.5*3.1415926/180;
	//float neckmin = -16.5*3.1415926/180;

	//float neckUNDmax = 14.5*3.14159265/180;
	//float neckUNDmin = -14.5*3.14159265/180;

	//float cha10 = 128;
	//float cha9 = 128;
	//float cha8 = 128;
	//cout << "neckmax is" << neckmax <<"\n" << "neckmin is" << neckmin <<"\n";
	//float thetaLR = 0;// Turn left and right
	//float thetaUND = 0; // Turn up and down

	//float lN = 0.405;//the height of neck
	//float lE = 0.135+0.405;//the distance between neck and eyes

	//thetaLR = -atan(xr/yr);

	//cout << "robot neck theta" << thetaLR <<"\n";

	//if (thetaLR > neckmax)
	//{
	//	thetaLR = neckmax;
	//}

	//if (thetaLR < neckmin)
	//{
	//	thetaLR = neckmin;
	//}
	//	
	//cout << "theta between max and min: " << thetaLR <<"\n";

	//thetaUND = 3.1415926/2-acos((lE-lN)/sqrt(xr*xr+yr*yr+(zr-lN)*(zr-lN)))-asin((zr-lN)/sqrt(xr*xr+yr*yr+(zr-lN)*(zr-lN)));

	//if (thetaUND > neckUNDmax)
	//{
	//	thetaUND = neckUNDmax;
	//}

	//if (thetaUND < neckUNDmin)
	//{
	//	thetaUND = neckUNDmin;
	//}

	//cout << "thetaUND between max and min: " << thetaUND <<"\n";

	//cha10 = 255*(thetaLR-neckmin)/(neckmax-neckmin);

	//cha8 = cha9 = 255-255*(thetaUND-neckUNDmin)/(neckUNDmax-neckUNDmin);

	////body->at(9).addFrame((unsigned char) myround(cha10));
	//body->at(9).updateDefaultPos((unsigned char) myround(cha10));

	////body->at(7).addFrame((unsigned char) myround(cha8));
	//body->at(7).updateDefaultPos((unsigned char) myround(cha8));

	////body->at(8).addFrame((unsigned char) myround(cha9));
	//body->at(8).updateDefaultPos((unsigned char) myround(cha9));

	//cout<<cha10 <<" "<<myround(cha9)<<" "<<myround(cha8)<<" \n";

}

void NadineControl::generateRandTraj(int aChannelId, int startPoint, int aAnimLength)
{//range of the noise is fixed in this version
	/*channels[aChannelId]=gcnew array <int> (aAnimLength);
	for(int i=0; i<aAnimLength;i++)
	{
	channels[aChannelId][i]=startPoint+System::Random()%60-30;
	}
	channelsToAnim[aChannelId]=true;*/
}

//void NadineControl::initAnimations()
//{
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[0]=true;
//	animations[0]=gcnew Animations("blink",channelsToAnim);
//
//	channelsToAnim[0]=false;
//	channelsToAnim[7]=true;
//	channelsToAnim[8]=true;
//	animations[1]=gcnew Animations("nod",channelsToAnim);
//
//	channelsToAnim[0]=true;
//	channelsToAnim[1]=true;
//	channelsToAnim[2]=true;
//	channelsToAnim[9]=true;
//	channelsToAnim[7]=false;
//	channelsToAnim[8]=false;
//	animations[2]=gcnew Animations("shake",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[0]=true;
//	channelsToAnim[4]=true;
//	channelsToAnim[6]=true;
//
//	animations[3]=gcnew Animations("happy",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	for(int i=14;i<27;i++)
//	{
//		channelsToAnim[i]=true;
//	}
//	animations[4]=gcnew Animations("praise",channelsToAnim);
//
//	for(int i=15;i<27;i+=2)
//	{
//		channelsToAnim[i]=false;
//	}
//	animations[5]=gcnew Animations("wave",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[0]=true;
//	channelsToAnim[7]=true;
//	channelsToAnim[8]=true;
//	channelsToAnim[10]=true;
//	channelsToAnim[11]=true;
//	channelsToAnim[15]=true;
//	channelsToAnim[16]=true;
//	channelsToAnim[17]=true;
//	channelsToAnim[18]=true;
//	channelsToAnim[19]=true;
//	channelsToAnim[20]=true;
//	channelsToAnim[21]=true;
//	channelsToAnim[22]=true;
//	channelsToAnim[23]=true;
//	channelsToAnim[24]=true;
//	channelsToAnim[25]=true;
//	channelsToAnim[26]=true;
//
//	animations[6]=gcnew Animations("thanks",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[0]=true;
//	channelsToAnim[2]=true;
//	channelsToAnim[3]=true;
//	channelsToAnim[7]=true;
//	channelsToAnim[8]=true;
//	channelsToAnim[9]=true;
//	channelsToAnim[16]=true;
//	channelsToAnim[18]=true;
//	channelsToAnim[20]=true;
//	channelsToAnim[24]=true;
//	channelsToAnim[26]=true;
//
//	animations[7]=gcnew Animations("sit",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[0]=true;
//	channelsToAnim[2]=true;
//	channelsToAnim[4]=true;
//	channelsToAnim[7]=true;
//	channelsToAnim[8]=true;
//	channelsToAnim[9]=true;
//	channelsToAnim[10]=true;
//	channelsToAnim[11]=true;
//	channelsToAnim[13]=true;
//	channelsToAnim[14]=true;
//	channelsToAnim[17]=true;
//	channelsToAnim[18]=true;
//	channelsToAnim[21]=true;
//	channelsToAnim[22]=true;
//
//	animations[8]=gcnew Animations("sad",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[0]=true;
//	channelsToAnim[1]=true;
//	channelsToAnim[2]=true;
//	channelsToAnim[4]=true;
//	channelsToAnim[10]=true;
//	channelsToAnim[11]=true;
//	channelsToAnim[13]=true;
//	channelsToAnim[14]=true;
//	channelsToAnim[17]=true;
//	channelsToAnim[18]=true;
//	channelsToAnim[19]=true;
//	channelsToAnim[20]=true;
//
//	animations[9]=gcnew Animations("angry",channelsToAnim);
//
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[13]=true;
//	channelsToAnim[14]=true;
//
//	animations[10]=gcnew Animations("BreathingSlow",channelsToAnim);
//	animations[11]=gcnew Animations("BreathingFast",channelsToAnim);
//
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[9]=true;
//
//	animations[12]=gcnew Animations("read",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//	channelsToAnim[7]=true;
//	channelsToAnim[8]=true;
//	animations[13]=gcnew Animations("headnod",channelsToAnim);
//
//	for(int i=0; i<28;i++)
//		channelsToAnim[i]=false;
//
//}

//The following functions are animations. this is a temporary solution and they should be replaced by XML files.


void NadineControl::gazeTarget(bool aTargetSet, string aTarget)
{
	//lookAtTarget
	//target=aTarget;
	//	for(int i=0;i<aTarget.size();i++)
	//		target[i]=aTarget.at(i);

	targetSet=aTargetSet;
}

void NadineControl::hToVoice(float xs,float ys,float zs)//data from sound coordinate, convert it into robot coordinate, and control the robot head to move
{

	if (shutDown)
		return;

	/*input is:
	pos.y=2;
	pos.z=0;//.*cos(angle);
	pos.x=pos.y/tan(angle);  //scale*2.*sin(angle);
	*/



	//input: xs, ys, zs denotes the positon (xs, ys, zs) of the user detected from the microphone array, the position is based on the sound system  
	// it should be converted into world system, and then converted into robot system
	float xsd, ysd, zsd, xrd, yrd, zrd, xw, yw, zw, xr, yr, zr;
	xsd = 0;
	ysd = 1.6;
	zsd = 1.1;

	xrd = 0.75;
	yrd = 2.45;
	zrd = 0.75;
	//The next three lines convert from sound system to world system
	xw = ys + xsd;
	yw = -xs + ysd;
	zw = zs + zsd;
	//rottheta = 0.5236; 30degrees
	// We suppose the robot rotate 45degrees
	//sqrt(2)=1.4142

	cout<<"World position is: "<<xw<<" "<<yw<<" "<<zw<<" \n";
	//The next three lines are from world system to robot system
	xr = 1.4142/2*(xw + yw - xrd - yrd);
	yr = 1.4142/2*(-xw + yw + xrd - yrd);
	zr = zw - zrd;

	cout<<"In Robot space, user position is: "<<xr<<" "<<yr<<" "<<zr<<" \n";

	animToBePlayed++;
	//myWrapper->stopAnim();
	float neckmax = 16.5*3.1415926/180;
	float neckmin = -16.5*3.1415926/180;
	float cha10 = 128;
	cout << "neckmax is" << neckmax <<"\n" << "neckmin is" << neckmin <<"\n";
	float theta=0;

	theta = -atan(xr/yr);

	cout << "SOUND theta" << theta <<"\n";

	if (theta > neckmax)
	{
		theta = neckmax;
	}

	if (theta < neckmin)
	{
		theta = neckmin;
	}

	cout << "theta between max and min: " << theta <<"\n";

	cha10 = 255*(theta-neckmin)/(neckmax-neckmin);
	//mut->WaitOne();

	/*
	body->at(7).updateDefaultPos(60); // used to gaze at ground (jason, only for head, verify point towards target)
	body->at(8).updateDefaultPos(60);
	*/

	body->at(9).addFrame((unsigned char) myround(cha10));
	body->at(9).updateDefaultPos((unsigned char) myround(cha10));
}


void NadineControl::pointToward(float xs, float ys, float zsInput)//data from sound coordinate, convert it into robot coordinate, and control the robot arm to move
{
	/*float xsd, ysd, zsd, xrd, yrd, zrd, xw, yw, zw, xr, yr, zr;
	float armPointLength, x, y, z, zs;
	String ^command;
	bool inRange=false;
	if(zsInput==0)
		zs=0.7;
	else
		zs=zsInput;

	xsd = 0;// the center of microphone array (xsd,ysd,zsd)
	ysd = 1.6;
	zsd = 1.1;

	xrd = 0.75;//the neck point in the world coordinate frame (xrd,yrd,zrd)
	yrd = 2.45;
	zrd = 0.75;
	//The next three lines convert from sound system to world system
	xw = ys + xsd;
	yw = -xs + ysd;
	zw = zs + zsd;
	//rottheta = 0.5236; 30degrees
	// We suppose the robot rotate 45degrees
	//sqrt(2)=1.4142

	//The next three lines are from sound system to robot system
	xr = 1.4142/2*(xw + yw - xrd - yrd);
	yr = 1.4142/2*(-xw + yw + xrd - yrd);
	zr = zw - zrd;


	float neckmax = 16.5*3.1415926/180;
	float neckmin = -16.5*3.1415926/180;
	float cha10 = 128;
	cout << "neckmax is" << neckmax <<"\n" << "neckmin is" << neckmin <<"\n";
	float theta=0;

	theta = -atan(xr/yr);

	cout << "SOUND theta" << theta <<"\n";

	armPointLength = 0.3;

	cout<<"user distance" << sqrt(xr*xr + yr*yr) << "\n";
	cout<<"xr = "<<xr <<" \n";
	cout<<"yr = "<<yr << "\n";

	/*
	x = armPointLength/sqrt(xr*xr + yr*yr)*xr; // compute the hand position according to the user position (sound)
	y = armPointLength/sqrt(xr*xr + yr*yr)*yr;
	z = -0.08;
	\*Comment stopped here

	cout<<"theta = "<<theta << "\n";
	x =  -0.1+armPointLength*sin(theta); // compute the hand position according to the user position (sound)
	//y = -armPointLength*cos(theta);
	
	y = (0.19*x - 0.0401)/0.21; // let the y increase with x
	//y = (0.37*x - 0.0073)/0.19; // let the y increase with x in a narrow range

	//y = -0.3;
	z = -0.08;

	cout<<"hand distance" << sqrt(x*x + y*y) << "\n";

	cout<<"x = "<<x <<" \n";
	cout<<"y = "<<y << "\n";
	cout<<"z = "<<z << "\n";

	if (x>-0.012)
	{
		x = -0.012;
	}

	if (x<-0.2)
	{
		x=-0.2;
	}


	if (y > -0.18)
	{
		y= -0.18;
	}
	if (y < -0.37)
	{
		y = -0.37;
	}

	if (z > 0.07)
	{
		z = 0.07;
	}
	if (z < -0.2)
	{
		z = -0.2;
	}

	cout<<"cut x = "<<x <<" \n";
	cout<<"cut y = "<<y << "\n";
	cout<<"cut z = "<<z << "\n";

	if (x <= -0.01 && x>= -0.22)
	//if (x>-0.22&&x<-0.01&&y>-0.39&&y<-0.2&&z>-0.2&&z<0.07)
	{
		cout<<"Changing folder \n";
		cout<<"Starting MAtlbal calculation   << \n";
		matlab->Execute("cd D:/IMI-Project/i2p_Nadine_Control/matlabPointToward");
		command="pointTowardC=pointToward("+x+","+y+","+z+")";

		cout<<"xyz in robot coordinate sent to the matlab: \n" ;
		cout<<"x:"<<x;
		cout<<",  y:"<<y;
		cout<<"  z:"<<z<<"\n";

		matlab->Execute(command);
		cout<<"MAtlab calculation finished \n";
		inRange=true;
	} 
	else
	{
		cout<<"Out of range<< \n";
	}
	

	cout << x <<"  "<<y<<"   "<<z<<"\n";
	
	
	if(inRange)
	{
		int animLength=81;

		System::Array ^resa14=gcnew array<double>(animLength);
		System::Array ^resb14=gcnew array<double>(animLength);
		System::Array ^resa16=gcnew array<double>(animLength);
		System::Array ^resb16=gcnew array<double>(animLength);
		System::Array ^resa18=gcnew array<double>(animLength);
		System::Array ^resb18=gcnew array<double>(animLength);
		System::Array ^resa20=gcnew array<double>(animLength);
		System::Array ^resb20=gcnew array<double>(animLength);
		System::Array ^resa22=gcnew array<double>(animLength);
		System::Array ^resb22=gcnew array<double>(animLength);
		System::Array ^resa24=gcnew array<double>(animLength);
		System::Array ^resb24=gcnew array<double>(animLength);
		System::Array ^resa26=gcnew array<double>(animLength);
		System::Array ^resb26=gcnew array<double>(animLength);
		System::Array ^resa15=gcnew array<double>(animLength);
		System::Array ^resb15=gcnew array<double>(animLength);
		System::Array ^resa17=gcnew array<double>(animLength);
		System::Array ^resb17=gcnew array<double>(animLength);
		System::Array ^resa19=gcnew array<double>(animLength);
		System::Array ^resb19=gcnew array<double>(animLength);
		System::Array ^resa21=gcnew array<double>(animLength);
		System::Array ^resb21=gcnew array<double>(animLength);
		System::Array ^resa23=gcnew array<double>(animLength);
		System::Array ^resb23=gcnew array<double>(animLength);
		System::Array ^resa25=gcnew array<double>(animLength);
		System::Array ^resb25=gcnew array<double>(animLength);
		System::Array ^resa27=gcnew array<double>(animLength);
		System::Array ^resb27=gcnew array<double>(animLength);

		matlab->Execute("Channel15=pointTowardC(:,2)");
		matlab->GetFullMatrix("Channel15","base",resa15,resb15);

		matlab->Execute("Channel17=pointTowardC(:,4)");
		matlab->GetFullMatrix("Channel17","base",resa17,resb17);

		matlab->Execute("Channel19=pointTowardC(:,6)");
		matlab->GetFullMatrix("Channel19","base",resa19,resb19);

		matlab->Execute("Channel21=pointTowardC(:,8)");
		matlab->GetFullMatrix("Channel21","base",resa21,resb21);

		matlab->Execute("Channel23=pointTowardC(:,10)");
		matlab->GetFullMatrix("Channel23","base",resa23,resb23);
		
		matlab->Execute("Channel25=pointTowardC(:,12)");
		matlab->GetFullMatrix("Channel25","base",resa25,resb25);

		matlab->Execute("Channel27=pointTowardC(:,14)");
		matlab->GetFullMatrix("Channel27","base",resa27,resb27);

		double temp=0;
		//channels[14]=gcnew array<int>(animLength);
		std::vector<unsigned char> *tmpChannel=new vector<unsigned char>();
		
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa15->GetValue(i);
			//channels[14][i]=(int)temp15;
			tmpChannel->push_back(temp);

		}
		body->at(14).addTrajectory(*tmpChannel);
		tmpChannel->clear();

		//channels[16]=gcnew array<int>(animLength);
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa17->GetValue(i);
			//channels[16][i]=(int)temp17;
			tmpChannel->push_back(temp);
		}
		body->at(16).addTrajectory(*tmpChannel);
		tmpChannel->clear();

		//channels[18]=gcnew array<int>(animLength);
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa19->GetValue(i);
			//channels[18][i]=(int)temp19;
			tmpChannel->push_back(temp);
		}
		body->at(18).addTrajectory(*tmpChannel);
		tmpChannel->clear();

		//channels[20]=gcnew array<int>(animLength);
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa21->GetValue(i);
			//channels[20][i]=(int)temp21;
			tmpChannel->push_back(temp);
		}
		body->at(20).addTrajectory(*tmpChannel);
		tmpChannel->clear();

		//channels[22]=gcnew array<int>(animLength);
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa23->GetValue(i);
			//channels[22][i]=(int)temp23;
			tmpChannel->push_back(temp);
		}
		body->at(22).addTrajectory(*tmpChannel);
		tmpChannel->clear();

		//channels[24]=gcnew array<int>(animLength);
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa25->GetValue(i);
			//channels[24][i]=(int)temp25;
			tmpChannel->push_back(temp);
		}
		body->at(24).addTrajectory(*tmpChannel);
		tmpChannel->clear();

		//channels[26]=gcnew array<int>(animLength);
		for(int i=0;i<animLength;i++)
		{
			temp =(double)resa27->GetValue(i);
			//channels[26][i]=(int)temp27;
			tmpChannel->push_back(temp);
		}
		body->at(26).addTrajectory(*tmpChannel);
		tmpChannel->clear();


	}*/
}

bool NadineControl::shakeTimeInterval(int aTime)
{
	time_t temp=time(NULL);
	return temp-lastAction>aTime;
}

void NadineControl::initBody()
{
	channel1=new Joint(128);
	//channel1=new Joint(10);
	channel2=new Joint(128);
	channel3=new Joint(128);
	channel4=new Joint(128);
	channel5=new Joint(0);
	channel6=new Joint(0);
	channel7=new Joint(0);
	channel8=new Joint(240);
	channel9=new Joint(240);
	channel10=new Joint(128);
	channel11=new Joint(128);
	channel12=new Joint(128);
	channel13=new Joint(128);
	channel14=new Joint(130);
	channel15=new Joint(128);
	channel16=new Joint(65);
	channel17=new Joint(55);
	channel18=new Joint(109);
	channel19=new Joint(135);
	channel20=new Joint(80);
	channel21=new Joint(50);
	channel22=new Joint(110);
	channel23=new Joint(120);
	channel24=new Joint(70);
	channel25=new Joint(30);
	channel26=new Joint(50);
	channel27=new Joint(58);
	channel28=new Joint(128);

	body->push_back(*channel1);
	body->push_back(*channel2);
	body->push_back(*channel3);
	body->push_back(*channel4);
	body->push_back(*channel5);
	body->push_back(*channel6);
	body->push_back(*channel7);
	body->push_back(*channel8);
	body->push_back(*channel9);
	body->push_back(*channel10);
	body->push_back(*channel11);
	body->push_back(*channel12);
	body->push_back(*channel13);
	body->push_back(*channel14);
	body->push_back(*channel15);
	body->push_back(*channel16);
	body->push_back(*channel17);
	body->push_back(*channel18);
	body->push_back(*channel19);
	body->push_back(*channel20);
	body->push_back(*channel21);
	body->push_back(*channel22);
	body->push_back(*channel23);
	body->push_back(*channel24);
	body->push_back(*channel25);
	body->push_back(*channel26);
	body->push_back(*channel27);
	body->push_back(*channel28);

	changePosture("poseGerman");
}

string NadineControl::timeConversion(int i)//00:00:00.00
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

