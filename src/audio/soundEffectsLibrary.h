#pragma once
#include <al/al.h>
#include <al/alext.h>
#include <sndfile.h>
#include <inttypes.h>
#include <vector>

class SoundEffectsLibrary
{
 private:
  SoundEffectsLibrary();
  ~SoundEffectsLibrary();

  std::vector<ALuint> soundEffectBuffers;

 public:
  static SoundEffectsLibrary* Get();

  ALuint Load(const char* filename);
  bool UnLoad(const ALuint& bufferId);
  std::vector<ALuint> GetSoundBuffers();
};
