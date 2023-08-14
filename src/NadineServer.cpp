#pragma unmanaged
#include "NadineServer.h"
#include "stdafx.h"
#include "AgentControl.h"
#include "AgentControl_server.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "ThriftTools.hpp"
#include <map>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace ::imi;
#pragma managed


NadineServer::NadineServer(void)
{

}

NadineServer::NadineServer(char* voicePath, char* voicePathGerman, char* voicePathFrench)
{
	this->voicePath = new char[2046];
	strcpy(this->voicePath , voicePath);

	this->voicePathGerman = new char[2046];
	strcpy(this->voicePathGerman , voicePathGerman);

	this->voicePathFrench = new char[2046];
	strcpy(this->voicePathFrench , voicePathFrench);
}

//NadineServer::~NadineServer(void)
//{
//}
#pragma unmanaged


void NadineServer::setWorldClientIP( std::string ip )
{
	this->worldClientIP = ip;
}

void NadineServer::setFeedbackServerIP( std::string ip )
{
	//this->feedbackServerIP = "localhost";
	this->feedbackServerIP = ip;
}

void NadineServer::setAnimationXMLPath(char * animationXMLPath)
{
	this->animationXMLPath = new char[2046];
	strcpy(this->animationXMLPath, animationXMLPath);
}

void NadineServer::startServer(void)
{
	//start the thrift server for the Nadine Robot
	std::cout<<"AAAAAAA \n";
	int port = 9090;
	initializeThrift(); 



	//int port2 = imi::g_Inputs_constants.DEFAULT_FACE_SERVICE_PORT;
	boost::shared_ptr<AgentControlHandler> handler(new AgentControlHandler(this->voicePath,this->voicePathGerman,this->voicePathFrench, this->animationXMLPath, this->worldClientIP, this->feedbackServerIP));
	imi::createServer<AgentControlHandler, AgentControlProcessor>(handler, port);




	/*
	boost::shared_ptr<AgentControlHandler> handler(new AgentControlHandler(this->voicePath, this->animationXMLPath, this->worldClientIP, this->feedbackServerIP));
	//start feedback
	//imi::createServer <AgentControlHandler, AgentControlProcessor> (handler, port);
	//keepAlive(server,port);
	boost::shared_ptr<TProcessor> processor(new AgentControlProcessor(handler));
	boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	std::cout<<"AAAAAAA  BB \n";
	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);

	std::cout<<"BBBBBBB \n";
	//keepAlive(server,port);
	continueToServe=true;
    while(continueToServe)
	{
		try
        {
            std::cerr << "Serving on port " << port << ".\n";
            server.serve();
            std::cerr << "Done serving on port " << port << ".\n";
        } 
        catch( std::exception exc ) 
        {
            std::cerr << "Lost Server on port " << port << ". Error: " << exc.what();
        }
            
	}
  server.stop();*/
}

void NadineServer::stopServer()
{//This function is not working at the moment....
	continueToServe=false;
}
