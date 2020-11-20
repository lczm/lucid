#include "ecs.h"
#include "soundBuffer.h"
#include "soundDevice.h"
#include "constants.h"
#include "musicBuffer.h"

class AudioSystem : public System {
 private:
 public:
  AudioSystem();
  ~AudioSystem();

  void Update(double dt, Registry* registry, Input* input);
};