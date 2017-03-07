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
		bool Initialise(int localport);
		bool Connect(std::string ip, int remoteport);
		bool CheckForData();
		bool Send(char * msg, int len);
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
		void SendAll(char * msg, int len, IPaddress * address = NULL);
		void Send(char * msg, int len, IPaddress * address);
		~HostNetworkModule();
};

class NetworkPackage{
	public:
		size_t	length = 0;
		char 	msg[256];
		void addFloats	(float * to_add,	int how_many)	{memcpy(msg + length, to_add, how_many*sizeof(float));	length += how_many*sizeof(float);	};
		void addInts	(int * to_add,		int how_many)	{memcpy(msg + length, to_add, how_many*sizeof(int));	length += how_many*sizeof(int);		};
		void addChars	(char * to_add,		int how_many)	{memcpy(msg + length, to_add, how_many*sizeof(char));	length += how_many*sizeof(char);	};
		void addDoubles	(double * to_add,	int how_many)	{memcpy(msg + length, to_add, how_many*sizeof(double));	length += how_many*sizeof(double);	};
		void addString	(std::string * s )					{memcpy(msg + length, s->c_str(), s->size()*sizeof(char));length += s->size()*sizeof(char);	};
};

extern HostNetworkModule * hostmodule;
extern ClientNetworkModule * clientmodule;