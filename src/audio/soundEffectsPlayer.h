#pragma once
#include <al/al.h>
#include <iostream>

class SoundEffectsPlayer
{
 private:
  ALuint source;
  ALuint buffer = 0;

 public:
  SoundEffectsPlayer();
  ~SoundEffectsPlayer();

  void Play(const ALuint& buffer_to_play);
  void Stop();
  void Pause();
  void Resume();

  void SetBufferToPlay(const ALuint& buffer_to_play);
  void SetLooping(const bool& loop);
  void SetPosition(const float& x, const float& y, const float& z);

  bool IsPlaying();
};