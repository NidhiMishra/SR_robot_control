#include <windows.h>

#using <mscorlib.dll>

#include "NadineAnimations.h"
#include <string>
#include <vector>

using namespace System;
using namespace System::IO;
using namespace std;

#define __DEBUG


//List of Aimations available. Not used yet
enum AnimList
{
	NOD = 0,
	SHAKE=1,
	WAVE=2,
	BLINK=3,
	NEUTRAL=4,
	HAPPY=5,
}animList;


Animations::Animations()
{
	initChannels();
	int i=0;
	channelsBool=gcnew array <bool>(28);
	for(i=0;i<28;i++)
		channelsBool[i]=false;
}

Animations::Animations(String ^id, array <bool> ^AchannelsBool)
{
	initChannels();
	StreamReader^ sr = gcnew StreamReader( "../animations/"+id+".txt");
	Console::WriteLine( "../animations/"+id+".txt");
	//channels=gcnew array <array <int> ^> (28);
	
	numberFrames=0;
	array <String ^> ^animation;
	int i=0;
	channelsBool=gcnew array <bool>(28);
	for(i=0;i<28;i++)
		channelsBool[i]=AchannelsBool[i];
//		channels->push_back(new vector<unsigned char>());
	i=0;
	try
	{
		try
			{
				String^ line;
				line = sr->ReadToEnd();
				animation=line->Split(',');
				
				/*for(i=0;i<28;i++)
				{
					if(channelsBool[i])
					{
						//channels[i]=gcnew array <int> ((animation->Length-1)/29);
						
						//channels->at(i)->swap(new vector<unsigned char>());
					}
					else
					{
						//channels[i]=nullptr;
					}
				}*/
				i=0;
				while(i<(animation->Length-29))
				{
					for(int j=0;j<28;j++)
					{
						if(channelsBool[j])
						{
							channels->at(j).push_back((unsigned char)System::Convert::ToInt32(animation[i+j+1]));
//							channels[j][(i/29)]=System::Convert::ToInt32(animation[i+j+1]);
						}
					}
					i+=29;
					numberFrames++;
				}
				numberFrames--;
			}
			finally
			{
				if ( sr )
					delete (IDisposable^)sr;
			}
	}
	catch ( Exception^ e ) 
	{
		// Let the user know what went wrong.
		Console::WriteLine( "The file could not be read:" );
		Console::WriteLine( e->Message );
	}
	Console::WriteLine(numberFrames);
}

	/*

	//Extend the animFiles array with new animation when needed
		array<String ^> ^ animFiles = gcnew array<String ^>(6);
		animAvailable = gcnew array<String ^>(6);
		animFiles[0]="nod";
		animFiles[1]="shake";
		animFiles[2]="wave";
		animFiles[3]="blink";
		animFiles[4]="neutral";
		animFiles[5]="happy";
		for(int i=0; i<6;i++)
		{
			StreamReader^ sr = gcnew StreamReader( "../animations/"+animFiles[i]+".txt");
			try
			{
				try
				  {
					String^ line;
					line = sr->ReadToEnd();
					animAvailable[i]=line;
				  }
				  finally
				  {
					 if ( sr )
						delete (IDisposable^)sr;
				  }
			}
		   catch ( Exception^ e ) 
		   {
			  // Let the user know what went wrong.
			  Console::WriteLine( "The file could not be read:" );
			  Console::WriteLine( e->Message );
		   }
	}*/


String^ Animations::getAnimation(int id)
{
	return animAvailable[id];
}

