#include "audioSystem.h"

AudioSystem::AudioSystem()
{
  // seLibrary = SoundEffectsLibrary::Get();
  sd = SoundDevice::Get();
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(float dt, Registry* registry, Input* input)
{
  Listener listener = registry->GetResource<Listener>();

  sd->SetLocation(listener.x, listener.y, listener.z);
  // TODO: add feature to change orientation

  // TODO: change get components to check if theres transform instead of using each and eachexact

  // Sound without transform default plays at where the listener is
  registry->GetComponentsIter<Sound>()->EachExact([&](Sound& sound) {
    sound.SetPosition(listener.x, listener.y, listener.z);

    if (sound.play)
    {
      sd->SetGain(sound.gain);
      sound.SetLooping(sound.looping);
      sound.Play();
      sound.play = false;
    }
  });

  // Sound with transform plays at where the transform is
  registry->GetComponentsIter<Sound, Transform>()->Each([&](Sound& sound, Transform& transform) {
    sound.SetPosition(transform.position.x, transform.position.y, transform.position.z);

    if (sound.play)
    {
      sd->SetGain(sound.gain);
      sound.SetLooping(sound.looping);
      sound.Play();
      sound.play = false;
    }
  });
}
