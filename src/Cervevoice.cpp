#pragma once
#include "StdAfx.h"
#include <iostream>
#include "structDef.h"
#include "Cervevoice.h"
#include "cerevoice_aud.h"
#include "cerevoice_eng.h"
#include "LipAnimationGenerator.h"
using namespace std;

Cervevoice::Cervevoice(void)
{

}

Cervevoice::Cervevoice(char* voicePath,char* voicePathGerman,char* voicePathFrench)
{
	char voiceFullPath[2046];
	char licenseFullPath[2046];

	sprintf(voiceFullPath, "%s.voice", voicePath);
	sprintf(licenseFullPath, "%s.lic", voicePath);

	eng = CPRCEN_engine_load(licenseFullPath, voiceFullPath);
	chan=CPRCEN_engine_open_default_channel(eng);
	//player= CPRC_sc_player_new(8000);
	//CPRCEN_engine_set_callback(eng,chan, (void *)NULL, &Cervevoice::channel_callback);
	//CPRCEN_engine_channel_to_file(eng,chan,"../lipAnim.wav",CPRCEN_RIFF);//Direct channel to a wav file 
	
	if(eng!=NULL)
	{
		cout << "Init of Cerevoice component successfull \n";
	}
	else
	{
		cout << "Init of Cerevoice component failed \n";
	}
	//eng =CPRCEN_engine_new();
	//**********************GERMAN*********************//
	char voiceFullPathGerman[2046];
	char licenseFullPathGerman[2046];

	sprintf(voiceFullPathGerman, "%s.voice", voicePathGerman);
	sprintf(licenseFullPathGerman, "%s.lic", voicePathGerman);

	engGerman = CPRCEN_engine_load(licenseFullPathGerman, voiceFullPathGerman);
	chanGerman=CPRCEN_engine_open_default_channel(engGerman);

	if(engGerman!=NULL)
	{
		cout << "Init of Cerevoice German component successfull \n";
	}
	else
	{
		cout << "Init of Cerevoice German component failed \n";
	}
	//******************FRENCH*************************//

	char voiceFullPathFrench[2046];
	char licenseFullPathFrench[2046];

	sprintf(voiceFullPathFrench, "%s.voice", voicePathFrench);
	sprintf(licenseFullPathFrench, "%s.lic", voicePathFrench);

	engFrench = CPRCEN_engine_load(licenseFullPathFrench, voiceFullPathFrench);
	chanFrench=CPRCEN_engine_open_default_channel(engFrench);

	if(engFrench!=NULL)
	{
		cout << "Init of Cerevoice French component successfull \n";
	}
	else
	{
		cout << "Init of Cerevoice French component failed \n";
	}
	//*******************************************//
	//Initialisation of LipAnim
	generator=new LipAnimationGenerator();

}


Cervevoice::~Cervevoice(void)
{
	CPRCEN_engine_delete(eng);
	CPRCEN_engine_delete(engGerman);
	CPRCEN_engine_delete(engFrench);
}

lip_position * Cervevoice::processString(string text, bool language)
{
	CPRCEN_engine_channel_to_file(eng,chan,"../lipAnim.wav",CPRCEN_RIFF);//Direct channel to a wav file 
	const char * textc=text.c_str();
	generator->German_mode = language;
	//cout << textc;
	if(eng!=NULL)
	{
		//CPRCEN_engine_speak_to_file(eng,textc,"../../out.wav");//create speech into wav file
		CPRCEN_engine_clear_callback(eng, chan);
		abuffer=CPRCEN_engine_channel_speak(eng,chan,textc,strlen(textc),1);
		return(channel_callback(abuffer, NULL));
	}
	else
	{
		cout << "not initialised. Doing nothing \n";
		return NULL;
	}
}

lip_position * Cervevoice::processStringGerman(string text)
{
	CPRCEN_engine_channel_to_file(engGerman,chanGerman,"../lipAnim.wav",CPRCEN_RIFF);//Direct channel to a wav file 
	const char * textc=text.c_str();
	generator->German_mode = true;
	//cout << textc;
	if(engGerman!=NULL)
	{
		//CPRCEN_engine_speak_to_file(eng,textc,"../../out.wav");//create speech into wav file
		CPRCEN_engine_clear_callback(engGerman, chanGerman);
		abuffer=CPRCEN_engine_channel_speak(engGerman,chanGerman,textc,strlen(textc),1);
		return(channel_callback(abuffer, NULL));
	}
	else
	{
		cout << "not initialised. Doing nothing \n";
		return NULL;
	}
}

lip_position * Cervevoice::processStringFrench(string text)
{
	CPRCEN_engine_channel_to_file(engFrench,chanFrench,"../lipAnim.wav",CPRCEN_RIFF);//Direct channel to a wav file 
	const char * textc=text.c_str();
	generator->German_mode = true;
	//cout << textc;
	if(engFrench!=NULL)
	{
		//CPRCEN_engine_speak_to_file(eng,textc,"../../out.wav");//create speech into wav file
		CPRCEN_engine_clear_callback(engFrench, chanFrench);
		abuffer=CPRCEN_engine_channel_speak(engFrench,chanFrench,textc,strlen(textc),1);
		return(channel_callback(abuffer, NULL));
	}
	else
	{
		cout << "not initialised. Doing nothing \n";
		return NULL;
	}
}

lip_position * Cervevoice:: channel_callback(CPRC_abuf * abuf, void * userdata)
{
	/*CPRC_sc_player * player=CPRC_sc_player_new(48); //This does not work due to bad linkage (Aryel)
	CPRC_sc_audio * buf = CPRC_sc_audio_short_disposable(CPRC_abuf_wav_data(abuf), CPRC_abuf_wav_sz(abuf));

	CPRC_sc_audio_cue(player,buf);*/
	const CPRC_abuf_trans * trans;
	int name;
	float start, end;
	const char * phoneme;
	anim_data temp;

	for(int i=0;i<CPRC_abuf_trans_sz(abuf);i++)
	{
		trans=CPRC_abuf_get_trans(abuf, i);
		start=CPRC_abuf_trans_start(trans);
		end=CPRC_abuf_trans_end(trans);
		name=CPRC_abuf_trans_type(trans);
		//phoneme=CPRC_abuf_trans_phone(trans);
		//phoneme=CPRC_ABUF_TRANS_PHONE(trans);

		//in this version, we only extract phonemes and words...
		if(name==CPRC_ABUF_TRANS_PHONE)
		{
			phoneme=trans->phone;
			//if(end-start>0.1)
			//	cout << " \n Start=" << start << " end=" << end << " PHONEME = " << trans->name << "\n"; //<<  CPRC_abuf_trans_type(trans) << "\n";
			temp.start=start;
			temp.end=end;
			temp.phone=trans->name;
			generator->addPhoneme(temp);
		}
		else if(name==CPRC_ABUF_TRANS_WORD)
		{
			//cout << " \n Start=" << start << " end=" << end << " Type= WORD \n";// <<  CPRC_abuf_trans_type(trans) << "\n";
		}
	}
	generator->generateAnimation();
	return(generator->lipAnimation);
	
}
int Cervevoice::getAnimSize()
{
	return generator->frameCounter;
}