void Animations::initChannels()
{
	//CHANGE THIS
	channels=new vector<vector<unsigned char>>();
	channel1=new vector<unsigned char>();
	channel2=new vector<unsigned char>();
	channel3=new vector<unsigned char>();
	channel4=new vector<unsigned char>();
	channel5=new vector<unsigned char>();
	channel6=new vector<unsigned char>();
	channel7=new vector<unsigned char>();
	channel8=new vector<unsigned char>();
	channel9=new vector<unsigned char>();
	channel10=new vector<unsigned char>();
	channel11=new vector<unsigned char>();
	channel12=new vector<unsigned char>();
	channel13=new vector<unsigned char>();
	channel14=new vector<unsigned char>();
	channel15=new vector<unsigned char>();
	channel16=new vector<unsigned char>();
	channel17=new vector<unsigned char>();
	channel18=new vector<unsigned char>();
	channel19=new vector<unsigned char>();
	channel20=new vector<unsigned char>();
	channel21=new vector<unsigned char>();
	channel22=new vector<unsigned char>();
	channel23=new vector<unsigned char>();
	channel24=new vector<unsigned char>();
	channel25=new vector<unsigned char>();
	channel26=new vector<unsigned char>();
	channel27=new vector<unsigned char>();
	channel28=new vector<unsigned char>();

	channels->push_back(*channel1);
	channels->push_back(*channel1);
	channels->push_back(*channel2);
	channels->push_back(*channel3);
	channels->push_back(*channel4);
	channels->push_back(*channel5);
	channels->push_back(*channel6);
	channels->push_back(*channel7);
	channels->push_back(*channel8);
	channels->push_back(*channel9);
	channels->push_back(*channel10);
	channels->push_back(*channel11);
	channels->push_back(*channel12);
	channels->push_back(*channel13);
	channels->push_back(*channel14);
	channels->push_back(*channel15);
	channels->push_back(*channel16);
	channels->push_back(*channel17);
	channels->push_back(*channel18);
	channels->push_back(*channel19);
	channels->push_back(*channel20);
	channels->push_back(*channel21);
	channels->push_back(*channel22);
	channels->push_back(*channel23);
	channels->push_back(*channel24);
	channels->push_back(*channel25);
	channels->push_back(*channel26);
	channels->push_back(*channel27);
	channels->push_back(*channel28);
}


bool Animations::isChannelUsing(int id)
{
	return this->channelsBool[id];
}

int Animations::readAllXMLDoc( XMLDocWrapper * currDoc)
{
#ifdef __DEBUG
	fprintf(stdout, "XMLDoc n_nodes = %d\n", currDoc->Wrapper_getXMLDoc_n_nodes());
#endif
	for ( int i = 0 ; i < currDoc->Wrapper_getXMLDoc_n_nodes(); i ++ )
	{
		int returnValue = this->readOneXMLNode( currDoc->Wrapper_getXMLDoc_Node_At(i) );
		if ( returnValue ) return returnValue;
	}
	return 0;
}

// tag name identification is done here
int Animations::readOneXMLNode( XMLNodeWrapper * currNode )
{
	if ( ! currNode ) return 0; //NULL termination
	const char * currNodeTag = currNode->Wrapper_getXMLNode_tag();
#ifdef __DEBUG
		fprintf( stdout , "visiting node:%s\n", currNodeTag);
#endif

	if ( strcmp( currNodeTag , "animation" ) == 0 )
	{
		// root Node, do nth
	}
	else
	if ( strcmp( currNodeTag , "animation_name" ) == 0 )
	{
		this->animationName = gcnew String(std::string(currNode->Wrapper_getXMLNode_text()).c_str());
	}
	else
	if ( strncmp( currNodeTag , "channel__" , strlen("channel") ) == 0 )
	{
		// read to see which channel
		int channel;
		int numRead = sscanf( currNodeTag , "channel%d", &channel);
		if ( numRead != 1 )
		{
			fprintf( stderr , "readOneXMLNode[error]: cannot read correct channel number\n" );
			return -1;
		}

		// prase the text
		// **assumed that the channel vector pointers are already initialized
		try
		{
			char * tempBuff = new char[2046];
			strcpy(tempBuff, currNode->Wrapper_getXMLNode_text());
			char * pch;
			pch = strtok (tempBuff," ");
			while (pch != NULL)
			{
				this->channels->at(channel-1).push_back(unsigned char(atoi(pch)));
				pch = strtok (NULL, " ");
			}
			this->channelsBool[channel-1] = true;
			this->numberFrames = this->channels->at(channel-1).size();
#ifdef __DEBUG
		fprintf( stdout , "visiting node:%s\n", currNodeTag);
#endif			
		}
		catch(std::exception& e)
		{
			fprintf( stderr , "readOneXMLNode[error]: exception[%s]\n", e.what());
			return -2;
		}
	}

	// recurvisely go to all children nodes
	for ( int i = 0 ; i < currNode->Wrapper_XMLNode_get_n_children() ; i ++ )
	{
		
		int returnValue = this->readOneXMLNode(currNode->Wrapper_getXMLNode_Children_At(i));
		if ( returnValue ) return returnValue;
	}
	return 0;
}



