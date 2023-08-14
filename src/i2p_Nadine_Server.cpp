#include "stdafx.h"
#using <system.dll>
#include "NadineServer.h"
#include "WorldClient.h"

using namespace std;

void about(int errorCode)
{
	// print about
	fprintf(stdout, "PARAMETERS: [] means this parameter is optional\n"
					"\t-voicepath <folder containing voice files>\\<voice file header>\n"
					"\t-animationXMLPath <folder containing animation XML files>\n"
					"\t[-worldClientIP] NNN.NNN.NNN.NNN\n"
					"\t[-feedbackServerIP] NNN.NNN.NNN.NNN\n");
	exit(errorCode);
}


int _tmain(int argc, _TCHAR* argv[])
{	
	
	//////////////////////////////////////
	// Define variable needed


	char* voicePathFrench = new char[2046];
	char* voicePathGerman = new char[2046];
	char* voicePath = new char[2046];
	char* animationXMLPath = new char[2046];
	char* worldClientIP = new char[20];
	char* feedbackIP = new char[20];
	worldClientIP[0] = '\0';
	feedbackIP[0] = '\0';
	animationXMLPath[0] = '\0';
	bool feedback=false;
	//////////////////////////////////////
	// Read arguements
	
	int totalArgc = 15;
	if ( argc > totalArgc )
	{
		about(1);
	}

	int next = 1;

	for ( int next = 1 ; next < argc ; next+=2 )
	{
		if ( strcmp(argv[next] , "-voicepath") == 0 )
		{
			strcpy(voicePath, argv[next+1]);
			fprintf(stdout,"\t-voicepath %s\n", voicePath);
		}
		else
		if ( strcmp(argv[next] , "-voicepathGerman") == 0 )
		{
			strcpy(voicePathGerman, argv[next+1]);
			fprintf(stdout,"\t-voicepath %s\n", voicePathGerman);
		}
		
		else
		if ( strcmp(argv[next] , "-voicepathFrench") == 0 )
		{
			strcpy(voicePathFrench, argv[next+1]);
			fprintf(stdout,"\t-voicepathFrench %s\n", voicePathFrench);
		}
		else
		if ( strcmp(argv[next] , "-animationXMLPath") == 0 )
		{
			strcpy(animationXMLPath, argv[next+1]); 
			fprintf(stdout,"\t-animationXMLPath %s\n", animationXMLPath);
		}
		else
		if ( strcmp(argv[next] , "-worldClientIP") == 0 )
		{
			strcpy(worldClientIP, argv[next+1]);
			fprintf(stdout,"\t-worldClientIP %s\n", worldClientIP);
		}
		else
		if ( strcmp(argv[next] , "-feedbackServerIP") == 0 )
		{
			strcpy(feedbackIP, argv[next+1]);
			fprintf(stdout,"\t-feedbackServerIP %s\n", feedbackIP);
			feedback=true;
		}
		else
		{
			about(2);
		}
	}
	
	NadineServer *myServer=new NadineServer(voicePath,voicePathGerman,voicePathFrench);
	fprintf(stderr,"voice initialized [%s]...\n", voicePath);
	myServer->setWorldClientIP(std::string(worldClientIP));
	fprintf(stderr,"worldClientIP set %s...\n", worldClientIP);
	if(feedback)
	{
		myServer->setFeedbackServerIP(std::string(feedbackIP));
		//myServer->setFeedbackServerIP("localhost");
		fprintf(stderr,"feedbackIP set %s...\n", feedbackIP);
	}
	myServer->setAnimationXMLPath(animationXMLPath);
	fprintf(stderr,"animationXMLPath set %s...\n", animationXMLPath);

	

	myServer->startServer();
	
	
	return 0;
}

