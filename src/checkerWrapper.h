//#import "checker.tlb" raw_interfaces_only

#pragma once
#using <system.dll>
#include <string>
#include "NadineAnimations.h"

using namespace System;
//using namespace Checker;
using namespace System::Timers;

ref class checkerWrapper
{
public:
	checkerWrapper();
	void initialisation(void);
	void playLipSync(std::string lipAnimation);
	void startTimer(void);
	void playAnim(String ^anim);
	void stopAnim();
	static void OnTimedEvent( Object^ source, ElapsedEventArgs^ e );
	//static path1 ^checker;
	
private:
	static Timer ^aTimer;
	Animations ^anim;
   
	
};

