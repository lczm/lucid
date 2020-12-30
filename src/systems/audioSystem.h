#pragma once

#include "ecs.h"
#include "constants.h"
#include "soundDevice.h"
#include "musicBuffer.h"
#include "soundEffectsLibrary.h"
#include "soundEffectsPlayer.h"

class AudioSystem : public System
{
 private:
  float gruntCooldown;

 public:
  AudioSystem();
  ~AudioSystem();

  void Update(float dt, Registry* registry, Input* input);
};
