#include "audioSystem.h"

namespace AudioVariables {
SoundDevice* sd = SoundDevice::Get();
SoundEffectsPlayer sePlayer;
}  // namespace AudioVariables

AudioSystem::AudioSystem() {
  gruntCooldown = 2;
}

AudioSystem::~AudioSystem() = default;

void AudioSystem::Update(double dt, Registry* registry, Input* input) {
  //std::vector<void*> seComponents = registry->GetComponents<SoundEffect, Transform>();
  //std::vector<void*> musicComponents = registry->GetComponents<Music>();

  //auto* soundEffects = static_cast<ComponentVector<SoundEffect>*>(seComponents[0]);
  //auto* soundEffectsTransform = static_cast<ComponentVector<Transform>*>(seComponents[1]);
  //auto* music = static_cast<ComponentVector<Music>*>(musicComponents[0]);

  //// gruntcooldown just so it doesnt play a million times a second
  //gruntCooldown += dt;
  //if (gruntCooldown > 1) {
  //  // Play sound effects
  //  for (size_t i = 0; i < soundEffects->Size(); i++) {
  //    SoundEffect& se = soundEffects->At(i);
  //    Transform& seTransform = soundEffectsTransform->At(i);
  //    int sound = SoundEffectsLibrary::Get()->Load(se.filePath);
  //    AudioVariables::sePlayer.SetLooping(se.looping);
  //    AudioVariables::sePlayer.SetPosition(seTransform.position.x, seTransform.position.y, seTransform.position.z);
  //    AudioVariables::sePlayer.Play(sound);
  //    // TODO: remove component once its been implemented;
  //    gruntCooldown = 0;
  //  }
  //}

  // Play Music
  // for (size_t i = 0; i < music->Size(); i++) {
  //  Music& m = music->At(i);
  //  static MusicBuffer musicToPlay(m.filePath);
  //  if (!musicToPlay.IsPlaying()) {
  //    musicToPlay.Play();
  //  } else {
  //    musicToPlay.UpdateBufferStream();
  //  }
  //}
}