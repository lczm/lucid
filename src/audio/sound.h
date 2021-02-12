#pragma once
#include <string>
#include "soundEffectsLibrary.h"
#include "soundEffectsPlayer.h"
#include "soundDevice.h"

class Sound
{
 public:
  ALuint soundBuffer;
  bool looping;
  bool play;
  float gain;

 private:
  SoundDevice* sd;
  SoundEffectsLibrary* seLibrary;
  SoundEffectsPlayer* player;
  std::string path;

 public:
  Sound();
  Sound(std::string path);
  ~Sound();
  void Play();
  void SetPosition(float x, float y, float z);
  void SetLooping(bool loop);
  void SetFilePath(std::string path);
};