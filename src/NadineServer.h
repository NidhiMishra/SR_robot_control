#pragma once
#include "stdafx.h"
#include <string>

class NadineServer
{
public:
	NadineServer(void);
	NadineServer(char* voicePath, char* voicePathGerman, char* voicePathFrench);
	~NadineServer(void);
	void setWorldClientIP( std::string ip );
	void setFeedbackServerIP( std::string ip );
	void setAnimationXMLPath( char * animationXMLPath);
	void startServer(void);
	void stopServer(void);
private:
	bool continueToServe;
	char * voicePath;
	char* voicePathGerman;char* voicePathFrench;
	char * animationXMLPath;
	std::string worldClientIP;
	std::string feedbackServerIP;
};

