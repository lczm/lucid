#pragma once
#include <al/alc.h>
#include <al/al.h>
#include <stdio.h>

class SoundDevice {
 public:
  static SoundDevice* Get();
  static void Init();

 private:
  SoundDevice();
  ~SoundDevice();

  ALCdevice* p_ALCDevice;
  ALCcontext* p_ALCContext;
};