#include <iostream>
#include <string>
#include "Network.h"

HostNetworkModule * hostmodule;
ClientNetworkModule * clientmodule;
int packet_size = 256;

bool InitialiseHost(int localport){
	hostmodule = new HostNetworkModule();

	if(!hostmodule->Initialise(localport)){
		delete hostmodule;
		return 0;
	}
	return 1;
}

bool HostNetworkModule::Initialise(int localport){
	if( SDLNet_Init() < 0 ){
		std::cout << "SDLNET_Init() failed:" << SDLNet_GetError() << std::endl;
		return 0;
	}

	if( !( socket = SDLNet_UDP_Open( localport ) ) )
	{
		std::cout << "Failed to open socket:" << SDLNet_GetError() << std::endl;
		return 0;
	}

	if( !( packet = SDLNet_AllocPacket( packet_size ) ) )
	{
		std::cout << "SDLNet_AllocPacket() failed:" << SDLNet_GetError() << std::endl;
		return 0; 
	}

	std::cout << "Hostmodule successfully initialised" << std::endl;
	return 1;
}

void HostNetworkModule::checkForNewConnection(IPaddress address){
	for(unsigned int i = 0; i < connections.size(); i++)
		if(connections[i].host == address.host && connections[i].port == address.port)
			return;

	connections.push_back(address);
	std::cout << "New Connection from:" << address.host << ":" << address.port << std::endl;
}

bool HostNetworkModule::CheckForData(){
	if ( SDLNet_UDP_Recv(socket, packet))
	{
		checkForNewConnection(packet->address);
		char message[packet->len];
		memcpy(message, packet->data, packet->len);
		std::cout << "Data received:" << message << std::endl;
		//std::cout << "From:         " << packet->address.host << ":" << packet->address.port << std::endl;
		return 1;
	}
	return 0;
}

HostNetworkModule::~HostNetworkModule(){
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
}

void HostNetworkModule::SendAll(std::string message){
	for(unsigned int i = 0; i < connections.size(); i++){
		memcpy(packet->data, message.c_str(), message.length() );
		packet->len = message.length();
		packet->address = connections[i];

		if ( SDLNet_UDP_Send(socket, -1, packet) == 0 )
		{
			std::cout << "SDLNet_UDP_Send failed:" << SDLNet_GetError() << std::endl;
		}
	}
}




bool InitialiseClient(std::string ip, int localport, int remoteport){
	if( SDLNet_Init() < 0 ){
		std::cout << "SDLNET_Init() failed:" << SDLNet_GetError() << std::endl;
		return 0;
	}

	clientmodule = new ClientNetworkModule();
	if(!clientmodule->Initialise(ip, localport, remoteport)){
		delete clientmodule;
		return 0;
	}
	return 1;
}

bool ClientNetworkModule::Initialise(std::string ip, int localport, int remoteport){
	if( SDLNet_Init() < 0 ){
		std::cout << "SDLNET_Init() failed:" << SDLNet_GetError() << std::endl;
		return 0;
	}

	if( !( socket = SDLNet_UDP_Open( localport ) ) )
	{
		std::cout << "Failed to open socket:" << SDLNet_GetError() << std::endl;
		return 0;
	}

	if( SDLNet_ResolveHost( &remoteip, ip.c_str(), remoteport ) < 0 )
	{
		std::cout << "Failed to resolve host:" << SDLNet_GetError() << std::endl;
		return 0;
	}

	if( !( packet = SDLNet_AllocPacket( packet_size ) ) )
	{
		std::cout << "SDLNet_AllocPacket() failed:" << SDLNet_GetError() << std::endl;
		return 0; 
	}

	packet->address.host = remoteip.host; 
	packet->address.port = remoteip.port;

	Send("Hi I'm new here");
	std::cout << "Clientmodule successfully initialised" << std::endl;
	return 1;
}

bool ClientNetworkModule::CheckForData(){
	if ( SDLNet_UDP_Recv(socket, packet))
	{
		char message[packet->len];
		memcpy(message, packet->data, packet->len);
		std::cout << "Data received:" << message << std::endl;
		//std::cout << "From:         " << packet->address.host << ":" << packet->address.port << std::endl;
		return 1;
	}
	return 0;
}

bool ClientNetworkModule::Send( std::string message ){
	memcpy(packet->data, message.c_str(), message.length() );
	packet->len = message.length();

	if ( SDLNet_UDP_Send(socket, -1, packet) == 0 )
	{
		std::cout << "SDLNet_UDP_Send failed:" << SDLNet_GetError() << std::endl;
		return 0; 
	}
	return 1;
}

ClientNetworkModule::~ClientNetworkModule(){
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
}