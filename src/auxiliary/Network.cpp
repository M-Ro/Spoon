#include <iostream>
#include <string>
#include <sstream>
#include "Network.h"

#include "../game/Game.h"
#include "../game/base/GameServer.h"

extern Game *game;

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
	game->AddNewPlayer(&connections[connections.size()-1]);
	std::cout << "New Connection from:" << address.host << ":" << address.port << std::endl;
}

bool HostNetworkModule::CheckForData(){
	if ( SDLNet_UDP_Recv(socket, packet))
	{
		checkForNewConnection(packet->address);
		std::string msg(packet->data,packet->data+packet->len);
		std::cout << "Data received:" << msg << std::endl;
		//std::cout << "From:         " << packet->address.host << ":" << packet->address.port << std::endl;
		return 1;
	}
	return 0;
}

HostNetworkModule::~HostNetworkModule(){
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
}

//	address = the player we don't want to send anything
void HostNetworkModule::SendAll(char * msg, int len, IPaddress * address){
	memcpy(packet->data, msg, len );
	packet->len = len;
	for(unsigned int i = 0; i < connections.size(); i++){
		if(address  == &connections[i])	
			continue;
		packet->address = connections[i];
		if ( SDLNet_UDP_Send(socket, -1, packet) == 0 )
		{
			std::cout << "SDLNet_UDP_Send failed: " << SDLNet_GetError() << std::endl;
		}
	}
}

void HostNetworkModule::Send(char * msg, int len, IPaddress * address){
	memcpy(packet->data, msg, len );
	packet->len = len;
	packet->address = *address;
	if ( SDLNet_UDP_Send(socket, -1, packet) == 0 )
	{
		std::cout << "SDLNet_UDP_Send failed: " << SDLNet_GetError() << std::endl;
	}
}




bool InitialiseClient(std::string ip, int localport, int remoteport){
	if( SDLNet_Init() < 0 ){
		std::cout << "SDLNET_Init() failed: " << SDLNet_GetError() << std::endl;
		return 0;
	}

	clientmodule = new ClientNetworkModule();
	if(clientmodule->Initialise(localport)){
		if(!clientmodule->Connect(ip, remoteport)){
			delete clientmodule;
			return 0;
		}
	}
	else
		return 0;
	return 1;
}
//////
bool ClientNetworkModule::Initialise(int localport){
	if( SDLNet_Init() < 0 ){
		std::cout << "SDLNET_Init() failed: " << SDLNet_GetError() << std::endl;
		return 0;
	}
	std::cout<<"SDLNET initialised"<<std::endl;

	if( !( socket = SDLNet_UDP_Open( localport ) ) )
	{
		std::cout << "Failed to open socket: " << SDLNet_GetError() << std::endl;
		return 0;
	}
	std::cout<<"Socket opened to port "<<localport<<std::endl;
	return 1;
}

bool ClientNetworkModule::Connect(std::string ip, int remoteport){
	if( SDLNet_ResolveHost( &remoteip, ip.c_str(), remoteport ) < 0 )
	{
		std::cout << "Failed to resolve host: " << SDLNet_GetError() << std::endl;
		return 0;
	}
	std::cout<<"Host resolved succesfully"<<std::endl;

	if( !( packet = SDLNet_AllocPacket( packet_size ) ) )
	{
		std::cout << "SDLNet_AllocPacket() failed: " << SDLNet_GetError() << std::endl;
		return 0; 
	}
	std::cout<<"Network Packet allocated"<<std::endl;

	packet->address.host = remoteip.host; 
	packet->address.port = remoteip.port;

	std::cout << "Clientmodule successfully initialised" << std::endl;
	char hello[] = "hello";
	Send(hello);
	return 1;
}

bool ClientNetworkModule::CheckForData(){
	if ( SDLNet_UDP_Recv(socket, packet))
	{
		//std::string msg(packet->data,packet->data+packet->len);
		//std::cout<<"client:"<<packet->len<<std::endl;
		game->HandleNetworkMsg((char*)packet->data);
		//std::cout << "Data received:" << msg << std::endl;
		//std::cout << "From:         " << packet->address.host << ":" << packet->address.port << std::endl;
		return 1;
	}
	return 0;
}

bool ClientNetworkModule::Send( char * msg ){
	memcpy(packet->data, msg, strlen(msg) );
	packet->len = strlen(msg);
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