#pragma once
#include <string>
#include "structDef.h"
#include "LipAnimationGenerator.h"
#include "cerevoice_aud.h"
#include "cerevoice_eng.h"

using namespace std;


typedef struct user_data {
    CPRC_sc_player * player;
    /* Add other user-specific settings here */
} user_data;

class Cervevoice
{
public:
	Cervevoice(void);
	Cervevoice(char * voicePath, char* voicePathGerman, char* voicePathFrench);
	~Cervevoice(void);
	
	
	lip_position * processString(string text, bool language);
	lip_position * channel_callback(CPRC_abuf * abuf, void * userdata);

	int getAnimSize();
	//void channel_callbackBackUp(CPRC_abuf * abuf, void * userdata);
	lip_position * processStringGerman(string text);
	CPRCEN_engine * engGerman;
	CPRCEN_channel_handle chanGerman;

	lip_position * processStringFrench(string text);
	CPRCEN_engine * engFrench;
	CPRCEN_channel_handle chanFrench;

	CPRCEN_engine * eng;
	CPRCEN_channel_handle chan;
	CPRC_abuf * abuffer;
	CPRC_sc_player * player;
	LipAnimationGenerator *generator;
	
};

