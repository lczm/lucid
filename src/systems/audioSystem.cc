#include "audioSystem.h"

AudioSystem::AudioSystem() {
  SoundDevice::Init();
  gruntCooldown = 3;
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(double dt, Registry* registry, Input* input) {
  // this chunk isnt supposed to be running in update but
  // still thinking of a way to put it somewhere where itll only run once
  static SoundEffectsPlayer sePlayer1;
  static ALuint sound1 = SoundEffectsLibrary::Get()->Load(GRUNT_SOUND);
  static MusicBuffer pianoMusic(PIANO_MUSIC);

  // gruntcooldown just so it doesnt play a million times a second
  gruntCooldown += dt;
  if (gruntCooldown > 3) {
    sePlayer1.Play(sound1);
    gruntCooldown = 0;
  }

}