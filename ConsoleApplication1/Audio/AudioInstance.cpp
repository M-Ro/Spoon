#include "AudioInstance.h"
#include "AudioObject.h"

AudioInstance::AudioInstance(std::string const& soundname, bool looping)
{
	this->looping = looping;
	this->source = Source::GetFreeSource();
	this->destroy = false;
	this->audio_object = nullptr;

	if (!source) {
		return;
	}

	alSourcef(source->GetALSource(), AL_PITCH, 1);
	alSourcef(source->GetALSource(), AL_GAIN, 1.0f);
	alSource3f(source->GetALSource(), AL_POSITION, 0, 0, 0);
	alSource3f(source->GetALSource(), AL_VELOCITY, 0, 0, 0);

	ALint looping_al = looping ? AL_TRUE : AL_FALSE;
	alSourcei(source->GetALSource(), AL_LOOPING, looping_al);

	this->audio_object = AudioObject::GetAudioObject(soundname);
	this->source->QueueBuffer(this->audio_object->GetBuffers()[0]);
	alSourcePlay(this->source->GetALSource());
}

AudioInstance::~AudioInstance()
{

}

void AudioInstance::Update()
{
	if (!source) {
		return;
	}

	ALuint source_id = source->GetALSource();
	ALint source_state;
	alGetSourcei(source_id, AL_SOURCE_STATE, &source_state);

	if (source_state != AL_PLAYING && looping == false) {
		Source::ReturnSourceToPool(this->source);
		this->source = nullptr;
		this->destroy = true;
		return;
	}
}

void AudioInstance::SetPosition(glm::vec3 pos)
{
	if (!source) {
		return;
	}

	ALuint source_id = source->GetALSource();
	ALint source_state;
	alGetSourcei(source_id, AL_SOURCE_STATE, &source_state);

	if (source_state != AL_PLAYING) {
		return;
	}

	alSource3f(source_id, AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioInstance::SetVelocity(glm::vec3 vel)
{
	if (!source) {
		return;
	}

	ALuint source_id = source->GetALSource();
	ALint source_state;
	alGetSourcei(source_id, AL_SOURCE_STATE, &source_state);

	if (source_state != AL_PLAYING) {
		return;
	}

	alSource3f(source_id, AL_VELOCITY, vel.x, vel.y, vel.z);
}
