#include "ecs.h"
#include "AL/al.h"

class AudioSystem : public System {
 private:
 public:
  AudioSystem();
  ~AudioSystem();

  void Update(double dt, Registry* registry, Input* input);
};