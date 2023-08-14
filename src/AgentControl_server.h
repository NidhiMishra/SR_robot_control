//#include "stdafx.h"
#pragma managed
#include <vcclr.h>
#include "AgentControl.h"
#include "NadineControl.h"
#include <string>
#include "WorldClient.h"
//using namespace System::Threading;
//#using <system.dll>

//#pragma once

#include <iostream>  
#include <boost/thread.hpp>  
#include <boost/date_time.hpp>


//using namespace System::Threading;
using namespace  ::imi;
using namespace std;


/*
void startLookTargetAAAAA(bool targetSet, string target,gcroot<NadineControl^> robot)
{
//std::cout<<("lookAtTarget 0\n");
while(targetSet)
{ 
std::cout<<("lookAtTarget 1\n");
imi::Location& mLocation = imi::Location();
std::cout<<("lookAtTarget2\n");
queryFromWorldClient(mLocation, target);

std::cout<<("lookAtTarget3\n");
robot->lookAtPosition(mLocation.location.x,mLocation.location.y,mLocation.location.z);
std::cout<<("lookAtTarget4\n");
robot->playAnimation(100);

std::cout<<("lookAtTarget5\n");
System::Threading::Thread::Sleep(500);
std::cout<<("lookAtTarget6\n");
}
//std::cout<<("lookAtTarget 0\n");
}*/


class AgentControlHandler : virtual public AgentControlIf {
public:
	gcroot<NadineControl^> robot;
	string target;
	bool targetSet;
	int alreadyShaked;
	gcroot<Thread^> newThread; 

	double rPleasure;
	double rArousal;
	double rDominance;


	AgentControlHandler(char * voiceFilePath,char * voiceFilePathGerman, char * voiceFilePathFrench, char * animationXMLPath, std::string worldClientIP, std::string feedbackServerIP)
	{
		// Your initialization goes here
		if (feedbackServerIP.empty())
			robot=gcnew NadineControl(voiceFilePath,voiceFilePathGerman, voiceFilePathFrench);
		else
			robot=gcnew NadineControl(voiceFilePath, voiceFilePathGerman,voiceFilePathFrench, feedbackServerIP.c_str());
		if (!worldClientIP.empty())
			robot->setWorldClientIP(worldClientIP);
		robot->loadAnimationLibrary(animationXMLPath);
		robot->initMe(true);
		robot->setFeedbackServerIP(feedbackServerIP);
		
		alreadyShaked=0;
		targetSet=false;
		target="";
		//newThread = gcnew Thread(gcnew ThreadStart(this, &startLookTargetAAAAA(targetSet,target,robot)));
		//newThread->Start();
		rPleasure = 0;
		rDominance = 0;
		rArousal = 0;
	//	robot->changePosture("poseGerman");
		std::cerr << "AgentControlHandler Initialized";
	}

	//private:


protected:

	//checkerWrapper ^Mychecker;
	Status::type lookAtPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		//return Status::SUCCESS;
		//printf("lookAtPosition\n");
		//printf("%f %f %f \n",position.x,position.y,position.z);
		//cout<<"postion received: "<<position.x<<" "<<position.y<<" "<<position.z;
		/*
		if(position.y==0)
		{
		robot->hToVoice(position.x,position.y,position.z);
		printf("A\n");
		}
		else
		{
		robot->lookAtPosition(position.x,position.y,position.z);
		printf("B\n");
		}*/

		if(position.z==-1)//This is just for testing. if z==-1 I assume it is an eye movement request
		{
			robot->moveEyes(position.x,position.y);
			//printf("E\n");
		}
		else    
		{
			if(position.z==0)
			{
				robot->hToVoice(position.x,position.y,position.z);
				//robot->pointToward(position.x,position.y,position.z);/*jason, only for experiment, Point toward sound*/
				//printf("A\n");
			}
			else
			{
				robot->lookAtPosition(position.x,position.y,position.z);
				//printf("B\n");
			}
		}

		return Status::SUCCESS;
	}

	Status::type lookAtTarget(const  ::imi::TargetName& Atarget)
	{
		// Your implementation goes here
// 		target=Atarget;
// 		if(!targetSet)
// 		{
// 			targetSet=true;
// 
// 		}
		//robot->gazeTarget(true,target);
		robot->lookAtTarget(Atarget);

		std::cout<<("lookAtTarget:"+Atarget+"\n");
		return Status::SUCCESS;
	}


