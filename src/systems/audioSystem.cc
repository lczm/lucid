#include "audioSystem.h"

namespace AudioVariables
{
SoundEffectsPlayer player;
}

AudioSystem::AudioSystem()
{
  sd = SoundDevice::Get();
  seLibrary = SoundEffectsLibrary::Get();
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(float dt, Registry* registry, Input* input)
{
  registry->GetComponentsIter<Sound, Transform>()->Each([&](Sound& sound, Transform& transform) {
    if (sound.play)
    {
      ALuint soundBuffer = seLibrary->Load(sound.filePath);
      AudioVariables::player.SetPosition(transform.position.x, transform.position.y,
                                         transform.position.z);
      AudioVariables::player.SetLooping(sound.looping);
      AudioVariables::player.Play(soundBuffer);
      sound.play = false;
    }
  });
}
