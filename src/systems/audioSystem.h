#pragma once

#include <unordered_map>

#include "ecs.h"
#include "constants.h"
#include "soundDevice.h"
#include "soundEffectsLibrary.h"
#include "soundEffectsPlayer.h"

class AudioSystem : public System
{
 private:
  SoundEffectsLibrary* seLibrary;
  std::unordered_map<ALuint, SoundEffectsPlayer*> bufferMap;

 public:
  AudioSystem();
  ~AudioSystem();

  void Update(float dt, Registry* registry, Input* input);
};
