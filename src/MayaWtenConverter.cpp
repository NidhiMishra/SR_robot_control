#pragma once
#include "StdAfx.h"
#include "MayaWtenConverter.h"

MayaWtenConverter::MayaWtenConverter(void)
{
	channelToLabel[1]="Eyelid 0 2 0;";
	channelToLabel[2]="Brows 0 2 1;";
	channelToLabel[3]="EyeUD 0 2 2;";
	channelToLabel[4]="EyeLR 0 2 3;";
	channelToLabel[5]="Mouth 0 2 4;";
	channelToLabel[6]="O 0 2 5;";
	channelToLabel[7]="Smile 0 2 6;";
	channelToLabel[8]="NTw1 0 2 7;";
	channelToLabel[9]="NTw2 0 2 8;";
	channelToLabel[10]="NLR 0 2 9;";
	channelToLabel[11]="BTw1 0 2 10;";
	channelToLabel[12]="BTw2 0 2 11;";
	channelToLabel[13]="BLR 0 2 12;";
	channelToLabel[14]="LShldr 0 2 13;";
	channelToLabel[15]="RShldr 0 2 14;";
	channelToLabel[16]="LArm 0 2 15;";
	channelToLabel[17]="RArm 0 2 16;";
	channelToLabel[18]="LSide 0 2 17;";
	channelToLabel[19]="RSide 0 2 18;";
	channelToLabel[20]="LUarm 0 2 19;";
	channelToLabel[21]="RUarm 0 2 20;";
	channelToLabel[22]="LElbow 0 2 21;";
	channelToLabel[23]="RElbow 0 2 22;";
	channelToLabel[24]="LFArm 0 2 23;";
	channelToLabel[25]="RFArm 0 2 24;";
	channelToLabel[26]="LWrist 0 2 25;";
	channelToLabel[27]="RWrist 0 2 26;";
	channelToLabel[28]="CH217 0 2 27;";
}


MayaWtenConverter::~MayaWtenConverter(void)
{
}

map <string,int> MayaWtenConverter::reverse()
{
	map <string, int> result;

	for(int i=0;i<channelToLabel.size();i++)
		result[channelToLabel.at(i+1)]=i;

	return result;
}

int MayaWtenConverter::convert(string path)
{
	string type;//string that contains the extension of the file. txt for wten.
	string processedPath="";
	try
	{
		if(path.front()=='"')
			processedPath=path.substr(1,path.size()-2);
		else
			processedPath=path;

		type=processedPath.substr(processedPath.size()-3, 3);
	}
	catch(exception &e)
	{
		type="";
	}
	if(type=="txt")
		return wtenToMaya(processedPath);
	else
	{
		if(type=="nim")
			return mayaToWten(processedPath);
		else
			return -1;
	}
}

int MayaWtenConverter::mayaToWten(string path)
{
	ifstream file;
	fstream wtenFile;
	LipAnimationGenerator *temp=new LipAnimationGenerator();
	char *line="";
	string lineS="";
	string currentLabel="";
	int currentIndex=0;
	int lineCounter=0;
	string number="";
	map <string, int> labelToChannel=reverse();

	vector <vector<int>> channels;
	channels.resize(28);

	try
	{
		cout <<"trying to open" << path <<"\n";
		file.open(path,fstream::in);
	}
	catch (exception &e)
	{
		cerr << e.what();
	}


	while(getline(file,lineS))
	{
		cout << "\n"+lineS;
		if(lineCounter++<7)
			continue;
		if(lineS.substr(0,5)=="anim ")
		{
			currentLabel=lineS.substr(5, lineS.size()-1);
			lineCounter=0;
			continue;
		}
		if(lineS.back()=='}')//1 128 auto auto 1 1 0;
			continue;
		while(lineS.front()==' ')//remove the spaces
		{
			lineS=lineS.substr(1,lineS.size()-1);
		}
		while(lineS.front()!=' ')//remove the first number
		{
			lineS=lineS.substr(1,lineS.size()-1);
		}
		lineS=lineS.substr(1,lineS.size()-1);//remove the space
		while(lineS.front()!=' ')//get the second number
		{
			number=number+lineS.front();
			lineS=lineS.substr(1,lineS.size()-1);
		}
		try
		{
			currentIndex=labelToChannel.at(currentLabel);
		}
		catch (exception &e)
		{
			cerr << "Problem occured in the map. this " +currentLabel+" is not a valid key \n" << e.what();
			continue;
		}
		channels.at(currentIndex).push_back(atoi(number.c_str()));
		number="";
	}

	wtenFile.open("../outputWten.txt",fstream::out);
	for(int i=0; i<channels.at(0).size();i++)
	{
		wtenFile << temp->timeConversion(i) << ", ";
		int j=0;
		for(int j=0;j<28;j++)
			wtenFile << channels.at(j).at(i) <<",";
		wtenFile << "\n";
	}


	return 0;
}


	


int MayaWtenConverter::wtenToMaya(string path)
{
	ifstream file;
	fstream mayaFile;
	char *line="";//a line from the wten file
	string lineS="";
	string number="";

	vector <vector<int>> channels;
	channels.resize(28);

	try
	{
		cout <<"trying to open" << path <<"\n";
		file.open(path,fstream::in);
	}
	catch (exception &e)
	{
		cerr << e.what();
	}
	while(getline(file,lineS))
	{
		//lineS=line;//00:00:00.00, 72,126,129,132,0,0,0,114,134,135,176,128,135,133,135,42,37,157,165,130,98,160,181,255,255,0,0,127,
		cout << lineS <<"\n" ;
		if(lineS.size()>1)
		{
			lineS=lineS.substr(13,lineS.size()-1);//remove the time

			for(int i=27; i>=0;i--)
			{
				lineS.pop_back();//remove ','
				while((lineS!="")&&(lineS.back()!=','))
				{
					number=lineS.back()+number;
					lineS.pop_back();
				}
				channels.at(i).push_back(atoi(number.c_str()));
				number="";
			}

		}
	}
	file.close();

	mayaFile.open("../outputMaya.anim",fstream::out);
	mayaFile << "animVersion 1.1;\n mayaVersion 2012 x64;\ntimeUnit ntsc;\nlinearUnit cm;\nangularUnit deg;\nstartTime 1;\nendTime "<<channels.at(0).size()<<";";

	for(int i=0;i<27;i++)
	{
		mayaFile<<"\nanim "<<channelToLabel.at(i+1)<<"\n animData {\n input time;\n output unitless;\n weighted 0; \n preInfinity constant;\n postInfinity constant;\n keys {";
		
		for(int j=0;j<channels[i].size();j++)
		{
			mayaFile<<"\n "<<j+1<<" "<<channels.at(i).at(j)<<" auto auto 1 1 0;";
		}
		mayaFile<<"  }\n}";
	}
	mayaFile<< "\nanim nadine_MAIN_CTRLShape 1 0 0;\nanim annotationLocator1 1 2 0;\nanim annotationLocator1Shape 2 0 0;\nanim annotation1 2 1 0;\nanim annotationShape1 3 0 0;";
	return 0;
}