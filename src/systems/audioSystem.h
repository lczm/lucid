#pragma once

#include "ecs.h"
#include "sound.h"
#include "constants.h"

class AudioSystem : public System
{
 private:
  SoundDevice* sd;

 public:
  AudioSystem();
  ~AudioSystem();

  void Update(float dt, Registry* registry, Input* input);
};
