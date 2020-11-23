#pragma once
#include <al/alc.h>
#include <al/al.h>
#include <stdio.h>
#include <vector>

class SoundDevice {
 private:
  SoundDevice();
  ~SoundDevice();

  ALCdevice* p_ALCDevice;
  ALCcontext* p_ALCContext;

 public:
  static SoundDevice* Get();
  static void Init();

  void GetLocation(float &x, float &y, float &z);
  void GetOrientation(float &orientation);
  float GetGain();

  void SetLocation(const float &x, const float &y, const float &z);
  void SetOrientation(const float &atx, const float &aty, const float &atz, const float &upx,
                      const float &upy, const float &upz);
  void SetGain(const float &val);
};