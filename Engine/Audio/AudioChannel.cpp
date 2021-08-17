#include "AudioChannel.h"

namespace nc {
	bool AudioChannel::IsPlaying()
	{
		if (channel = nullptr) return false;
		bool isPlaying;
		channel->isPlaying(&isPlaying);

		return isPlaying;
	}

	void AudioChannel::Stop()
	{
		if (IsPlaying()) {
			channel->stop();

		}
	}

	void AudioChannel::SetPitch(float pitch)
	{
		if (IsPlaying()) {
			channel->setPitch(pitch);

		}
	}

	float AudioChannel::GetPitch()
	{
		float volume;
		if (IsPlaying()) {
			channel->getPitch(&volume);
		}
		return volume;
	}

	void AudioChannel::SetVolume(float volume)
	{
		if (IsPlaying()) 
		{
			channel->setPitch(volume);

		}
	}

	float AudioChannel::GetVolume()
	{
		float volume = 0;
		if (IsPlaying()) {
			channel->setPitch(volume);

		}
		return volume;
	}

}
