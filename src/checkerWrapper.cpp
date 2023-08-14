#include "StdAfx.h"
#include <iostream>
#include "checkerWrapper.h"
#using <System.Windows.Forms.dll>
#using <System.dll>
#using <System.Drawing.dll>
#include <string>

//using namespace Checker;
using namespace System;
using namespace System::Timers;
using namespace std;

checkerWrapper::checkerWrapper()
{
	//anim=anAnimations;
//	checker=gcnew path1();

	//aTimer = gcnew Timer( 10000 );
//	checker->comm->Receive();
	//aTimer->Elapsed += gcnew ElapsedEventHandler( checkerWrapper::OnTimedEvent );
	//aTimer->Interval = 2000;
    //aTimer->Enabled = true;
//	checker.startForm();
	
}

void checkerWrapper::playAnim(String ^anim)
{
	//checker->playAnim(anim);
}

void checkerWrapper::stopAnim()
{
	//checker->stopAnim();
}

void checkerWrapper::OnTimedEvent( Object^ source, ElapsedEventArgs^ e )
{
	cout << "timer ticked \n";
	//checker->comm.Receive();
	//checker->playAnim("");
	//nextFrame();;
//	checker->playAnim(anim-getAnimation(3));
	//nextFrame();
}
void checkerWrapper::initialisation()
{
	//checker->startForm();
}

void checkerWrapper::startTimer()
{
	//checker->startTimer();
}

void checkerWrapper::playLipSync(std::string lipAnimation)
{
	int size;
	signed char *buffer=new signed char[lipAnimation.size()];
	String ^lipAnimConv=gcnew String(lipAnimation.c_str());

	size=lipAnimation.size();
	//checker->playLipSync(lipAnimConv);
}