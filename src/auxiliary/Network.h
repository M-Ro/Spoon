#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL_net.h>

bool InitialiseClient(std::string ip, int localport, int remoteport);
bool InitialiseHost(int localport);

class ClientNetworkModule
{
	protected:
		IPaddress remoteip;
		UDPsocket socket;
		UDPpacket * packet;
	public:
		bool Initialise(std::string ip, int localport, int remoteport);
		bool CheckForData();
		bool Send(std::string message);
		~ClientNetworkModule();
};

class HostNetworkModule
{
	protected:
		UDPsocket socket;
		UDPpacket * packet;
		std::vector<IPaddress> connections;
		void checkForNewConnection(IPaddress address);
	public:
		bool Initialise(int localport);
		bool CheckForData();
		void SendAll(std::string message);
		~HostNetworkModule();
};

extern HostNetworkModule * hostmodule;
extern ClientNetworkModule * clientmodule;