#include "ecs.h"
#include "soundEffectsLibrary.h"
#include "soundEffectsPlayer.h"
#include "soundDevice.h"
#include "constants.h"
#include "musicBuffer.h"

class AudioSystem : public System {
 private:
  float gruntCooldown;

 public:
  AudioSystem();
  ~AudioSystem();

  void Update(double dt, Registry* registry, Input* input);
};