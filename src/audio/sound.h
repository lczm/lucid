#pragma once
#include <string>
#include "soundEffectsLibrary.h"
#include "soundEffectsPlayer.h"
#include "soundDevice.h"

class Sound
{
 public:
  ALuint soundBuffer;
  bool looping = false;
  bool play = false;
  float gain = 1.0f;

 private:
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

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(                      //
        CEREAL_NVP(path),         //
        CEREAL_NVP(soundBuffer),  //
        CEREAL_NVP(looping),      //
        CEREAL_NVP(play),         //
        CEREAL_NVP(gain)          //
    );
  }
};