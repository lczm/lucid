#pragma once

#include "ecs.h"
#include "constants.h"
#include "soundDevice.h"
#include "soundEffectsLibrary.h"
#include "soundEffectsPlayer.h"

class AudioSystem : public System
{
 private:
  SoundEffectsLibrary* seLibrary;
  std::map<ALuint, SoundEffectsPlayer*> bufferMap;
  SoundDevice* sd;

 public:
  AudioSystem();
  ~AudioSystem();

  void Update(float dt, Registry* registry, Input* input);
};
