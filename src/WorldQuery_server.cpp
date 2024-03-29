// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "WorldQuery.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "EventPublisher_constants.h"
#include "WorldQuery_server.h"
#include <map>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace ::imi;

class WorldQueryHandler : virtual public WorldQueryIf {
 public:
  WorldQueryHandler()
 {
    // Your initialization goes here
  }

  void getLocation(Location& _return, const  ::imi::TargetName& object) {
    // Your implementation goes here
      _return.location.x = (rand() % 5 + (-5))/4 ;
      _return.location.y = 1;
      _return.location.z = 1;
    printf("getLocation\n");
  }



};


void startWorldQueryServer(WorldQueryServerArguments* arg) {

	#ifdef _WIN32
    /* Init windows sockets :D */
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);

        // TODO Throw exception??
    }
   /** End init windows socket**/
    #endif

    int port = g_EventPublisher_constants.DEFAULT_WORLD_QUERY_PORT;
  
    shared_ptr<WorldQueryHandler> handler(new WorldQueryHandler());
    shared_ptr<TProcessor> processor(new WorldQueryProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
 
}
