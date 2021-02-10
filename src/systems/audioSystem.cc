#include "audioSystem.h"

AudioSystem::AudioSystem()
{
  seLibrary = SoundEffectsLibrary::Get();
  sd = SoundDevice::Get();
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(float dt, Registry* registry, Input* input)
{
  Listener listener = registry->GetResource<Listener>();
  sd->SetLocation(listener.x, listener.y, listener.z);
  // TODO: add feature to change orientation

  // Sound without transform default plays at where the listener is
  registry->GetComponentsIter<Sound>()->EachExact([&](Sound& sound) {
    ALuint soundBuffer = seLibrary->Load(sound.filePath);

    // if its a new sound that hasnt been played yet
    if (!bufferMap.count(soundBuffer))
    {
      SoundEffectsPlayer* player = new SoundEffectsPlayer();
      bufferMap[soundBuffer] = player;
    }
    bufferMap[soundBuffer]->SetPosition(listener.x, listener.y, listener.z);

    if (sound.play)
    {
      sd->SetGain(sound.gain);
      bufferMap[soundBuffer]->SetLooping(sound.looping);
      bufferMap[soundBuffer]->Play(soundBuffer);
      sound.play = false;
    }

    seLibrary->UnLoad(soundBuffer);
  });

  // Sound with transform plays at where the transform is
  registry->GetComponentsIter<Sound, Transform>()->EachExact(
      [&](Sound& sound, Transform& transform) {
        ALuint soundBuffer = seLibrary->Load(sound.filePath);

        // if its a new sound that hasnt been played yet
        if (!bufferMap.count(soundBuffer))
        {
          SoundEffectsPlayer* player = new SoundEffectsPlayer();
          bufferMap[soundBuffer] = player;
        }
        bufferMap[soundBuffer]->SetPosition(transform.position.x, transform.position.y,
                                            transform.position.z);

        if (sound.play)
        {
          sd->SetGain(sound.gain);
          bufferMap[soundBuffer]->SetLooping(sound.looping);
          bufferMap[soundBuffer]->Play(soundBuffer);
          sound.play = false;
        }

        seLibrary->UnLoad(soundBuffer);
      });
}
