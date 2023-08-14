#ifndef WorldQuery_server_H
#define WorldQuery_server_H


#include <string>
#include "WorldQuery.h"
//#include "ServerToClient.h"

struct WorldQueryServerArguments
{/*
	std::vector<imi::AgentControl_moveToPosition_args>& _worldPosition;*/
	//ServerToClient* stc;
    WorldQueryServerArguments(ServerToClient* ostc)
	:stc(ostc)
    {

    }

};

void startWorldQueryServer(WorldQueryServerArguments*);

#endif