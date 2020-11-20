#include "audioSystem.h"

AudioSystem::AudioSystem() {
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(double dt, Registry* registry, Input* input) {
  SoundDevice::Init();
  ALuint sound = SoundBuffer::Get()->AddSoundEffect(GRUNT_SOUND);

  static MusicBuffer pianoMusic(PIANO_MUSIC);
  if (!pianoMusic.IsPlaying()) {
    pianoMusic.Play();
  }
  if (pianoMusic.IsPlaying()) {
	  pianoMusic.UpdateBufferStream();
  }
}