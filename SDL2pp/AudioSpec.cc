/*
  libSDL2pp - C++11 bindings/wrapper for SDL2
  Copyright (C) 2014 Dmitry Marakasov <amdmi3@amdmi3.ru>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <algorithm>

#include <SDL2pp/Audio.hh>

namespace SDL2pp {

void AudioSpec::SDLCallback(void *userdata, Uint8* stream, int len) {
	AudioSpec* audiospec = static_cast<AudioSpec*>(userdata);
	audiospec->callback_(stream, len);
}

AudioSpec::AudioSpec() {
	std::fill((char*)this, (char*)this + sizeof(SDL_AudioSpec), 0);
}

AudioSpec::AudioSpec(int freq, SDL_AudioFormat format, Uint8 channels, Uint16 samples, AudioSpec::AudioCallback&& callback)
	  : callback_(std::move(callback)) {
	std::fill((char*)this, (char*)this + sizeof(SDL_AudioSpec), 0);
	SDL_AudioSpec::freq = freq;
	SDL_AudioSpec::format = format;
	SDL_AudioSpec::channels = channels;
	SDL_AudioSpec::samples = samples;
	if (callback) {
		SDL_AudioSpec::callback = SDLCallback;
		SDL_AudioSpec::userdata = static_cast<void*>(this);
	}
}

AudioSpec::AudioSpec(const AudioSpec& other, AudioSpec::AudioCallback&& callback) : SDL_AudioSpec(*other.Get()), callback_(std::move(callback)) {
	if (callback) {
		SDL_AudioSpec::callback = SDLCallback;
		SDL_AudioSpec::userdata = static_cast<void*>(this);
	}
}

AudioSpec::~AudioSpec() {
}

AudioSpec::AudioSpec(AudioSpec&&) = default;
AudioSpec& AudioSpec::operator=(AudioSpec&&) = default;

void AudioSpec::ChangeCallback(AudioCallback&& callback) {
	callback_ = callback;
}

void AudioSpec::MergeChanges(const SDL_AudioSpec& obtained) {
	freq = obtained.freq;
	format = obtained.format;
	channels = obtained.channels;
	samples = obtained.samples;
}

const SDL_AudioSpec* AudioSpec::Get() const {
	return static_cast<const SDL_AudioSpec*>(this);
}

bool AudioSpec::IsSameFormat(const AudioSpec& other) const {
	return freq == other.freq && format == other.format && channels == other.channels;
}

}
