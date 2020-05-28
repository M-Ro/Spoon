#pragma once

//	Network communication
extern const int NET_entUpdate;	 //	Updating the status of an entity		(server to client)
extern const int NET_entRemove;  //	Remove this 							(server to client)
extern const int NET_selfInfo;	 //	We try to tell the player his own id	(server to client)
extern const int NET_spoonSpawn; //	Played press Mouse1 					(client to server)
extern const int NET_skullSpawn; // Played cheatspawned a skull 			(client to server)