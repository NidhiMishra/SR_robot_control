//#pragma unmanaged
#pragma once
#ifndef _WorldClient_H
#define _WorldClient_H

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include <iostream>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

#include "WorldQuery.h"


void queryFromWorldClient(char* worldClientIP, imi::Location& _return, const  ::imi::TargetName& object);

#endif