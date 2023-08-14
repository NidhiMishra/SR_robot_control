#include "Joint.h"
#include <iterator>

Joint::Joint(unsigned char aDefaultPos)
{
	//id=aId;
	defaultPos=aDefaultPos;
	state=0;//idle
}


Joint::~Joint(void)
{

}

void Joint::addFrame(unsigned char aFrame)
{
	state=1;
	trajectory.push_back(aFrame);
}

void Joint::addIdleMovement(std::vector<unsigned char> aTrajectory)
{
	if(state==0)
	{
		for(int i=0;i<aTrajectory.size();i++)
			trajectory.insert(trajectory.begin(),aTrajectory.at(i));
		state=2;//Moving
	}

}


void Joint::addTrajectory(std::vector<unsigned char> aTrajectory)//, int size)
{
	
	trajectory.clear(); //This is a temporary solution to avoid strange blending 

	// the following will have memory access problem somehow
// 	for(int i=0;i<aTrajectory.size();i++)
// 		trajectory.insert(trajectory.begin(),aTrajectory.at(i));

	// change to use this one line assignment,
	// note rbegin() means a reverse iterator pointing to the last element
	// note rend() means a reverse iterator pointing to the first element
	trajectory.assign(aTrajectory.rbegin(), aTrajectory.rend());

	state=1;//Moving
}

void Joint::clearTrajectory()
{
	trajectory.clear();
}

unsigned char Joint::getFrame()
{
	unsigned char currentFrame=defaultPos;
	if(trajectory.size()>0)
	{
		currentFrame=trajectory.back();
		trajectory.pop_back();
	}
	else
	{
		state=0;//idle
	}
	return currentFrame;
}

void Joint::updateDefaultPos(unsigned char aPos)
{
	defaultPos=aPos;
	if(int(aPos)>255)
		defaultPos=255;
	if(int(aPos)<0)
		defaultPos=0;
}
