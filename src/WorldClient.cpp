#pragma once

#include "WorldClient.h"
#include "EventPublisher_constants.h"

void queryFromWorldClient(char* worldClientIP, imi::Location& _return, const  ::imi::TargetName& object)
{
	//Where is the world model running? To do getLocation

    //boost::shared_ptr<TSocket> socket(new TSocket("worldquery.alpha.imi-ntu.sg", imi::g_EventPublisher_constants.DEFAULT_WORLD_QUERY_PORT));
	// old worldClientIP = "155.69.52.73"
    boost::shared_ptr<TSocket> socket(new TSocket(std::string(worldClientIP), imi::g_EventPublisher_constants.DEFAULT_WORLD_QUERY_PORT));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    imi::WorldQueryClient client = imi::WorldQueryClient(protocol);
    
    transport->open();

    client.getLocation(_return, object);

	if (object == "Default")
	{
		fprintf(stdout, "Default: %f %f %f\n", _return.location.x, _return.location.y, _return.location.z);
	}

   // sleep(3000);

    std::cout << "retrieved " << std::endl;

    transport->close();
   // return 0;
}