#include "AudioInterface.h"

#include "Base/Entity/Entity.h"

AudioInterface::AudioInterface()
{
	this->audio = new AudioSystem();
	this->next_id = 1;
}

AudioInterface::~AudioInterface()
{
	// TODO clean up instances

	delete this->audio;
}

void AudioInterface::Update()
{
	for (auto& kv : instances) {
		kv.second.first->Update();
		kv.second.first->SetPosition(kv.second.second->position);
		kv.second.first->SetVelocity(kv.second.second->velocity);
	}

	for (auto& kv : static_instances) {
		kv.second.first->Update();
	}

	// Remove any instances we need to
	for (auto it = instances.cbegin(); it != instances.cend();)
	{
		if (it->second.first->destroy) {
			delete it->second.first;
			it = instances.erase(it);
		}
		else {
			++it;
		}
	}

	// Remove any static instances we need to
	for (auto it = static_instances.cbegin(); it != static_instances.cend();)
	{
		if (it->second.first->destroy) {
			delete it->second.first;
			it = static_instances.erase(it);
		}
		else {
			++it;
		}
	}
}

size_t AudioInterface::Play(Entity* tracked, std::string const& soundfile, float volume, bool looping)
{
	size_t id = GetNextId();

	AudioInstance* obj = new AudioInstance(soundfile, looping);
	obj->SetPosition(tracked->position);
	obj->SetVelocity(tracked->velocity);

	std::pair<AudioInstance*, Entity*> pairing {
		obj, tracked
	};

	instances.emplace(id, pairing);

	return next_id;
}

size_t AudioInterface::PlayAt(glm::vec3 pos, std::string const& soundfile, float volume, bool looping)
{
	size_t id = GetNextId();

	AudioInstance* obj = new AudioInstance(soundfile, looping);
	obj->SetPosition(pos);

	std::pair<AudioInstance*, glm::vec3> pairing{
		obj, pos
	};

	static_instances.emplace(id, pairing);

	return next_id;
}

bool AudioInterface::Stop(size_t instance_id)
{
	if (instances.count(instance_id)) {
		std::pair<AudioInstance*, Entity*> pairing = instances.at(instance_id);

		// TODO

		return true;
	}
	else if (static_instances.count(instance_id)) {
		std::pair<AudioInstance*, Entity*> pairing = instances.at(instance_id);

		// TODO

		return true;
	}

	return false;
}

size_t AudioInterface::GetNextId()
{
	return next_id++;
}
