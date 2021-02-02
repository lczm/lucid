#include "audioSystem.h"

namespace AudioVariables
{
SoundDevice* sd = SoundDevice::Get();
}  // namespace AudioVariables

AudioSystem::AudioSystem()
{
  seLibrary = SoundEffectsLibrary::Get();
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(float dt, Registry* registry, Input* input)
{
  registry->GetComponentsIter<Sound, Transform>()->Each([&](Sound& sound, Transform& transform) {
    if (sound.play)
    {
      ALuint soundBuffer = seLibrary->Load(sound.filePath);
      // if its a new sound that hasnt been played yet
      if (!bufferMap.count(soundBuffer))
      {
        SoundEffectsPlayer* player = new SoundEffectsPlayer();
        bufferMap[soundBuffer] = player;
      }
      bufferMap[soundBuffer]->SetPosition(transform.position.x, transform.position.y,
                                          transform.position.z);
      bufferMap[soundBuffer]->SetLooping(sound.looping);
      bufferMap[soundBuffer]->Play(soundBuffer);
      sound.play = false;
    }
  });

  Listener listener = registry->GetResource<Listener>();
  AudioVariables::sd->SetLocation(listener.x, listener.y, listener.z);
  AudioVariables::sd->SetGain(listener.gain);
  // TODO: add feature to change orientation
}
