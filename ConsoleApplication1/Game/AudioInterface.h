#pragma once

#include <map>

#include "../Audio/AudioSystem.h"
#include "../Audio/AudioInstance.h"

class Entity;

// This is a gamecode wrapper for the engine's audio library and handles attaching of
// sound sources and realtime tracking of sound <-> entity position/velocities
class AudioInterface
{
public:
	AudioInterface();
	~AudioInterface();

	// Play a sound attached to the given entity
	size_t Play(Entity* tracked, std::string const& soundfile, float volume, bool looping);
	
	// Play a sound at the given location
	size_t PlayAt(glm::vec3 pos, std::string const& soundfile, float volume, bool looping);
	
	// Returns false if we couldn't stop the sound for whatever reason
	bool Stop(size_t instance_id);

	// Should be called every frame, updates audio instance positions to their tracked entities
	void Update();

protected:

	// Gets a UID for each sound played
	size_t GetNextId();

	AudioSystem* audio;

	typedef std::map<size_t, std::pair<AudioInstance*, Entity*>> InstanceSet;
	typedef std::map<size_t, std::pair<AudioInstance*, glm::vec3>> StaticInstanceSet;

	InstanceSet instances;
	StaticInstanceSet static_instances;

	size_t next_id;
};