void Animations::LoadAnimationFromXML( char * XMLFilename )
{

	/////////////////////////////////////
	// Step 1: load it in XML structure

	XMLDocWrapper * xmlWrapper = new XMLDocWrapper();
	xmlWrapper->Wrapper_XMLDoc_init();

	if (!xmlWrapper->Wrapper_XMLDoc_parse_file_DOM(XMLFilename))
	{
		printf("Error while loading\n");
		return;
	}
#ifdef __DEBUG
	//fprintf(stdout, "Wrapper_XMLDoc_parse_file_DOM Done, n_nodes = %d\n", xmlWrapper->xmlDoc->n_nodes);
	fprintf(stdout, "Wrapper_XMLDoc_parse_file_DOM Done, n_nodes = %d\n", xmlWrapper->Wrapper_getXMLDoc_n_nodes());
	//else
	//	xmlWrapper->Wrapper_XMLDoc_print(stdout, "\n", "\t",true, 20460, 32);
#endif
	

	/////////////////////////////////////
	// Step 2: convert the data from XML structure to vector unsigned char

	
	int returnValue = this->readAllXMLDoc(xmlWrapper);
	if ( returnValue ) 
	{
		fprintf( stderr , "LoadAnimationFromXML is getting error code [%d] from readAllXMLDoc, exiting....\n", returnValue);
		exit(returnValue);
	}

	///////////////////////////////////
	// Clean up
	// - XML sructure
	// - 

	xmlWrapper->Wrapper_XMLDoc_free();
}



AnimationLibrary::AnimationLibrary()
{
	animations = gcnew Dictionary<String^, Animations^>();
	representiveFrame = gcnew Dictionary<String^, Animations^>();
}

int dirExists(char* dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return 1;  //something is wrong with your path!
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return 0;   // this is a directory!
	return -1;    // this is not a directory!
}

vector<char*> getAllFilesInFolderWithExtension(char* dirName_in, char* ext)
{

	vector<char*> allFiles;

	char path[_MAX_PATH];
	WIN32_FIND_DATA ffd;

	strncpy(path, dirName_in, _MAX_PATH);
	if (path[strlen(path) - 1] != '\\')
		strcat(path, "\\");
	
	strcat(path, ext);

	HANDLE hFind = FindFirstFile(path, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return allFiles;
	}

	bool finished = false;
	while (!finished)
	{
#ifdef __DEBUG
		if (dirName_in[strlen(dirName_in)-1] != '\\')
			fprintf(stdout , "%s\\%s\n", dirName_in, ffd.cFileName);
		else
			fprintf(stdout , "%s%s\n", dirName_in, ffd.cFileName);
#endif
		char * foundFile = new char[_MAX_PATH];
		if (dirName_in[strlen(dirName_in)-1] != '\\')
			sprintf(foundFile , "%s\\%s" , dirName_in, ffd.cFileName);
		else
			sprintf(foundFile , "%s%s" , dirName_in, ffd.cFileName);
		
		allFiles.push_back(foundFile);

		if (!FindNextFile(hFind, &ffd))
			finished = true;
	}

	return allFiles;
}



int AnimationLibrary::loadLibraryFromFolder(char * folderPath)
{

	//////////////////////////////////
	// check if folder is exist and correct

	vector<char*> allXMLFileFullPath;
	if ( dirExists( folderPath ) == 0 )
		allXMLFileFullPath = getAllFilesInFolderWithExtension( folderPath , "*.xml");
	else
	{
		fprintf( stderr , "loadLibraryFromFolder[error %d]: problem opening folder [%s]\n", dirExists( folderPath ) , folderPath);
		return -1;
	}	

	for ( int i = 0 ; i < allXMLFileFullPath.size() ; i ++ )
	{
		Animations^ newAnime = gcnew Animations();
		newAnime->LoadAnimationFromXML(allXMLFileFullPath[i]);
		if ( newAnime->numberFrames == 1 ) 
			this->representiveFrame->Add(newAnime->animationName, newAnime);
		else
			this->animations->Add(newAnime->animationName, newAnime);
	}

	return 0;
}

Animations^ AnimationLibrary::getAnimation(String^ animeName)
{
	Animations^ returnAnime = gcnew Animations();

	if ( this->animations->TryGetValue(animeName, returnAnime) )
	{
		return returnAnime;
	}
	else
	{
		Console::WriteLine("Key = \"{0}\" is not found.", animeName);
		return returnAnime;
	}
	
}


Animations^ AnimationLibrary::getPoseFrame(String^ animeName)
{
	Animations^ returnAnime = gcnew Animations();

	if ( this->representiveFrame->TryGetValue(animeName, returnAnime) )
	{
		return returnAnime;
	}
	else
	{
		Console::WriteLine("Key = \"{0}\" is not found.", animeName);
		return nullptr;
	}
	
}


int AnimationLibrary::getAnimationSize()
{
	return this->animations->Count;
}

int AnimationLibrary::getPoseFrameSize()
{
	return this->representiveFrame->Count;
}