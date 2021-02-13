#pragma once

#include <unordered_map>

#include "ecs.h"
#include "sound.h"
#include "constants.h"

class AudioSystem : public System
{
 private:
  std::unordered_map<ALuint, SoundEffectsPlayer*> bufferMap;
  SoundDevice* sd;

 public:
  AudioSystem();
  ~AudioSystem();

  void Update(float dt, Registry* registry, Input* input);
};