protected:
	void endLookAt() {
		// Your implementation goes here
		// robot->gazeTarget(false,"");
		robot->endLookAt();
		printf("endLookAt\n");
	}

	Status::type pointAtPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		printf("pointAtPosition\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type pointAtTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("pointAtTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	void endPointAt() {
		// Your implementation goes here
		printf("endPointAt\n");
	}

	Status::type speak(const std::string& phrase, const double volume) {
		string temp;
		temp = phrase;
		if (rPleasure < -0.2)//angry voice
		{
			temp = "<voice emotion = 'cross'>" + temp + "</voice>";
		}
		if (rPleasure>0.2)//happy voice
		{
			temp = "<voice emotion = 'happy'>" + temp + "</voice>";
		}
		robot->makeNadineSpeak(temp, volume);
		printf("Nadine is speaking \n");
		return Status::SUCCESS;//To do: Check for failure
	}

	Status::type greetPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		printf("greetPosition\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type greetTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("greetTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type playAnimation(const Animation::type animation) {
		// Your implementation goes here
		//printf(to_string(animation));
		switch (animation)
		{
		case imi::Animation::WHY:
//			robot->playAnimation(4);
			robot->playAnimation("HeadUP_TwoArmGesture");
			printf("why\n");
			break;
		case imi::Animation::WAVE_HAND://WAVE_HAND==3?
//			robot->playAnimation(5);
			robot->playAnimation("LOOKUP_Waving");
			printf("Wave hand\n");
			break;
		case imi::Animation::NOD_YES://WAVE_HAND==3?
//			robot->playAnimation(1);
			robot->playAnimation("nod");
			printf("Nod Yes\n");
			break;
		case imi::Animation::NOD_NO://WAVE_HAND==3?
//			robot->playAnimation(2);
			robot->playAnimation("shake");
			printf("Head Shake \n");
			break;
		case imi::Animation::OFFER:
//			robot->playAnimation(7);
			robot->playAnimation("LOOKUP_TwoArmGesture");
			printf("offer sit\n");
			break;
		case imi::Animation::RIGHT_SWEEP: //this is supposed to be thanks. To be added in thrift def
//			robot->playAnimation(6);
			robot->playAnimation("LOOKUP_OneArmGesture");
			printf("offer \n");
			break;
		case imi::Animation::RELEASE_OBJECT: //This is for reading animation
//			robot->playAnimation(12);
			if (robot->lookingUp){
				robot->playAnimation("Idle_Transition");
				printf("idle transition! \n");
				sleep(3000);
			}
			robot->playAnimation("Laptop_Default_idle");
			//printf("idle animation! \n");
			break;
		default:
//			robot->playAnimation(100);
			printf("Error: Animation Undefined\n");
			break;
		}

		//printf(animation+"\n");
		return Status::SUCCESS;//To do: Check for failure
	}

	Status::type moveDistance(const  ::imi::Vec3& distance) {
		// Your implementation goes here
		printf("moveDistance\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type moveToPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		printf("moveToPosition\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type moveToTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("moveToTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	bool isAgentMoving() {
		// Your implementation goes here
		printf("isAgentMoving\n");
		return false;
	}

	Status::type endMove() {
		// Your implementation goes here
		printf("endMove\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type touchPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		/* float gx=position.x;
		float gy=position.y;
		float gz=position.z;

		float dur;
		float lengtharm;
		dur = sqrt((gx-(-0.1766))*(gx-(-0.1766))+(gy)*(gy)+(gz-0.2927)*(gz-0.2927));// the bound
		lengtharm = 0.691;
		/* if (dur>lengtharm)
		{
		printf("The user's hand is out of the range of the robot!");
		robot->makeNadineSpeak("Sorry, I can't touch your hand. Please come a bit closer");
		robot->playAnimation(100);
		}

		else
		{*/
		//robot->shakeHand(gx,gy,gz);
		robot->shakeHand(-0.2096,-0.3871,0.1518);
		printf("touchPosition\n");
		//}
		return Status::SUCCESS;
	}

	Status::type touchTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		robot->playAnimation(target);
		//if(alreadyShaked==0)
		//robot->shakeHand(-0.002541,-0.3871,0.2);
		printf("touchTarget\n");
		return Status::SUCCESS;
	}

	Status::type graspTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		//robot->playAnimation(100);
		printf("graspTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	void endGrasp() {
		// Your implementation goes here
		printf("endGrasp\n");
	}

	Status::type shakeHands(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("shakeHands\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type sitAtPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		printf("sitAtPosition\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type sitOnTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("sitOnTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type endSit() {
		// Your implementation goes here
		printf("endSit\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type acknowledgePosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		printf("acknowledgePosition\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type acknowledgeTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("acknowledgeTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type beckonPosition(const  ::imi::WorldPosition& position) {
		// Your implementation goes here
		printf("beckonPosition\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type beckonTarget(const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("beckonTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type setFaceExpression(const  ::imi::Facial_Expression::type newExpression, const double intensity) {
		// Your implementation goes here
		if(newExpression==imi::Facial_Expression::HAPPINESS)
		{
			robot->playAnimation("happy");
			return Status::SUCCESS;
		}	
		else
			return Status::METHOD_NOT_YET_IMPLEMENTED;
		printf("setFaceExpression\n");
		
	}

	Status::type setEmotionalState(const  ::imi::Emotion::type newEmotion, const double intensity) {
		// Your implementation goes here
		printf("setEmotionalState\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type setEmotionalStateTowardTarget(const DirectedEmotion::type newDirectedEmotion, const double intensity, const  ::imi::TargetName& target) {
		// Your implementation goes here
		printf("setEmotionalStateTowardTarget\n");
		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

	Status::type setMood(const double valence, const double arousal, const double dominance) {
		// Your implementation goes here
		printf("setMood\n");

		cout << "received: P: " << valence << " A: " << arousal << " D: " << dominance << "\n";

		rPleasure = valence;
		rDominance = dominance;
		rArousal = arousal;

		return Status::METHOD_NOT_YET_IMPLEMENTED;
	}

};
/*
int main(int argc, char **argv) {
int port = 9090;
shared_ptr<AgentControlHandler> handler(new AgentControlHandler());
shared_ptr<TProcessor> processor(new AgentControlProcessor(handler));
shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
server.serve();
return 0;
}

*/