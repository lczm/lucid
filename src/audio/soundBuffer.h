#pragma once
#include <AL/al.h>
#include <AL/alext.h>
#include <sndfile.h>
#include <vector>
#include <inttypes.h>

class SoundBuffer {
 public:
  static SoundBuffer* Get();
  ALuint AddSoundEffect(const char* filename);
  bool RemoveSoundEffect(const ALuint& buffer);

 private:
  SoundBuffer();
  ~SoundBuffer();

  std::vector<ALuint> p_SoundEffectBuffers;
};
